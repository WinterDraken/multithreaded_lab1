#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "FairShareMutex.hpp"

template <typename Key, typename Value>
class UpgradedKVStore {
private:
    std::unordered_map<Key, Value> store;
    FairShareMutex rw_lock;

    std::vector<std::shared_ptr<const std::unordered_map<Key, Value>>> snapshot_vector;
    mutable FairShareMutex snapshot_lock;

    
    std::mutex batch_mutex;
    std::condition_variable batch_cv;
    std::vector<std::pair<Key, Value>> pending_batch;
    const size_t batch_threshold = 1000;
    bool stop_thread = false;
    std::thread batch_thread;

    
    void batch_worker() {
        while (true) {
            std::vector<std::pair<Key, Value>> batch_to_write;

            {
                std::unique_lock<std::mutex> lock(batch_mutex);
                batch_cv.wait(lock, [this] {
                    return pending_batch.size() >= batch_threshold || stop_thread;
                });

                if (stop_thread && pending_batch.empty())
                    break;

                batch_to_write.swap(pending_batch);
            }

            rw_lock.lock();
            for (const auto& [k, v] : batch_to_write)
                store[k] = v;
            rw_lock.unlock();
        }
    }

public:
    UpgradedKVStore() {
        
        batch_thread = std::thread([this]() { this->batch_worker(); });
    }

    ~UpgradedKVStore() {
        {
            std::lock_guard<std::mutex> lock(batch_mutex);
            stop_thread = true;
            batch_cv.notify_all();
        }
        batch_thread.join();
    }

    void put(const Key& key, const Value& value) {
        rw_lock.lock();
        store[key] = value;
        rw_lock.unlock();
    }

    void put_batch(const std::vector<std::pair<Key, Value>>& batch) {
        {
            std::lock_guard<std::mutex> lock(batch_mutex);
            pending_batch.insert(pending_batch.end(), batch.begin(), batch.end());
        }
        batch_cv.notify_one();
    }
    

    bool get(const Key& key, Value& value) {
        rw_lock.shared_lock();
        auto it = store.find(key);
        if (it != store.end()) {
            value = it->second;
            rw_lock.shared_unlock();
            return true;
        }
        rw_lock.shared_unlock();
        return false;
    }

    bool upgradeable_get_and_put(const Key& key, const Value& new_val) {
        rw_lock.shared_lock();
        auto it = store.find(key);
        if (it == store.end()) {
            rw_lock.shared_unlock();
            return false;
        }

        
        rw_lock.shared_unlock();
        rw_lock.lock();
        store[key] = new_val;
        rw_lock.unlock();
        return true;
    }

    std::shared_ptr<const std::unordered_map<Key, Value>> snapshot() {
        rw_lock.shared_lock();
        auto snap = std::make_shared<const std::unordered_map<Key, Value>>(store);
        rw_lock.shared_unlock();

        snapshot_lock.lock();
        snapshot_vector.push_back(snap);
        snapshot_lock.unlock();
        return snap;
    }

    void clear_snapshots() {
        snapshot_lock.lock();
        snapshot_vector.clear();
        snapshot_lock.unlock();
    }

    std::shared_ptr<const std::unordered_map<Key, Value>> get_snapshot(size_t index) const {
        snapshot_lock.shared_lock();
        std::shared_ptr<const std::unordered_map<Key, Value>> snap = nullptr;
        if (index < snapshot_vector.size())
            snap = snapshot_vector[index];
        snapshot_lock.shared_unlock();
        return snap;
    }

    void remove_snapshot(size_t index) {
        snapshot_lock.lock();
        if (index < snapshot_vector.size())
            snapshot_vector.erase(snapshot_vector.begin() + index);
        snapshot_lock.unlock();
    }
};

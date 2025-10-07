#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "FairShareMutex.hpp"

template <typename Key, typename Value>
class KVStore{
    private:
        std::unordered_map<Key, Value> store;
        FairShareMutex rw_lock;

        std::vector<std::shared_ptr<const std::unordered_map<Key, Value>>> snapshot_vector;
        mutable FairShareMutex snapshot_lock;

    public:
        void put(const Key& key, const Value& value){
            rw_lock.lock();
            store[key] = value;
            rw_lock.unlock();
        }
        
        void erase(const Key& key){
            rw_lock.lock();
            store.erase(key);
            rw_lock.unlock();
        }

        bool get(const Key& key, Value& value){
            rw_lock.shared_lock();
            auto data = store.find(key);
            if(data != store.end()){
                value = data->second;
                rw_lock.shared_unlock();
                return true;
            }
            rw_lock.shared_unlock();
            return false;

        }

        std::shared_ptr<const std::unordered_map<Key, Value>> snapshot(){
            rw_lock.shared_lock();
            auto snap = std::make_shared<const std::unordered_map <Key, Value>>(store);
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

            if(index < snapshot_vector.size()) {
                snap = snapshot_vector[index];
            }

            snapshot_lock.shared_unlock();
            return snap;
        }

        void remove_snapshot(size_t index) {
            snapshot_lock.lock();             
            if(index < snapshot_vector.size()) {
                snapshot_vector.erase(snapshot_vector.begin() + index);
            }
            snapshot_lock.unlock();
        }


};
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "FairShareMutex.hpp"

template <typename Key, typename Value>
class KVStore{
    private:
        std::unordered_map<Key, Value> store;
        FairShareMutex rw_lock;

        std::vector<std::shared_ptr<const std::unordered_map<Key, Value>>> snapshot_vector;

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

            snapshot_vector.push_back(snap);
            return snap;
        }

        const std::vector<std::shared_ptr<const std::unordered_map<Key, Value>>>& get_snapshots() const {
            return snapshot_vector;
        }

        void clear_snapshots() {
            snapshot_vector.clear();
        }
        
        std::shared_ptr<const std::unordered_map<Key, Value>> get_snapshot(size_t index) const {
            if(index < snapshot_vector.size()) {
                return snapshot_vector[index];
            } else {
                return nullptr; // no snapshot at this index
            }
        }


};
#include <unordered_map>
#include <string>
#include "FairShareMutex.hpp"

template <typename Key, typename Value>
class KVStore{
    private:
        std::unordered_map<Key, Value> store;
        FairShareMutex rw_lock;
    
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


};
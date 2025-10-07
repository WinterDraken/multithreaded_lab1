#include "KVStore.hpp"
#include <thread>
#include <vector>
#include <iostream>

void snapshot_stress(){

    KVStore<int, int> kv;
    
    const int num_threads = 8;
    const int num_ops = 50000;

    auto worker = [&] (int id){
        for(int i = 0; i < num_ops; ++i){
            if(i % 20 == 0){
                kv.put(i, id);
                if (i % 1000 == 0){
                    std::cout << "[Thread " << id << "] Wrote key " << i << " with value " << id << "\n";
                }
            }
            else{
                int val;
                kv.get(i, val);
                if(i % 10000 == 0){
                    std::cout << "[Thread " << id << "] Read key " << i << " got value " << val << "\n";
                }
            }

            if(i % 10000 == 0) {       
                auto snap = kv.snapshot();
                std::cout << "[Thread " << id << "] Snapshot taken at iteration " << i
                          << " with " << snap->size() << " keys.\n";

                
                int count = 0;
                for(const auto& [k,v] : *snap) {
                    if(count++ >= 5) break; 
                    std::cout << "    Snapshot Key: " << k << ", Value: " << v << "\n";
                }
                std::cout << "-------------------------\n";
            }
        }
    };

    std::vector<std::thread> threads;
    for(int t = 0; t < num_threads; ++t)
        threads.emplace_back(worker, t);

    for(auto& th : threads) th.join();
    std::cout << "Snapshot stress test completed.\n";

}
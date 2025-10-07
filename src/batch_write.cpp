#include "UpgradedKVStore.hpp"
#include "batch_write.hpp"
#include <vector>
#include <iostream>
#include <chrono>


void batch_write(){
    UpgradedKVStore<int,int> kv;

    std::vector<std::pair<int,int>> batch;
    
    
    for(int i = 0; i < 10000; i++){
        batch.emplace_back(i, i);
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    kv.put_batch(batch); 


    std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Batch write (thresholded) time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()
              << " µs\n";

              
    UpgradedKVStore<int,int> kv2;
    
    start = std::chrono::high_resolution_clock::now();
    
    for(auto& [k,v]: batch) {
        kv2.put(k,v);
    }
    end = std::chrono::high_resolution_clock::now();
    
    std::cout << "Individual write time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count()
              << " ms\n";
}
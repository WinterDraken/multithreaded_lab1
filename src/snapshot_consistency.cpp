#include "KVStore.hpp"
#include <thread>
#include <vector>
#include <iostream>
#include "snapshot_consistency.hpp"


void snapshot_consistency(){

    KVStore<int, int> kv;

    const int num_threads = 4;
    const int num_ops = 1000;

    auto writer = [&](int id){
        for(int i = 0; i < num_ops; ++i){
            kv.put(i, id * 1000 + i);
        }
    };

    auto snapshot = [&]() {
        for (int i = 0; i < 10; ++i) {
            auto snap = kv.snapshot();

            std::cout << "Snapshot #" << i << " contents:\n";
            for (const auto& [k, v] : *snap) {
                std::cout << "  Key: " << k << ", Value: " << v;
                if (v < k) std::cout << "  <-- INCONSISTENT!";
                std::cout << "\n";
            }
            std::cout << "-------------------------\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };



    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t){
        threads.emplace_back(writer, t);
    }
    std::thread snap_thread(snapshot);

    for (auto& th: threads) th.join();
    snap_thread.join();

    std::cout << "Snapshot consistency test completed.";


}
#include "UpgradedKVStore.hpp"
#include "lock_upgrade.hpp"
#include <thread>
#include <vector>
#include <iostream>


void lock_upgrade(){
    UpgradedKVStore<int,int> kv;
    std::mutex print_mutex;

    for(int i=0;i<1000;i++) {
        kv.put(i,i);
    }

    auto reader = [&](int id){
        for(int i=0;i<500;i++){
            int v;
            kv.get(i,v);

            if(i % 100 == 0) {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cout << "[Reader " << id << "] read key " << i << " = " << v << "\n";
            }
        }
    };

    auto upgrader = [&](int id){
        for(int i=0;i<500;i++){
            kv.upgradeable_get_and_put(i,i+1000);
        
            if(i % 100 == 0) {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cout << "[Upgrader " << id << "] updated key " << i << " to " << i+1000 << "\n";
            }
        }
    };

    std::vector<std::thread> threads;
    for(int i=0;i<4;i++) threads.emplace_back(reader,i);
    threads.emplace_back(upgrader,0);
    threads.emplace_back(upgrader,1);

    for(auto& t: threads) t.join();

    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << "Lock upgrade test completed.\n";

}
#include "KVStore.hpp"
#include <iostream>

void snapshot_isolation(){
    KVStore<int, int> kv;

    for(int i = 0; i < 100; ++i) kv.put(i, i);

    kv.snapshot();

    for(int i = 0; i < 100; ++i) kv.put(i, i + 1000);

    kv.snapshot();

    auto snap1 = kv.get_snapshot(0);
    auto snap2 = kv.get_snapshot(1);

    std::cout << "Snapshot #0 (original state):\n";
    for(const auto& [k,v] : *snap1) {
        std::cout << "  Key: " << k << ", Value: " << v;
        if(v != k) std::cout << "  <-- IMMUTABILITY VIOLATED!";
        std::cout << "\n";
    }
    std::cout << "-------------------------\n";

    std::cout << "Snapshot #1 (after modification):\n";
    for(const auto& [k,v] : *snap2) {
        std::cout << "  Key: " << k << ", Value: " << v;
        if(v != k + 1000) std::cout << "  <-- INCORRECT VALUE!";
        std::cout << "\n";
    }
    std::cout << "-------------------------\n";

    bool snap1_ok = true, snap2_ok = true;
    for(int i = 0; i < 100; ++i) {
        if(snap1->at(i) != i) snap1_ok = false;
        if(snap2->at(i) != i + 1000) snap2_ok = false;
    }

    if(!snap1_ok) std::cerr << "Snapshot #0 immutability test FAILED!\n";
    if(!snap2_ok) std::cerr << "Snapshot #1 correctness test FAILED!\n";

    std::cout << "Snapshot isolation test completed.\n";

}
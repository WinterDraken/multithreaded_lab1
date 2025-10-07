#include <iostream>
#include "snapshot_stress.hpp"
#include "snapshot_consistency.hpp"
#include "snapshot_isolation.hpp"
#include "lock_upgrade.hpp"
#include "batch_write.hpp"

int main() {
    std::cout << "==============================\n";
    std::cout << "Running Snapshot Consistency Test\n";
    std::cout << "==============================\n";
    snapshot_consistency();
    std::cout << "\n\n";

    std::cout << "==============================\n";
    std::cout << "Running Snapshot Isolation Test\n";
    std::cout << "==============================\n";
    snapshot_isolation();
    std::cout << "\n\n";

    std::cout << "==============================\n";
    std::cout << "Running Snapshot Stress Test\n";
    std::cout << "==============================\n";
    snapshot_stress();
    std::cout << "\n\n";

    std::cout << "==============================\n";
    std::cout << "Running Upgraded KVStore Lock Upgrade Test\n";
    std::cout << "==============================\n";
    lock_upgrade();
    std::cout << "\n\n";

    std::cout << "==============================\n";
    std::cout << "Running Upgraded KVStore Batch Write Test\n";
    std::cout << "==============================\n";
    batch_write();
    std::cout << "\n\n";

    std::cout << "All upgraded KVStore benchmarks completed.\n";

    return 0;
}

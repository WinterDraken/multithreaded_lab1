#include <iostream>
#include "snapshot_stress.hpp"
#include "snapshot_consistency.hpp"
#include "snapshot_isolation.hpp"



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

    std::cout << "All snapshot benchmarks completed.\n";

    return 0;
}

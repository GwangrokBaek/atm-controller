#include "test_framework.hpp"

/**
 * @brief Main test runner for ATM Controller
 */

int main() {
    cout << "ATM Controller Test Suite" << endl;
    cout << "=========================" << endl;
    
    // Register all tests from modules
    TestFramework::registerAllTests();
    
    // Run all registered tests
    TestFramework::runAllTests();
    
    return (TestFramework::failed > 0) ? 1 : 0;
}

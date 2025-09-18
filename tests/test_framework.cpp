#include "test_framework.hpp"

extern void test_card_insert_and_eject();
extern void test_invalid_card_operations();
extern void test_withdraw_and_balance();
extern void test_deposit_and_balance();
extern void test_pin_authentication();
extern void test_card_reader_exception_handling();
extern void test_memory_allocation_failure();
extern void test_transaction_rollback_on_cash_failure();

namespace TestFramework {
    int passed = 0;
    int failed = 0;
    
    vector<TestCase> testCases;
    
    void registerTest(const string& name, function<void()> testFunc) {
        testCases.push_back({name, testFunc});
    }
    
    void registerAllTests() {
        testCases.clear();
        
        // Card tests
        registerTest("test_card_insert_and_eject", test_card_insert_and_eject);
        registerTest("test_invalid_card_operations", test_invalid_card_operations);
        
        // Bank tests
        registerTest("test_withdraw_and_balance", test_withdraw_and_balance);
        registerTest("test_deposit_and_balance", test_deposit_and_balance);
        registerTest("test_pin_authentication", test_pin_authentication);
        
        // Exception handling tests
        registerTest("test_card_reader_exception_handling", test_card_reader_exception_handling);
        registerTest("test_memory_allocation_failure", test_memory_allocation_failure);
        registerTest("test_transaction_rollback_on_cash_failure", test_transaction_rollback_on_cash_failure);
    }
    
    void runAllTests() {
        resetStats();
        cout << "Running " << testCases.size() << " test(s)..." << endl;
        cout << "===========================================" << endl;
        
        for (const auto& testCase : testCases) {
            try {
                testCase.testFunction();
            } catch (...) {
                cerr << "Test " << testCase.name << " threw exception" << endl;
                ++failed;
            }
        }
        
        cout << "===========================================" << endl;
        printResults();
    }
    
    void printResults() {
        cout << "Tests passed: " << passed << ", failed: " << failed << endl;
        
        if (failed == 0) {
            cout << "All tests PASSED!" << endl;
        } else {
            cout << "Some tests FAILED!" << endl;
        }
    }
    
    void resetStats() {
        passed = 0;
        failed = 0;
    }
}

#include "test_framework.hpp"

extern void test_card_insert_and_eject();
extern void test_invalid_card_operations();
extern void test_withdraw_and_balance();
extern void test_deposit_and_balance();
extern void test_pin_authentication();

namespace TestFramework {
    int passed = 0;
    int failed = 0;
    
    vector<TestCase> testCases;
    
    void registerTest(const string& name, function<void()> testFunc) {
        testCases.push_back({name, testFunc});
    }
    
    void registerAllTests() {
        testCases.clear();
        
        registerTest("test_card_insert_and_eject", test_card_insert_and_eject);
        registerTest("test_invalid_card_operations", test_invalid_card_operations);
        
        registerTest("test_withdraw_and_balance", test_withdraw_and_balance);
        registerTest("test_deposit_and_balance", test_deposit_and_balance);
        registerTest("test_pin_authentication", test_pin_authentication);
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

#pragma once
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

/**
 * @brief Test framework for ATM Controller tests
 * 
 * Provides macros and utilities for writing and running tests
 */
namespace TestFramework {
    extern int passed;
    extern int failed;
    
    struct TestCase {
        string name;
        function<void()> testFunction;
    };
    
    extern vector<TestCase> testCases;
    
    // Register a test case
    void registerTest(const string& name, function<void()> testFunc);
    
    // Register all tests from modules
    void registerAllTests();
    
    // Run all registered tests
    void runAllTests();
    
    // Print test results
    void printResults();
    
    // Reset test statistics
    void resetStats();
}

#if defined(_MSC_VER)
#define FUNC_NAME __FUNCTION__
#else
#define FUNC_NAME __func__
#endif

/**
 * @brief Define a test function
 * 
 * Usage: TEST(test_name) { ... test code ... } END_TEST
 */
#define TEST(name) \
    void name() { \
        cout << "[ RUN ] " << FUNC_NAME << endl;

/**
 * @brief End test function definition
 */
#define END_TEST \
        cout << "[ OK  ] " << FUNC_NAME << endl; \
        cout << endl; \
    }

/**
 * @brief Require condition to be true, increment appropriate counter
 */
#define REQUIRE(cond) \
    do { \
        if(!(cond)) { \
            cerr << "REQUIRE failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << endl; \
            ++TestFramework::failed; \
        } else { \
            ++TestFramework::passed; \
        } \
    } while(0)

/**
 * @brief Require condition to be true with custom message
 */
#define REQUIRE_MSG(cond, msg) \
    do { \
        if(!(cond)) { \
            cerr << "REQUIRE failed: " #cond << " - " << msg << " at " << __FILE__ << ":" << __LINE__ << endl; \
            ++TestFramework::failed; \
        } else { \
            ++TestFramework::passed; \
        } \
    } while(0)

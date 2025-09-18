#pragma once
#include "Interfaces.hpp"
#include <functional>
#include <vector>

using namespace std;

/**
 * @brief Manages atomic transactions with automatic rollback capability
 */
class TransactionManager {
private:
    struct Operation {
        function<Status()> execute;
        function<void()> rollback;
        bool executed;
        
        Operation(function<Status()> exec, function<void()> roll)
            : execute(exec), rollback(roll), executed(false)
        {}
    };
    
    vector<Operation> operations;
    bool committed;

public:
    TransactionManager() : committed(false) {}
    
    /**
     * @brief Destructor automatically rolls back uncommitted transactions
     */
    ~TransactionManager() {
        if (!committed) {
            rollback();
        }
    }
    
    /**
     * @brief Add an operation to the transaction
     * 
     * @param execute Function to execute the operation
     * @param rollback Function to rollback the operation if needed
     */
    void addOperation(function<Status()> execute, function<void()> rollback) {
        operations.emplace_back(execute, rollback);
    }
    
    /**
     * @brief Execute all operations in the transaction
     */
    Status execute() {
        for (auto& op : operations) {
            Status status = op.execute();
            if (!status.isOk()) {
                return status;
            }
            op.executed = true;
        }
        return Status::okStatus();
    }
    
    /**
     * @brief Commit the transaction
     */
    void commit() {
        committed = true;
    }
    
    /**
     * @brief Manually rollback all executed operations
     */
    void rollback() {
        // Rollback in reverse order
        for (auto it = operations.rbegin(); it != operations.rend(); ++it) {
            if (it->executed) {
                try {
                    it->rollback();
                } catch (...) {
                }
            }
        }
    }
};

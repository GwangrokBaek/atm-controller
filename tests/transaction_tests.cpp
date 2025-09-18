#include "test_framework.hpp"
#include "Controller.hpp"
#include "fakes/FakeCardReader.hpp"
#include "fakes/FakeBank.hpp"
#include "fakes/FakeCashBin.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * @brief Test atomic transaction with rollback when cash dispensing fails
 */
TEST(test_atomic_transaction_rollback)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account = "ACCOUNT-001";
    int initialBalance = 1000;
    int withdrawAmount = 200;
    int cashCapacity = 50;  // Not enough cash
    
    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account}}};
    unordered_map<AccountId, int> balanceMap = {{account, initialBalance}};
    
    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCashBin cashBin(cashCapacity);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);
    
    // Complete authentication flow
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account).isOk());
    
    // Verify initial balance
    auto balance1 = atm.getBalance();
    REQUIRE(balance1.isOk());
    REQUIRE(balance1.value() == initialBalance);
    
    // Attempt withdrawal
    auto withdrawResult = atm.withdraw(withdrawAmount);
    REQUIRE(!withdrawResult.isOk());  // Should fail
    
    // Verify balance
    auto balance2 = atm.getBalance();
    REQUIRE(balance2.isOk());
    REQUIRE(balance2.value() == initialBalance);
    
    REQUIRE(atm.ejectCard().isOk());
END_TEST

/**
 * @brief Test atomic transaction success when all operations succeed
 */
TEST(test_atomic_transaction_success)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account = "ACCOUNT-001";
    int initialBalance = 1000;
    int withdrawAmount = 200;
    int cashCapacity = 10000;  // Enough cash
    
    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account}}};
    unordered_map<AccountId, int> balanceMap = {{account, initialBalance}};
    
    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCashBin cashBin(cashCapacity);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);
    
    // Complete authentication flow
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account).isOk());
    
    // Verify initial balance
    auto balance1 = atm.getBalance();
    REQUIRE(balance1.isOk());
    REQUIRE(balance1.value() == initialBalance);
    
    // Attempt withdrawal
    auto withdrawResult = atm.withdraw(withdrawAmount);
    REQUIRE(withdrawResult.isOk());  // Should succeed
    
    // Verify balance
    auto balance2 = atm.getBalance();
    REQUIRE(balance2.isOk());
    REQUIRE(balance2.value() == initialBalance - withdrawAmount);
    
    // Verify cash bin
    REQUIRE(cashBin.getCurrentCapacity() == cashCapacity - withdrawAmount);
    
    REQUIRE(atm.ejectCard().isOk());
END_TEST

/**
 * @brief Test multiple transactions maintain atomicity independently
 */
TEST(test_multiple_atomic_transactions)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account = "ACCOUNT-001";
    int initialBalance = 1000;
    
    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account}}};
    unordered_map<AccountId, int> balanceMap = {{account, initialBalance}};
    
    FakeBank bank(pinMap, accountsMap, balanceMap);
    FakeCashBin cashBin(1000);  // Not enough cash
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);
    
    // Complete authentication flow
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account).isOk());
    
    // First transaction - should succeed
    REQUIRE(atm.withdraw(100).isOk());
    auto balance1 = atm.getBalance();
    REQUIRE(balance1.isOk());
    REQUIRE(balance1.value() == 900);
    
    // Second transaction - should succeed
    REQUIRE(atm.withdraw(200).isOk());
    auto balance2 = atm.getBalance();
    REQUIRE(balance2.isOk());
    REQUIRE(balance2.value() == 700);
    
    // Third transaction - should fail
    REQUIRE(!atm.withdraw(800).isOk());
    auto balance3 = atm.getBalance();
    REQUIRE(balance3.isOk());
    REQUIRE(balance3.value() == 700);
    
    // Fourth transaction - should succeed
    REQUIRE(atm.withdraw(300).isOk());
    auto balance4 = atm.getBalance();
    REQUIRE(balance4.isOk());
    REQUIRE(balance4.value() == 400);
    
    REQUIRE(atm.ejectCard().isOk());
END_TEST

#include "test_framework.hpp"
#include "Controller.hpp"
#include "fakes/FakeCardReader.hpp"
#include "fakes/FakeBank.hpp"
#include "fakes/FakeCashBin.hpp"
#include <unordered_map>
#include <vector>
#include <stdexcept>

using namespace std;

/**
 * @brief Test exception handling during card insertion
 */
TEST(test_card_reader_exception_handling)
    Card card = "CARD-001";

    FakeBank bank({{"", ""}}, {{"", {}}}, {{},{}});
    FakeCashBin cashBin(0);
    
    // Create a card reader that throws an exception
    class ThrowingCardReader : public ICardReader {
    public:
        Result<Card> read(void) override {
            throw std::runtime_error("Card reader hardware failure");
        }
        
        Status eject(void) override {
            return Status::okStatus();
        }
    };
    
    ThrowingCardReader cardReader;
    Controller atm(cardReader, bank, cashBin);

    auto status = atm.insertCard();
    REQUIRE(!status.isOk());
    REQUIRE_MSG(atm.state() == Controller::State::Idle, "ATM should remain in Idle state after exception");
END_TEST

/**
 * @brief Test memory allocation failure handling
 */
TEST(test_memory_allocation_failure)
    Card card = "CARD-001";

    FakeBank bank({{"", ""}}, {{"", {}}}, {{},{}});
    FakeCashBin cashBin(0);
    
    // Create a card reader that throws bad_alloc
    class MemoryFailureCardReader : public ICardReader {
    public:
        Result<Card> read(void) override {
            throw std::bad_alloc();
        }
        
        Status eject(void) override {
            return Status::okStatus();
        }
    };
    
    MemoryFailureCardReader cardReader;
    Controller atm(cardReader, bank, cashBin);

    REQUIRE(!atm.insertCard().isOk());
    REQUIRE_MSG(atm.state() == Controller::State::Idle, "ATM should remain in Idle state after memory failure");
END_TEST

/**
 * @brief Test transaction rollback on cash dispense failure
 */
TEST(test_transaction_rollback_on_cash_failure)
    Card card = "CARD-001";
    Pin pin = "12345";
    AccountId account1 = "ACCOUNT-001";
    int initialBalance = 1000;
    int withdrawAmount = 100;

    unordered_map<Card, Pin> pinMap = {{card, pin}};
    unordered_map<Card, vector<AccountId>> accountsMap = {{card, {account1}}};
    unordered_map<AccountId, int> balanceMap = {{account1, initialBalance}};

    FakeBank bank(pinMap, accountsMap, balanceMap);
    
    // Create a cash bin that fails on dispense
    class FailingCashBin : public ICashBin {
    public:
        Status canDispense(int money) override {
            return Status::okStatus();
        }
        
        Status dispense(int money) override {
            return Status::error(Err::HardwareError); // fail when actually dispensing
        }
    };
    
    FailingCashBin cashBin;
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);

    // Complete authentication flow
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.enterPin(pin).isOk());
    REQUIRE(atm.selectAccount(account1).isOk());
    
    // Get initial balance
    auto balanceResult = atm.getBalance();
    REQUIRE(balanceResult.isOk());
    REQUIRE(balanceResult.value() == initialBalance);
    
    // Attempt withdrawal - should fail but balance should be restored
    auto withdrawStatus = atm.withdraw(withdrawAmount);
    REQUIRE(!withdrawStatus.isOk());
    
    // Verify balance is unchanged (transaction was rolled back)
    balanceResult = atm.getBalance();
    REQUIRE(balanceResult.isOk());
    REQUIRE_MSG(balanceResult.value() == initialBalance, "Balance should be restored after failed withdrawal");
END_TEST

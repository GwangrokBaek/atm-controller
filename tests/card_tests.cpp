#include "test_framework.hpp"
#include "Controller.hpp"
#include "fakes/FakeCardReader.hpp"
#include "fakes/FakeBank.hpp"
#include "fakes/FakeCashBin.hpp"
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * @brief Test card insertion and ejection functionality
 * 
 * - Cards can be inserted successfully
 * - Card reader stores the correct card value
 * - ATM state changes appropriately during card operations
 * - Cards can be ejected successfully
 */
TEST(test_card_insert_and_eject)
    Card card = "CARD-001";

    // Create minimal fake components for this test
    FakeBank bank({{"", ""}}, {{"", {}}}, {{},{}});
    FakeCashBin cashBin(0);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);

    // Test card insertion
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(cardReader.card.value() == card);
    REQUIRE(cardReader.inserted == true);
    REQUIRE(atm.state() == Controller::State::CardInserted);
    
    // Test card ejection
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(cardReader.ejected == true);
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

/**
 * @brief Test invalid card operations
 * 
 * - Cannot insert card when not in Idle state
 * - Cannot eject card when already in Idle state
 */
TEST(test_invalid_card_operations)
    Card card = "CARD-002";

    FakeBank bank({{"", ""}}, {{"", {}}}, {{},{}});
    FakeCashBin cashBin(0);
    FakeCardReader cardReader(card);
    Controller atm(cardReader, bank, cashBin);

    // Insert card first
    REQUIRE(atm.insertCard().isOk());
    REQUIRE(atm.state() == Controller::State::CardInserted);
    
    // Try to insert another card (should fail)
    REQUIRE(!atm.insertCard().isOk());
    REQUIRE(atm.state() == Controller::State::CardInserted);
    
    // Eject the card
    REQUIRE(atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
    
    // Try to eject again (should fail)
    REQUIRE(!atm.ejectCard().isOk());
    REQUIRE(atm.state() == Controller::State::Idle);
END_TEST

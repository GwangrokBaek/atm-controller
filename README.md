# ATM Controller


## Project Overview

### Key Features

- **Complete ATM Flow**: Insert Card → PIN → Select Account → Balance/Deposit/Withdraw
- **Transaction Atomicity**: ACID-compliant operations with automatic rollback
- **Exception Safety**: Comprehensive error handling for all failure scenarios  
- **Future-Ready Architecture**: Clean interfaces for banking and hardware integration
- **Extensive Testing**: 11 test suites with 90+ validation points
- **Production-Grade Documentation**: Detailed API documentation for all components

## Architecture

### Core Components

```
├── Controller.hpp/cpp     # Main ATM controller logic
├── Interfaces.hpp         # Banking & hardware abstractions
├── TransactionManager.hpp # Atomic transaction management
├── Result.hpp            # Error handling & return types
└── tests/                # Comprehensive test suite
```

### State Machine

```
Idle → CardInserted → Authenticated → AccountSelected
  ↑                                           ↓
  └─────────── ejectCard() ←──────────────────┘
```

### Interface Design

The system uses clean abstractions to support future integration:

- **`IBank`**: Banking service operations (PIN verification, balance, transactions)
- **`ICardReader`**: Card reader operations (read card, eject card)  
- **`ICashBin`**: Cash bin hardware (dispense cash, check capacity)

## Quick Start

### Prerequisites

- **C++17 or later**
- **CMake 3.12+**

### Build & Test

```bash
# Clone and build
git clone https://github.com/GwangrokBaek/atm-controller
cd atm-controller
cmake -S . -B build
cmake --build build -j

# Run tests
.\Debug\atm.exe
```

### Expected Output

```
ATM Controller Test Suite
=========================
Running 11 test(s)...
===========================================
[ RUN ] test_card_insert_and_eject
[ OK  ] test_card_insert_and_eject
...
===========================================
Tests passed: 90, failed: 0
All tests PASSED!
```

## Project Structure

```
atm-controller/
├── README.md                    # This file
├── CMakeLists.txt              # Build configuration
├── include/                    # Header files
│   ├── Controller.hpp          # Main ATM controller
│   ├── Interfaces.hpp          # Banking & hardware interfaces
│   ├── TransactionManager.hpp  # Atomic transaction management
│   └── Result.hpp              # Error handling types
├── src/                        # Implementation files
│   └── Controller.cpp          # Controller implementation
├── tests/                      # Test suite
│   ├── test_framework.hpp/cpp  # Test framework
│   ├── test_runner.cpp         # Main test runner
│   ├── card_tests.cpp          # Card operation tests
│   ├── banking_tests.cpp       # Banking operation tests
│   ├── exception_tests.cpp     # Exception handling tests
│   ├── transaction_tests.cpp   # Transaction atomicity tests
│   └── fakes/                  # Test doubles
│       ├── FakeBank.hpp        # Mock banking service
│       ├── FakeCardReader.hpp  # Mock card reader
│       └── FakeCashBin.hpp     # Mock cash dispenser
└── build/                      # Build artifacts (generated)
```

## Integration Guide

### For UI Developers

```cpp
Controller atm(cardReader, bank, cashBin);

// UI can directly call controller methods
if (atm.insertCard().isOk()) {
    displayMessage("Card inserted successfully");
    // Show PIN entry screen
}
```

### For Banking System Integration

Implement the `IBank` interface:

```cpp
class YourBankingSystem : public IBank {
public:
    Status verifyPin(const Card& card, const Pin& pin) override {
        // Your banking API integration here
    }
    // ... implement other methods
};
```

### For Hardware Integration

Implement hardware interfaces:

```cpp
class YourCardReader : public ICardReader {
public:
    Result<Card> read() override {
        // Your card reader hardware integration
    }
    // ... implement other methods  
};
```

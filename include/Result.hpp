#pragma once
#include <optional>

using namespace std;

enum class Err {
    None = 0,
    InvalidState,
    InvalidArg
};

/**
 * @brief Status wrapper for operations that don't return values
 */
struct Status {
    Err code = Err::None;
    
    bool isOk(void) const
    {
        return code == Err::None;
    }

    static Status okStatus(void)
    {
        return Status{};
    }

    static Status error(Err e)
    {
        return Status{ e };
    }
};

/**
 * @brief Result class that can hold either a value or an error
 * 
 * Usage:
 *  - Check success with isOk()
 *  - Get value with value() (when isOk() is true)
 *  - Get error with error() (when isOk() is false)
 */
template <typename T>
class Result {
private:
    optional<T> _v;
    Err _e{ Err::None };
public:
    Result(const T& value): _v(value)
    {}

    Result(T&& value): _v(move(value))
    {}

    Result(Err error): _v(nullopt), _e(error)
    {}

    bool isOk() const
    {
        return _v.has_value();
    }

    const T& value() const
    {
        return *_v;
    }

    T& value()
    {
        return *_v;
    }

    Err error() const
    {
        return _e;
    }
};
#ifndef CME_TIME_CLOCK_HPP
#define CME_TIME_CLOCK_HPP

#include <cstdint>

/* This class represents something capable of providing the current engine timestamp. */
class CMEClock
{
public: 
    /* Allows derived clock implementations to be destroyed through the clock interface. */
    virtual ~CMEClock() = default;

    /* Returns the current timestamp in microseconds. */
    virtual std::uint64_t now() const = 0;
};

#endif // CME_TIME_CLOCK_HPP
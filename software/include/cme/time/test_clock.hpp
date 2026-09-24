#ifndef CME_TIME_TEST_CLOCK_HPP
#define CME_TIME_TEST_CLOCK_HPP

#include <cstdint>

#include "cme/time/clock.hpp"

/* Provides a manually controlled clock for deterministic tests. */
class CMETestClock : public CMEClock
{
public: 
    /* Creates a test clock with an initial timestamp. */
    CMETestClock(std::uint64_t intialTime = 0);

    /* Returns the current controlled timestamp. */
    std::uint64_t now() const override;

    /* Sets the controlled timestamp to a specific value. */
    void setTime(std::uint64_t newTime);

    /* Advances the controlled timestamp by the specified number of microseconds. */
    void advanceTime(std::uint64_t microseconds);

private:
    // The timestamp currently returned by the test clock.
    std::uint64_t currentTime;
};

#endif // CME_TIME_TEST_CLOCK_HPP
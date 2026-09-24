#ifndef CME_TIME_SYSTEM_CLOCK_HPP
#define CME_TIME_SYSTEM_CLOCK_HPP

#include "cme/time/clock.hpp"

/* Provides timestamps using the computer's system clock. */
class CMESystemClock : public CMEClock
{
public: 
    /* Creates a system clock. */
    CMESystemClock();

    /* Returns the current system timestamp in microseconds. */
    std::uint64_t now() const override;
};

#endif // CME_TIME_SYSTEM_CLOCK_HPP
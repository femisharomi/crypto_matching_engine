#include <chrono>

#include "cme/time/system_clock.hpp"

CMESystemClock::CMESystemClock()
{
    
}

std::uint64_t CMESystemClock::now() const
{
    std::chrono::system_clock::time_point currentTime =
        std::chrono::system_clock::now();

    std::chrono::system_clock::duration timeSinceEpoch =
        currentTime.time_since_epoch();

    std::chrono::microseconds timeInMicroseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(
            timeSinceEpoch);

    std::uint64_t timestamp =
        static_cast<std::uint64_t>(
            timeInMicroseconds.count());

    return timestamp;
}
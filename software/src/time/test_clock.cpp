#include "cme/time/test_clock.hpp"


CMETestClock::CMETestClock(std::uint64_t intialTime) : currentTime(intialTime)
{

}

std::uint64_t CMETestClock::now() const
{
return currentTime;
}

void CMETestClock::setTime(std::uint64_t newTime)
{
currentTime = newTime;
}

void CMETestClock::advanceTime(std::uint64_t microseconds)
{
    currentTime += microseconds;
}
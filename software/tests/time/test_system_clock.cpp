#include "gtest/gtest.h"

#include <cstdint>

#include "cme/time/system_clock.hpp"

// ============================================================================
// CME SYSTEM CLOCK TEST SUITE
// ============================================================================

// ============================================================================
// SYSTEM CLOCK TESTS
// ============================================================================

TEST(CMESystemClockTests, ReturnsNonZeroTimestamp)
{
    CMESystemClock clock;

    std::uint64_t timestamp =
        clock.now();

    EXPECT_GT(
        timestamp,
        0);
}

TEST(CMESystemClockTests, LaterReadDoesNotGoBackwards)
{
    CMESystemClock clock;

    std::uint64_t firstTimestamp =
        clock.now();

    std::uint64_t secondTimestamp =
        clock.now();

    EXPECT_GE(
        secondTimestamp,
        firstTimestamp);
}
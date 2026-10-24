#include "gtest/gtest.h"

#include <cstddef>

#include "cme/engine/matching_engine.hpp"
#include "cme/input/simulated_order_feed.hpp"

// ============================================================================
// SIMULATED ORDER FEED TESTS
// ============================================================================

TEST(CMESimulatedOrderFeedTests, OpensExistingFeedFile)
{
    CMEMatchingEngine engine;

    CMESimulatedOrderFeed feed(
        "software/tests/input/test_feed_orders.txt",
        &engine);

    EXPECT_TRUE(feed.isOpen());
}

TEST(CMESimulatedOrderFeedTests, ProcessesAllCommandsFromFile)
{
    CMEMatchingEngine engine;

    CMESimulatedOrderFeed feed(
        "software/tests/input/test_feed_orders.txt",
        &engine);

    ASSERT_TRUE(feed.isOpen());

    std::size_t processedCommandCount =
        feed.processAll();

    EXPECT_EQ(
        processedCommandCount,
        3);
}
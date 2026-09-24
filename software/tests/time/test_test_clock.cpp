#include "gtest/gtest.h"

#include "cme/time/test_clock.hpp"
#include "cme/order_book/order_book.hpp"
#include "cme/market_data/market_data_snapshot.hpp"
    #include "cme/core/types.hpp"

// ============================================================================
// CME TEST CLOCK TEST SUITE
// ============================================================================

// ============================================================================
// TEST CLOCK TESTS
// ============================================================================

TEST(CMETestClockTests, StartsAtZeroByDefault)
{
    CMETestClock clock;

    EXPECT_EQ(
        clock.now(),
        0);
}

TEST(CMETestClockTests, StoresInitialTime)
{
    CMETestClock clock(1000);

    EXPECT_EQ(
        clock.now(),
        1000);
}

TEST(CMETestClockTests, SetTimeChangesCurrentTime)
{
    CMETestClock clock;

    clock.setTime(5000);

    EXPECT_EQ(
        clock.now(),
        5000);
}

TEST(CMETestClockTests, AdvanceTimeIncreasesCurrentTime)
{
    CMETestClock clock(1000);

    clock.advanceTime(250);

    EXPECT_EQ(
        clock.now(),
        1250);
}

TEST(CMETestClockTests, CanAdvanceTimeMultipleTimes)
{
    CMETestClock clock(1000);

    clock.advanceTime(100);
    clock.advanceTime(200);
    clock.advanceTime(300);

    EXPECT_EQ(
        clock.now(),
        1600);
}

// ============================================================================
// DETERMINISTIC CLOCK TESTS
// ============================================================================

TEST(CMEOrderBookTests, SnapshotUsesProvidedClockTimestamp)
{
    CMETestClock clock(1000);

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        nullptr,
        &clock);

    CMEMarketDataSnapshot snapshot =
        orderBook.getMarketDataSnapshot();

    EXPECT_EQ(
        snapshot.getTimestamp(),
        1000);
}

TEST(CMEOrderBookTests, SnapshotTimestampChangesWhenClockAdvances)
{
    CMETestClock clock(1000);

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        nullptr,
        &clock);

    CMEMarketDataSnapshot firstSnapshot =
        orderBook.getMarketDataSnapshot();

    clock.advanceTime(250);

    CMEMarketDataSnapshot secondSnapshot =
        orderBook.getMarketDataSnapshot();

    EXPECT_EQ(
        firstSnapshot.getTimestamp(),
        1000);

    EXPECT_EQ(
        secondSnapshot.getTimestamp(),
        1250);
}

TEST(CMEOrderBookTests, SnapshotTimestampCanBeSetDeterministically)
{
    CMETestClock clock;

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        nullptr,
        &clock);

    clock.setTime(5000);

    CMEMarketDataSnapshot firstSnapshot =
        orderBook.getMarketDataSnapshot();

    clock.setTime(9000);

    CMEMarketDataSnapshot secondSnapshot =
        orderBook.getMarketDataSnapshot();

    EXPECT_EQ(
        firstSnapshot.getTimestamp(),
        5000);

    EXPECT_EQ(
        secondSnapshot.getTimestamp(),
        9000);
}

TEST(CMEOrderBookTests, SnapshotUsesZeroTimestampWhenNoClockProvided)
{
    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"));

    CMEMarketDataSnapshot snapshot =
        orderBook.getMarketDataSnapshot();

    EXPECT_EQ(
        snapshot.getTimestamp(),
        0);
}

TEST(CMEOrderBookTests, SnapshotContainsSequenceNumberAndTimestamp)
{
    CMETestClock clock(7500);

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        nullptr,
        &clock);

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(10))));

    CMEMarketDataSnapshot snapshot =
        orderBook.getMarketDataSnapshot();

    EXPECT_EQ(
        snapshot.getSequenceNumber(),
        1);

    EXPECT_EQ(
        snapshot.getTimestamp(),
        7500);
}
#include "gtest/gtest.h"

#include "cme/market_data/market_data_snapshot.hpp"

// ============================================================================
// CME MARKET DATA SNAPSHOT TEST SUITE
// ============================================================================

// ============================================================================
// MARKET DATA SNAPSHOT TESTS
// ============================================================================

TEST(CMEMarketDataSnapshotTests, StoresCompleteSnapshotInformation)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEPrice(51000),
        3,
        4);

    EXPECT_EQ(
        snapshot.getSymbol(),
        CMESymbol("BTC-GBP"));

    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    EXPECT_EQ(
        snapshot.getBestBid().value(),
        CMEPrice(50000));

    EXPECT_EQ(
        snapshot.getBestAsk().value(),
        CMEPrice(51000));

    EXPECT_EQ(
        snapshot.getBuyLevelCount(),
        3);

    EXPECT_EQ(
        snapshot.getSellLevelCount(),
        4);
}

TEST(CMEMarketDataSnapshotTests, StoresMissingBestBid)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        std::nullopt,
        CMEPrice(51000),
        0,
        2);

    EXPECT_FALSE(
        snapshot.getBestBid().has_value());

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    EXPECT_EQ(
        snapshot.getBestAsk().value(),
        CMEPrice(51000));
}

TEST(CMEMarketDataSnapshotTests, StoresMissingBestAsk)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        std::nullopt,
        2,
        0);

    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    EXPECT_FALSE(
        snapshot.getBestAsk().has_value());

    EXPECT_EQ(
        snapshot.getBestBid().value(),
        CMEPrice(50000));
}

TEST(CMEMarketDataSnapshotTests, EmptySnapshotCanContainNoBestPrices)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        std::nullopt,
        std::nullopt,
        0,
        0);

    EXPECT_FALSE(
        snapshot.getBestBid().has_value());

    EXPECT_FALSE(
        snapshot.getBestAsk().has_value());

    EXPECT_EQ(
        snapshot.getBuyLevelCount(),
        0);

    EXPECT_EQ(
        snapshot.getSellLevelCount(),
        0);
}
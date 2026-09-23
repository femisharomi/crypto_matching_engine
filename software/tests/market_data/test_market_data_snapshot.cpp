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
        CMEQuantity(75),
        CMEPrice(51000),
        CMEQuantity(40),
        3,
        4,
        {},
        {});

    EXPECT_EQ(
        snapshot.getSymbol(),
        CMESymbol("BTC-GBP"));

    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    ASSERT_TRUE(
        snapshot.getBestBidQuantity().has_value());

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    ASSERT_TRUE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestBid().value(),
        CMEPrice(50000));

    EXPECT_EQ(
        snapshot.getBestBidQuantity().value(),
        CMEQuantity(75));

    EXPECT_EQ(
        snapshot.getBestAsk().value(),
        CMEPrice(51000));

    EXPECT_EQ(
        snapshot.getBestAskQuantity().value(),
        CMEQuantity(40));

    EXPECT_EQ(
        snapshot.getBuyLevelCount(),
        3);

    EXPECT_EQ(
        snapshot.getSellLevelCount(),
        4);
}

TEST(CMEMarketDataSnapshotTests, StoresMissingBestBidAndQuantity)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        std::nullopt,
        std::nullopt,
        CMEPrice(51000),
        CMEQuantity(40),
        0,
        2,
        {},
        {});


    EXPECT_FALSE(
        snapshot.getBestBid().has_value());

    EXPECT_FALSE(
        snapshot.getBestBidQuantity().has_value());

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    ASSERT_TRUE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestAsk().value(),
        CMEPrice(51000));

    EXPECT_EQ(
        snapshot.getBestAskQuantity().value(),
        CMEQuantity(40));
}

TEST(CMEMarketDataSnapshotTests, StoresMissingBestAskAndQuantity)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(75),
        std::nullopt,
        std::nullopt,
        2,
        0,
        {},
        {});


    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    ASSERT_TRUE(
        snapshot.getBestBidQuantity().has_value());

    EXPECT_FALSE(
        snapshot.getBestAsk().has_value());

    EXPECT_FALSE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestBid().value(),
        CMEPrice(50000));

    EXPECT_EQ(
        snapshot.getBestBidQuantity().value(),
        CMEQuantity(75));
}

TEST(CMEMarketDataSnapshotTests, EmptySnapshotContainsNoBestPricesOrQuantities)
{
    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        0,
        0,
        {},
        {});


    EXPECT_FALSE(
        snapshot.getBestBid().has_value());

    EXPECT_FALSE(
        snapshot.getBestBidQuantity().has_value());

    EXPECT_FALSE(
        snapshot.getBestAsk().has_value());

    EXPECT_FALSE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBuyLevelCount(),
        0);

    EXPECT_EQ(
        snapshot.getSellLevelCount(),
        0);
}
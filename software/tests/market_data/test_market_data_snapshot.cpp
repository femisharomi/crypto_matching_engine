#include "gtest/gtest.h"

#include <vector>

#include "cme/market_data/market_data_level.hpp"
#include "cme/market_data/market_data_snapshot.hpp"

// ============================================================================
// CME MARKET DATA SNAPSHOT TEST SUITE
// ============================================================================

// ============================================================================
// MARKET DATA SNAPSHOT TESTS
// ============================================================================

TEST(CMEMarketDataSnapshotTests, StoresCompleteSnapshotInformation)
{
    std::vector<CMEMarketDataLevel> bidLevels;
    bidLevels.push_back(
        CMEMarketDataLevel(
            CMEPrice(50000),
            CMEQuantity(75)));

    std::vector<CMEMarketDataLevel> askLevels;
    askLevels.push_back(
        CMEMarketDataLevel(
            CMEPrice(51000),
            CMEQuantity(40)));

    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(75),
        CMEPrice(51000),
        CMEQuantity(40),
        1,
        1,
        bidLevels,
        askLevels,
        42);

    EXPECT_EQ(
        snapshot.getSymbol(),
        CMESymbol("BTC-GBP"));

    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    EXPECT_EQ(
        snapshot.getBestBid().value(),
        CMEPrice(50000));

    ASSERT_TRUE(
        snapshot.getBestBidQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestBidQuantity().value(),
        CMEQuantity(75));

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    EXPECT_EQ(
        snapshot.getBestAsk().value(),
        CMEPrice(51000));

    ASSERT_TRUE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestAskQuantity().value(),
        CMEQuantity(40));

    EXPECT_EQ(
        snapshot.getBuyLevelCount(),
        1);

    EXPECT_EQ(
        snapshot.getSellLevelCount(),
        1);

    ASSERT_EQ(
        snapshot.getBidLevels().size(),
        1);

    EXPECT_EQ(
        snapshot.getBidLevels().at(0).getPrice(),
        CMEPrice(50000));

    EXPECT_EQ(
        snapshot.getBidLevels().at(0).getQuantity(),
        CMEQuantity(75));

    ASSERT_EQ(
        snapshot.getAskLevels().size(),
        1);

    EXPECT_EQ(
        snapshot.getAskLevels().at(0).getPrice(),
        CMEPrice(51000));

    EXPECT_EQ(
        snapshot.getAskLevels().at(0).getQuantity(),
        CMEQuantity(40));

    EXPECT_EQ(
        snapshot.getSequenceNumber(),
        42);
}

TEST(CMEMarketDataSnapshotTests, StoresMissingBestBidAndQuantity)
{
    std::vector<CMEMarketDataLevel> bidLevels;

    std::vector<CMEMarketDataLevel> askLevels;
    askLevels.push_back(
        CMEMarketDataLevel(
            CMEPrice(51000),
            CMEQuantity(40)));

    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        std::nullopt,
        std::nullopt,
        CMEPrice(51000),
        CMEQuantity(40),
        0,
        1,
        bidLevels,
        askLevels,
        10);

    EXPECT_FALSE(
        snapshot.getBestBid().has_value());

    EXPECT_FALSE(
        snapshot.getBestBidQuantity().has_value());

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    EXPECT_EQ(
        snapshot.getBestAsk().value(),
        CMEPrice(51000));

    ASSERT_TRUE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestAskQuantity().value(),
        CMEQuantity(40));

    EXPECT_TRUE(
        snapshot.getBidLevels().empty());

    ASSERT_EQ(
        snapshot.getAskLevels().size(),
        1);

    EXPECT_EQ(
        snapshot.getSequenceNumber(),
        10);
}

TEST(CMEMarketDataSnapshotTests, StoresMissingBestAskAndQuantity)
{
    std::vector<CMEMarketDataLevel> bidLevels;
    bidLevels.push_back(
        CMEMarketDataLevel(
            CMEPrice(50000),
            CMEQuantity(75)));

    std::vector<CMEMarketDataLevel> askLevels;

    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(75),
        std::nullopt,
        std::nullopt,
        1,
        0,
        bidLevels,
        askLevels,
        15);

    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    EXPECT_EQ(
        snapshot.getBestBid().value(),
        CMEPrice(50000));

    ASSERT_TRUE(
        snapshot.getBestBidQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestBidQuantity().value(),
        CMEQuantity(75));

    EXPECT_FALSE(
        snapshot.getBestAsk().has_value());

    EXPECT_FALSE(
        snapshot.getBestAskQuantity().has_value());

    ASSERT_EQ(
        snapshot.getBidLevels().size(),
        1);

    EXPECT_TRUE(
        snapshot.getAskLevels().empty());

    EXPECT_EQ(
        snapshot.getSequenceNumber(),
        15);
}

TEST(CMEMarketDataSnapshotTests, EmptySnapshotContainsNoBestPricesOrQuantities)
{
    std::vector<CMEMarketDataLevel> bidLevels;
    std::vector<CMEMarketDataLevel> askLevels;

    CMEMarketDataSnapshot snapshot(
        CMESymbol("BTC-GBP"),
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        0,
        0,
        bidLevels,
        askLevels,
        0);

    EXPECT_EQ(
        snapshot.getSymbol(),
        CMESymbol("BTC-GBP"));

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

    EXPECT_TRUE(
        snapshot.getBidLevels().empty());

    EXPECT_TRUE(
        snapshot.getAskLevels().empty());

    EXPECT_EQ(
        snapshot.getSequenceNumber(),
        0);
}
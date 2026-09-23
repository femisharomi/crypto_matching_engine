#include "gtest/gtest.h"
#include "cme/trade/in_memory_trade_publisher.hpp"

// ============================================================================
// CME IN MEMORY TRADE PUBLISHER TEST SUITE
// ============================================================================

// ============================================================================
// IN MEMORY TRADE PUBLISHER TESTS
// ============================================================================

TEST(CMEInMemoryTradePublisherTests, NewPublisherStartsEmpty)
{
    CMEInMemoryTradePublisher publisher;

    EXPECT_EQ(publisher.getTradeCount(), 0);
    EXPECT_TRUE(publisher.getTrades().empty());
}

TEST(CMEInMemoryTradePublisherTests, PublishedTradeIsStored)
{
    CMEInMemoryTradePublisher publisher;

    CMETrade trade(
        CMETradeId(1),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(25));

    publisher.publishTrade(trade);

    EXPECT_EQ(publisher.getTradeCount(), 1);

    const CMETrade& storedTrade =
        publisher.getTrade(0);

    EXPECT_EQ(
        storedTrade.getTradeId().value,
        std::uint64_t(1));

    EXPECT_EQ(
        storedTrade.getBuyOrderId(),
        CMEOrderId(1001));

    EXPECT_EQ(
        storedTrade.getSellOrderId(),
        CMEOrderId(1002));

    EXPECT_EQ(
        storedTrade.getTradeSymbol(),
        CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        storedTrade.getTradePrice(),
        CMEPrice(50000));

    EXPECT_EQ(
        storedTrade.getTradeQuantity(),
        CMEQuantity(25));
}

TEST(CMEInMemoryTradePublisherTests, MultipleTradesRemainInPublicationOrder)
{
    CMEInMemoryTradePublisher publisher;

    CMETrade firstTrade(
        CMETradeId(1),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(25));

    CMETrade secondTrade(
        CMETradeId(2),
        CMEOrderId(1003),
        CMEOrderId(1004),
        CMESymbol("BTC-GBP"),
        CMEPrice(50100),
        CMEQuantity(10));

    publisher.publishTrade(firstTrade);
    publisher.publishTrade(secondTrade);

    EXPECT_EQ(publisher.getTradeCount(), 2);

    EXPECT_EQ(
        publisher.getTrade(0).getTradeId().value,
        std::uint64_t(1));

    EXPECT_EQ(
        publisher.getTrade(1).getTradeId().value,
        std::uint64_t(2));
}

TEST(CMEInMemoryTradePublisherTests, GetTradesReturnsAllPublishedTrades)
{
    CMEInMemoryTradePublisher publisher;

    CMETrade firstTrade(
        CMETradeId(1),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(25));

    CMETrade secondTrade(
        CMETradeId(2),
        CMEOrderId(1003),
        CMEOrderId(1004),
        CMESymbol("ETH-GBP"),
        CMEPrice(3000),
        CMEQuantity(5));

    publisher.publishTrade(firstTrade);
    publisher.publishTrade(secondTrade);

    const std::vector<CMETrade>& trades =
        publisher.getTrades();

    ASSERT_EQ(trades.size(), 2);

    EXPECT_EQ(
        trades.at(0).getTradeId().value,
        std::uint64_t(1));

    EXPECT_EQ(
        trades.at(1).getTradeId().value,
        std::uint64_t(2));

    EXPECT_EQ(
        trades.at(1).getTradeSymbol(),
        CMESymbol("ETH-GBP"));
}

TEST(CMEInMemoryTradePublisherTests, ClearRemovesAllStoredTrades)
{
    CMEInMemoryTradePublisher publisher;

    CMETrade trade(
        CMETradeId(1),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(25));

    publisher.publishTrade(trade);

    ASSERT_EQ(publisher.getTradeCount(), 1);

    publisher.clear();

    EXPECT_EQ(publisher.getTradeCount(), 0);
    EXPECT_TRUE(publisher.getTrades().empty());
}

TEST(CMEInMemoryTradePublisherTests, InvalidTradeIndexThrows)
{
    CMEInMemoryTradePublisher publisher;

    EXPECT_THROW(
        publisher.getTrade(0),
        std::out_of_range);
}
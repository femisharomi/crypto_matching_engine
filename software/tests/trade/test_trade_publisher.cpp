#include "gtest/gtest.h"

#include "cme/order_book/order_book.hpp"
#include "cme/trade/trade_publisher.hpp"

// ============================================================================
// CME TRADE PUBLISHER TEST SUITE
// ============================================================================

// ============================================================================
// TRADE PUBLISHER TESTS
// ============================================================================

class CMETestTradePublisher : public CMETradePublisher
{
public:
    CMETestTradePublisher()
        : publishCount(0),
          lastPublishedTrade(std::nullopt)
    {
    }

    void publishTrade(const CMETrade& trade) override
    {
        publishCount++;
        lastPublishedTrade = trade;
    }

    std::size_t publishCount;

    std::optional<CMETrade> lastPublishedTrade;
};

TEST(CMETradePublisherTests, MatchingOrderPublishesGeneratedTrade)
{
    CMETestTradePublisher publisher;

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        &publisher);

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_EQ(publisher.publishCount, 1);

    ASSERT_TRUE(
        publisher.lastPublishedTrade.has_value());

    EXPECT_EQ(
        publisher.lastPublishedTrade->getBuyOrderId(),
        CMEOrderId(1002));

    EXPECT_EQ(
        publisher.lastPublishedTrade->getSellOrderId(),
        CMEOrderId(1001));

    EXPECT_EQ(
        publisher.lastPublishedTrade->getTradePrice(),
        CMEPrice(50000));

    EXPECT_EQ(
        publisher.lastPublishedTrade->getTradeQuantity(),
        CMEQuantity(25));
}

TEST(CMETradePublisherTests, RestingOrderDoesNotPublishTrade)
{
    CMETestTradePublisher publisher;

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        &publisher);

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_EQ(publisher.publishCount, 0);

    EXPECT_FALSE(
        publisher.lastPublishedTrade.has_value());
}

TEST(CMETradePublisherTests, OrderBookWorksWithoutPublisher)
{
    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    ASSERT_TRUE(
        orderBook.getLastTrade().has_value());

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradeQuantity(),
        CMEQuantity(25));
}
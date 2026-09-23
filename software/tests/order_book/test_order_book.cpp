#include "gtest/gtest.h"

#include "cme/order_book/order_book.hpp"
#include "cme/trade/in_memory_trade_publisher.hpp"

// ============================================================================
// CME ORDER BOOK TEST SUITE
// ============================================================================

// ============================================================================
// ORDER BOOK CREATION
// ============================================================================

TEST(CMEOrderBookTests, NewOrderBookStartsEmpty)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_EQ(orderBook.getSymbol(), CMESymbol("BTC-GBP"));
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

// ============================================================================
// ADD ORDERS AND PRICE PRIORITY
// ============================================================================

TEST(CMEOrderBookTests, AddsBuyAndSellOrders)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(49000),
            CMEQuantity(25))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(51000),
            CMEQuantity(25))));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(49000));
    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(51000));
}

TEST(CMEOrderBookTests, ReturnsBestBidAndBestAsk)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(49000),
            CMEQuantity(25)));

    orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25)));

    orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1003),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(52000),
            CMEQuantity(25)));

    orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1004),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(51000),
            CMEQuantity(25)));

    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(50000));
    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(51000));
}

// ============================================================================
// VALIDATION
// ============================================================================

TEST(CMEOrderBookTests, RejectsInvalidOrders)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_FALSE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(0),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_FALSE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("ETH-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_FALSE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(0),
            CMEQuantity(25))));

    EXPECT_FALSE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1003),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(0))));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

// ============================================================================
// BASIC MATCHING
// ============================================================================

TEST(CMEOrderBookTests, MatchingOrdersCreateTrade)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

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

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(
        orderBook.getLastTrade()->getBuyOrderId(),
        CMEOrderId(1002));

    EXPECT_EQ(
        orderBook.getLastTrade()->getSellOrderId(),
        CMEOrderId(1001));

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradePrice(),
        CMEPrice(50000));

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradeQuantity(),
        CMEQuantity(25));
}

// ============================================================================
// PARTIAL FILLS
// ============================================================================

TEST(CMEOrderBookTests, PartialFillLeavesRemainingQuantity)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(100))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(40))));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    const CMEOrder& remainingOrder =
        orderBook.getSellLevel(CMEPrice(50000))
            .getFrontOrder();

    EXPECT_EQ(
        remainingOrder.getOrderRemainingQuantity(),
        CMEQuantity(60));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradeQuantity(),
        CMEQuantity(40));
}

// ============================================================================
// MULTI-FILL MATCHING
// ============================================================================

TEST(CMEOrderBookTests, IncomingOrderCanMatchMultipleRestingOrders)
{
    CMEInMemoryTradePublisher publisher;

    CMEOrderBook orderBook(
        CMESymbol("BTC-GBP"),
        &publisher);

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(20))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(30))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1003),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(50))));

    ASSERT_EQ(publisher.getTradeCount(), 2);

    EXPECT_EQ(
        publisher.getTrade(0).getTradeQuantity(),
        CMEQuantity(20));

    EXPECT_EQ(
        publisher.getTrade(1).getTradeQuantity(),
        CMEQuantity(30));

    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
}

// ============================================================================
// CANCELLATION
// ============================================================================

TEST(CMEOrderBookTests, CancelsExistingOrder)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_TRUE(
        orderBook.cancelOrder(CMEOrderId(1001)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
}

TEST(CMEOrderBookTests, CancellingUnknownOrderReturnsFalse)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_FALSE(
        orderBook.cancelOrder(CMEOrderId(9999)));
}

// ============================================================================
// MODIFICATION
// ============================================================================

TEST(CMEOrderBookTests, ModifiesExistingOrder)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_TRUE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(51000),
            CMEQuantity(50)));

    EXPECT_EQ(
        orderBook.getBestBid(),
        CMEPrice(51000));

    const CMEOrder& modifiedOrder =
        orderBook.getBuyLevel(CMEPrice(51000))
            .getFrontOrder();

    EXPECT_EQ(
        modifiedOrder.getOrderRemainingQuantity(),
        CMEQuantity(50));
}

// ============================================================================
// MARKET ORDERS
// ============================================================================

TEST(CMEOrderBookTests, MarketOrderMatchesAvailableLiquidity)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

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
            CMEPrice(0),
            CMEQuantity(25),
            true)));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradePrice(),
        CMEPrice(50000));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, UnmatchedMarketOrderNeverRests)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_FALSE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(0),
            CMEQuantity(25),
            true)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

// ============================================================================
// IMMEDIATE OR CANCEL
// ============================================================================

TEST(CMEOrderBookTests, ImmediateOrCancelDiscardsRemainingQuantity)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

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
            CMEQuantity(100),
            false,
            CMETimeInForce::IOC)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradeQuantity(),
        CMEQuantity(25));
}

// ============================================================================
// FILL OR KILL
// ============================================================================

TEST(CMEOrderBookTests, FillOrKillExecutesWhenFullyMatchable)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(50))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(50),
            false,
            CMETimeInForce::FOK)));

    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(
        orderBook.getLastTrade()->getTradeQuantity(),
        CMEQuantity(50));
}

TEST(CMEOrderBookTests, FillOrKillRejectsWhenNotFullyMatchable)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_FALSE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(50),
            false,
            CMETimeInForce::FOK)));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    EXPECT_FALSE(
        orderBook.getLastTrade().has_value());
}

// ============================================================================
// PROCESS ORDER RESULTS
// ============================================================================

TEST(CMEOrderBookTests, ProcessOrderReturnsRestingStatus)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEMatchingResult result =
        orderBook.processOrder(
            CMEOrder(
                CMEOrderId(1001),
                CMESymbol("BTC-GBP"),
                CMESide::BUY,
                CMEPrice(50000),
                CMEQuantity(25)));

    EXPECT_EQ(
        result.getStatus(),
        CMEMatchingStatus::RESTING);

    EXPECT_FALSE(result.hasTrade());
    EXPECT_FALSE(result.hasRejection());
}

TEST(CMEOrderBookTests, ProcessOrderReturnsRejectedStatus)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEMatchingResult result =
        orderBook.processOrder(
            CMEOrder(
                CMEOrderId(1001),
                CMESymbol("ETH-GBP"),
                CMESide::BUY,
                CMEPrice(50000),
                CMEQuantity(25)));

    EXPECT_EQ(
        result.getStatus(),
        CMEMatchingStatus::REJECTED);

    EXPECT_TRUE(result.hasRejection());
    EXPECT_FALSE(result.hasTrade());
}

// ============================================================================
// MARKET DATA SNAPSHOT
// ============================================================================

TEST(CMEOrderBookTests, EmptyBookProducesEmptyMarketDataSnapshot)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEMarketDataSnapshot snapshot =
        orderBook.getMarketDataSnapshot();

    EXPECT_EQ(
        snapshot.getSymbol(),
        CMESymbol("BTC-GBP"));

    EXPECT_FALSE(
        snapshot.getBestBid().has_value());

    EXPECT_FALSE(
        snapshot.getBestAsk().has_value());

    EXPECT_FALSE(
        snapshot.getBestBidQuantity().has_value());

    EXPECT_FALSE(
        snapshot.getBestAskQuantity().has_value());
}

TEST(CMEOrderBookTests, SnapshotContainsBestPricesAndQuantities)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(50))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1003),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(51000),
            CMEQuantity(40))));

    CMEMarketDataSnapshot snapshot =
        orderBook.getMarketDataSnapshot();

    ASSERT_TRUE(
        snapshot.getBestBid().has_value());

    ASSERT_TRUE(
        snapshot.getBestAsk().has_value());

    ASSERT_TRUE(
        snapshot.getBestBidQuantity().has_value());

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
}

TEST(CMEOrderBookTests, SnapshotQuantityChangesAfterPartialFill)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(51000),
            CMEQuantity(100))));

    EXPECT_TRUE(orderBook.addOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(51000),
            CMEQuantity(40))));

    CMEMarketDataSnapshot snapshot =
        orderBook.getMarketDataSnapshot();

    ASSERT_TRUE(
        snapshot.getBestAskQuantity().has_value());

    EXPECT_EQ(
        snapshot.getBestAskQuantity().value(),
        CMEQuantity(60));
}
#include "gtest/gtest.h"

#include "cme/engine/matching_engine.hpp"

// ============================================================================
// CME MATCHING ENGINE TEST SUITE
// ============================================================================

// ============================================================================
// MATCHING ENGINE TESTS
// ============================================================================

TEST(CMEMatchingEngineTests, NewEngineStartsWithNoOrderBooks)
{
    CMEMatchingEngine engine;

    EXPECT_EQ(engine.getOrderBookCount(), 0);
}

TEST(CMEMatchingEngineTests, ProcessingFirstOrderCreatesOrderBook)
{
    CMEMatchingEngine engine;

    engine.processOrder(
        CMEOrder(
            CMEOrderId(1),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(10)));

    EXPECT_EQ(engine.getOrderBookCount(), 1);
    EXPECT_TRUE(engine.containsOrderBook(CMESymbol("BTC-GBP")));
}

TEST(CMEMatchingEngineTests, OrdersForSameSymbolReuseExistingOrderBook)
{
    CMEMatchingEngine engine;

    engine.processOrder(
        CMEOrder(
            CMEOrderId(1),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(10)));

    engine.processOrder(
        CMEOrder(
            CMEOrderId(2),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(51000),
            CMEQuantity(5)));

    EXPECT_EQ(engine.getOrderBookCount(), 1);
}

TEST(CMEMatchingEngineTests, DifferentSymbolsCreateDifferentOrderBooks)
{
    CMEMatchingEngine engine;

    engine.processOrder(
        CMEOrder(
            CMEOrderId(1),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(10)));

    engine.processOrder(
        CMEOrder(
            CMEOrderId(2),
            CMESymbol("ETH-GBP"),
            CMESide::BUY,
            CMEPrice(3000),
            CMEQuantity(5)));

    EXPECT_EQ(engine.getOrderBookCount(), 2);

    EXPECT_TRUE(engine.containsOrderBook(CMESymbol("BTC-GBP")));
    EXPECT_TRUE(engine.containsOrderBook(CMESymbol("ETH-GBP")));
}

TEST(CMEMatchingEngineTests, ReturnedOrderBookContainsProcessedOrders)
{
    CMEMatchingEngine engine;

    engine.processOrder(
        CMEOrder(
            CMEOrderId(1),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(10)));

    const CMEOrderBook& book =
        engine.getOrderBook(CMESymbol("BTC-GBP"));

    EXPECT_EQ(book.getBuyLevelCount(), 1);
}
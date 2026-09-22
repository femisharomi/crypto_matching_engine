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

// ============================================================================
// ENGINE COMMAND PROCESSING TESTS
// ============================================================================

TEST(CMEMatchingEngineTests, SubmitCommandAddsOrderToCorrectBook)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEEngineCommand command(order);

    EXPECT_TRUE(engine.processCommand(command));

    EXPECT_TRUE(
        engine.containsOrderBook(
            CMESymbol("BTC-GBP")));

    const CMEOrderBook& orderBook =
        engine.getOrderBook(
            CMESymbol("BTC-GBP"));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
}

TEST(CMEMatchingEngineTests, CancelCommandRemovesExistingOrder)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(
        engine.processCommand(
            CMEEngineCommand(order)));

    CMEEngineCommand cancelCommand =
        CMEEngineCommand::createCancelCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001));

    EXPECT_TRUE(
        engine.processCommand(cancelCommand));

    const CMEOrderBook& orderBook =
        engine.getOrderBook(
            CMESymbol("BTC-GBP"));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
}

TEST(CMEMatchingEngineTests, CancelCommandReturnsFalseForUnknownOrderBook)
{
    CMEMatchingEngine engine;

    CMEEngineCommand cancelCommand =
        CMEEngineCommand::createCancelCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001));

    EXPECT_FALSE(
        engine.processCommand(cancelCommand));
}

TEST(CMEMatchingEngineTests, ModifyCommandChangesExistingOrder)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(
        engine.processCommand(
            CMEEngineCommand(order)));

    CMEEngineCommand modifyCommand =
        CMEEngineCommand::createModifyCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001),
            CMEPrice(51000),
            CMEQuantity(50));

    EXPECT_TRUE(
        engine.processCommand(modifyCommand));

    const CMEOrderBook& orderBook =
        engine.getOrderBook(
            CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        orderBook.getBestBid(),
        CMEPrice(51000));

    const CMEOrder& modifiedOrder =
        orderBook
            .getBuyLevel(CMEPrice(51000))
            .getFrontOrder();

    EXPECT_EQ(
        modifiedOrder.getOrderId(),
        CMEOrderId(1001));

    EXPECT_EQ(
        modifiedOrder.getOrderRemainingQuantity(),
        CMEQuantity(50));
}

TEST(CMEMatchingEngineTests, ModifyCommandReturnsFalseForUnknownOrderBook)
{
    CMEMatchingEngine engine;

    CMEEngineCommand modifyCommand =
        CMEEngineCommand::createModifyCommand(
            CMESymbol("ETH-GBP"),
            CMEOrderId(9999),
            CMEPrice(3000),
            CMEQuantity(10));

    EXPECT_FALSE(
        engine.processCommand(modifyCommand));
}

TEST(CMEMatchingEngineTests, CommandsForDifferentSymbolsRemainSeparated)
{
    CMEMatchingEngine engine;

    CMEOrder btcOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder ethOrder(
        CMEOrderId(2001),
        CMESymbol("ETH-GBP"),
        CMESide::BUY,
        CMEPrice(3000),
        CMEQuantity(10));

    EXPECT_TRUE(
        engine.processCommand(
            CMEEngineCommand(btcOrder)));

    EXPECT_TRUE(
        engine.processCommand(
            CMEEngineCommand(ethOrder)));

    EXPECT_EQ(engine.getOrderBookCount(), 2);

    const CMEOrderBook& btcBook =
        engine.getOrderBook(
            CMESymbol("BTC-GBP"));

    const CMEOrderBook& ethBook =
        engine.getOrderBook(
            CMESymbol("ETH-GBP"));

    EXPECT_EQ(
        btcBook.getBestBid(),
        CMEPrice(50000));

    EXPECT_EQ(
        ethBook.getBestBid(),
        CMEPrice(3000));
}
#include "gtest/gtest.h"

#include <optional>

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

TEST(CMEMatchingEngineTests, ProcessingOrderCreatesOrderBook)
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

    EXPECT_TRUE(
        engine.containsOrderBook(
            CMESymbol("BTC-GBP")));

    const CMEOrderBook& orderBook =
        engine.getOrderBook(
            CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        orderBook.getBuyLevelCount(),
        1);
}

TEST(CMEMatchingEngineTests, DifferentSymbolsUseDifferentOrderBooks)
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

    EXPECT_TRUE(
        engine.containsOrderBook(
            CMESymbol("BTC-GBP")));

    EXPECT_TRUE(
        engine.containsOrderBook(
            CMESymbol("ETH-GBP")));

    EXPECT_EQ(
        engine
            .getOrderBook(CMESymbol("BTC-GBP"))
            .getBestBid(),
        CMEPrice(50000));

    EXPECT_EQ(
        engine
            .getOrderBook(CMESymbol("ETH-GBP"))
            .getBestBid(),
        CMEPrice(3000));
}

// ============================================================================
// ENGINE EVENT PROCESSING TESTS
// ============================================================================

TEST(CMEMatchingEngineTests, SubmitCommandProducesOrderProcessedEvent)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEEngineEvent event =
        engine.processCommandWithEvent(
            CMEEngineCommand(order));

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_PROCESSED);

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(1001));

    ASSERT_TRUE(
        event.hasMatchingResult());

    EXPECT_EQ(
        event.getMatchingResult()->getStatus(),
        CMEMatchingStatus::RESTING);
}

TEST(CMEMatchingEngineTests, InvalidSubmittedOrderProducesRejectedMatchingResult)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(0),
        CMEQuantity(25));

    CMEEngineEvent event =
        engine.processCommandWithEvent(
            CMEEngineCommand(order));

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_PROCESSED);

    ASSERT_TRUE(
        event.hasMatchingResult());

    EXPECT_EQ(
        event.getMatchingResult()->getStatus(),
        CMEMatchingStatus::REJECTED);

    ASSERT_TRUE(
        event.getMatchingResult()
            ->getRejection()
            .has_value());

    EXPECT_EQ(
        event.getMatchingResult()
            ->getRejection()
            ->getReason(),
        CMEOrderRejectionReason::INVALID_PRICE);
}

TEST(CMEMatchingEngineTests, CancelCommandProducesOrderCancelledEvent)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    engine.processCommand(
        CMEEngineCommand(order));

    CMEEngineCommand cancelCommand =
        CMEEngineCommand::createCancelCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001));

    CMEEngineEvent event =
        engine.processCommandWithEvent(
            cancelCommand);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_CANCELLED);

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(1001));

    EXPECT_FALSE(
        event.hasMatchingResult());

    EXPECT_EQ(
        engine
            .getOrderBook(CMESymbol("BTC-GBP"))
            .getBuyLevelCount(),
        0);
}

TEST(CMEMatchingEngineTests, ModifyCommandProducesOrderModifiedEvent)
{
    CMEMatchingEngine engine;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    engine.processCommand(
        CMEEngineCommand(order));

    CMEEngineCommand modifyCommand =
        CMEEngineCommand::createModifyCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001),
            CMEPrice(51000),
            CMEQuantity(50));

    CMEEngineEvent event =
        engine.processCommandWithEvent(
            modifyCommand);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_MODIFIED);

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(1001));

    const CMEOrderBook& orderBook =
        engine.getOrderBook(
            CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        orderBook.getBestBid(),
        CMEPrice(51000));

    EXPECT_EQ(
        orderBook
            .getBuyLevel(CMEPrice(51000))
            .getFrontOrder()
            .getOrderRemainingQuantity(),
        CMEQuantity(50));
}

TEST(CMEMatchingEngineTests, UnknownCommandProducesCommandRejectedEvent)
{
    CMEMatchingEngine engine;

    CMEEngineCommand cancelCommand =
        CMEEngineCommand::createCancelCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(9999));

    CMEEngineEvent event =
        engine.processCommandWithEvent(
            cancelCommand);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::COMMAND_REJECTED);

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(9999));

    EXPECT_FALSE(
        event.hasMatchingResult());
}

// ============================================================================
// TRADE PUBLISHER INTEGRATION TESTS
// ============================================================================

class CMEMatchingEngineTestTradePublisher : public CMETradePublisher
{
public:
    CMEMatchingEngineTestTradePublisher()
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

TEST(CMEMatchingEngineTests, EnginePassesPublisherToCreatedOrderBooks)
{
    CMEMatchingEngineTestTradePublisher publisher;

    CMEMatchingEngine engine(&publisher);

    engine.processOrder(
        CMEOrder(
            CMEOrderId(1001),
            CMESymbol("BTC-GBP"),
            CMESide::SELL,
            CMEPrice(50000),
            CMEQuantity(25)));

    engine.processOrder(
        CMEOrder(
            CMEOrderId(1002),
            CMESymbol("BTC-GBP"),
            CMESide::BUY,
            CMEPrice(50000),
            CMEQuantity(25)));

    EXPECT_EQ(
        publisher.publishCount,
        1);

    ASSERT_TRUE(
        publisher.lastPublishedTrade.has_value());

    EXPECT_EQ(
        publisher.lastPublishedTrade
            ->getTradeQuantity(),
        CMEQuantity(25));
}
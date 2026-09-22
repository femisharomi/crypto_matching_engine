#include "gtest/gtest.h"

#include "cme/engine/engine_event.hpp"

// ============================================================================
// CME ENGINE EVENT TEST SUITE
// ============================================================================

// ============================================================================
// ENGINE EVENT TESTS
// ============================================================================

TEST(CMEEngineEventTests, StoresOrderProcessedEvent)
{
    CMEMatchingResult result(
        CMEOrderId(1001),
        CMEMatchingStatus::RESTING,
        std::nullopt,
        std::nullopt);

    CMEEngineEvent event(
        CMEEngineEventType::ORDER_PROCESSED,
        CMESymbol("BTC-GBP"),
        CMEOrderId(1001),
        result);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_PROCESSED);

    EXPECT_EQ(
        event.getSymbol(),
        CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(1001));

    ASSERT_TRUE(event.hasMatchingResult());
    ASSERT_TRUE(event.getMatchingResult().has_value());

    EXPECT_EQ(
        event.getMatchingResult()->getStatus(),
        CMEMatchingStatus::RESTING);
}

TEST(CMEEngineEventTests, StoresCancellationEventWithoutMatchingResult)
{
    CMEEngineEvent event(
        CMEEngineEventType::ORDER_CANCELLED,
        CMESymbol("BTC-GBP"),
        CMEOrderId(1001),
        std::nullopt);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_CANCELLED);

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(1001));

    EXPECT_FALSE(event.hasMatchingResult());
}

TEST(CMEEngineEventTests, StoresModificationEventWithoutMatchingResult)
{
    CMEEngineEvent event(
        CMEEngineEventType::ORDER_MODIFIED,
        CMESymbol("BTC-GBP"),
        CMEOrderId(1001),
        std::nullopt);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::ORDER_MODIFIED);

    EXPECT_EQ(
        event.getSymbol(),
        CMESymbol("BTC-GBP"));

    EXPECT_FALSE(event.hasMatchingResult());
}

TEST(CMEEngineEventTests, StoresRejectedCommandEvent)
{
    CMEEngineEvent event(
        CMEEngineEventType::COMMAND_REJECTED,
        CMESymbol("BTC-GBP"),
        CMEOrderId(9999),
        std::nullopt);

    EXPECT_EQ(
        event.getEventType(),
        CMEEngineEventType::COMMAND_REJECTED);

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(9999));

    EXPECT_FALSE(event.hasMatchingResult());
}
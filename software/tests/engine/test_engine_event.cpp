#include "gtest/gtest.h"

#include "cme/engine/engine_event.hpp"

// ============================================================================
// CME ENGINE EVENT TEST SUITE
// ============================================================================

// ============================================================================
// ENGINE EVENT TESTS
// ============================================================================

TEST(CMEEngineEventTests, StoresOrderProcessedEventWithMatchingResult)
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

    ASSERT_TRUE(
        event.hasMatchingResult());

    EXPECT_EQ(
        event.getMatchingResult()->getStatus(),
        CMEMatchingStatus::RESTING);
}

TEST(CMEEngineEventTests, StoresEventWithoutMatchingResult)
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
        event.getSymbol(),
        CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        event.getOrderId(),
        CMEOrderId(9999));

    EXPECT_FALSE(
        event.hasMatchingResult());
}
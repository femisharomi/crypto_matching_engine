#include "gtest/gtest.h"

#include "cme/matching/matching_result.hpp"

// ============================================================================
// CME MATCHING RESULT TEST SUITE
// ============================================================================

// ============================================================================
// MATCHING RESULT TESTS
// ============================================================================

TEST(CMEMatchingResultTests, StoresRestingOrderResult)
{
    CMEMatchingResult result(
        CMEOrderId(1001),
        CMEMatchingStatus::RESTING,
        std::nullopt,
        std::nullopt);

    EXPECT_EQ(result.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(result.getStatus(), CMEMatchingStatus::RESTING);

    EXPECT_FALSE(result.hasTrade());
    EXPECT_FALSE(result.hasRejection());
}

TEST(CMEMatchingResultTests, StoresFilledOrderWithTrade)
{
    CMETrade trade(
        CMETradeId(5001),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(25));

    CMEMatchingResult result(
        CMEOrderId(1002),
        CMEMatchingStatus::FILLED,
        trade,
        std::nullopt);

    EXPECT_EQ(result.getOrderId(), CMEOrderId(1002));
    EXPECT_EQ(result.getStatus(), CMEMatchingStatus::FILLED);

    ASSERT_TRUE(result.hasTrade());
    ASSERT_TRUE(result.getTrade().has_value());

    EXPECT_EQ(
        result.getTrade()->getTradeId().value,
        std::uint64_t(5001));

    EXPECT_EQ(
        result.getTrade()->getTradeQuantity(),
        CMEQuantity(25));

    EXPECT_FALSE(result.hasRejection());
}

TEST(CMEMatchingResultTests, StoresPartiallyFilledOrderWithTrade)
{
    CMETrade trade(
        CMETradeId(5001),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(20));

    CMEMatchingResult result(
        CMEOrderId(1002),
        CMEMatchingStatus::PARTIALLY_FILLED,
        trade,
        std::nullopt);

    EXPECT_EQ(
        result.getStatus(),
        CMEMatchingStatus::PARTIALLY_FILLED);

    ASSERT_TRUE(result.hasTrade());
    ASSERT_TRUE(result.getTrade().has_value());

    EXPECT_EQ(
        result.getTrade()->getTradeId().value,
        std::uint64_t(5001));

    EXPECT_EQ(
        result.getTrade()->getTradeQuantity(),
        CMEQuantity(20));

    EXPECT_FALSE(result.hasRejection());
}

TEST(CMEMatchingResultTests, StoresCancelledOrderWithoutTrade)
{
    CMEMatchingResult result(
        CMEOrderId(1001),
        CMEMatchingStatus::CANCELLED,
        std::nullopt,
        std::nullopt);

    EXPECT_EQ(
        result.getStatus(),
        CMEMatchingStatus::CANCELLED);

    EXPECT_FALSE(result.hasTrade());
    EXPECT_FALSE(result.hasRejection());
}

TEST(CMEMatchingResultTests, StoresRejectedOrderWithRejectionInformation)
{
    CMEOrderRejection rejection(
        CMEOrderId(1001),
        CMEOrderRejectionReason::INVALID_PRICE,
        "Price must be greater than zero.");

    CMEMatchingResult result(
        CMEOrderId(1001),
        CMEMatchingStatus::REJECTED,
        std::nullopt,
        rejection);

    EXPECT_EQ(
        result.getOrderId(),
        CMEOrderId(1001));

    EXPECT_EQ(
        result.getStatus(),
        CMEMatchingStatus::REJECTED);

    EXPECT_FALSE(result.hasTrade());

    ASSERT_TRUE(result.hasRejection());
    ASSERT_TRUE(result.getRejection().has_value());

    EXPECT_EQ(
        result.getRejection()->getReason(),
        CMEOrderRejectionReason::INVALID_PRICE);

    EXPECT_EQ(
        result.getRejection()->getMessage(),
        "Price must be greater than zero.");
}
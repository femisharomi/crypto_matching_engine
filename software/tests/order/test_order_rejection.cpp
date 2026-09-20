#include <gtest/gtest.h>

#include "cme/order/order_rejection.hpp"

// ============================================================================
// CME ORDER REJECTION TEST SUITE
// ============================================================================

// ============================================================================
// REJECTION MODEL TESTS
// ============================================================================

TEST(CMEOrderRejectionTests, ConstructorStoresAllRejectionInformation)
{
    CMEOrderRejection rejection(
        CMEOrderId(1001),
        CMEOrderRejectionReason::INVALID_PRICE,
        "Price must be greater than zero.");

    EXPECT_EQ(rejection.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(rejection.getReason(), CMEOrderRejectionReason::INVALID_PRICE);
    EXPECT_EQ(rejection.getMessage(), "Price must be greater than zero.");
}

TEST(CMEOrderRejectionTests, DifferentReasonsRemainDistinct)
{
    EXPECT_NE(
        CMEOrderRejectionReason::INVALID_ORDER_ID,
        CMEOrderRejectionReason::INVALID_PRICE);

    EXPECT_NE(
        CMEOrderRejectionReason::INVALID_PRICE,
        CMEOrderRejectionReason::WRONG_SYMBOL);

    EXPECT_NE(
        CMEOrderRejectionReason::WRONG_SYMBOL,
        CMEOrderRejectionReason::FILL_OR_KILL_NOT_POSSIBLE);
}
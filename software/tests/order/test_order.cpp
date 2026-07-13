#include <gtest/gtest.h>
#include "cme/order/order.hpp"

// ============================================================================
// CME ORDER OBJECT TEST SUITE
// ============================================================================

// ============================================================================
// 1. CREATE ORDER OBJECT TESTS
// ============================================================================
TEST(CMEOrderTests, ConstructorStoresAllOrderInformation)
{
    // Arrange
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25)
    );

    // Assert
    EXPECT_EQ(order.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(order.getOrderSymbol(), CMESymbol("BTC-GBP"));
    EXPECT_EQ(order.getOrderSide(), CMESide::BUY);
    EXPECT_EQ(order.getOrderPrice(), CMEPrice(50000));
    EXPECT_EQ(order.getOrderOriginalQuantity(), CMEQuantity(25));
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(25));
}

// ============================================================================
// 2. CHECK ORDER IS FILLED TEST
// ============================================================================
TEST(CMEOrderTests, NewOrderWithPositiveQuantityIsNotFilled)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25)
    );

    EXPECT_FALSE(order.isOrderFilled());
}
#include <gtest/gtest.h>
#include "cme/order/order.hpp"

// ============================================================================
// CME ORDER OBJECT TEST SUITE
// ============================================================================

// ============================================================================
// CREATE ORDER OBJECT TESTS
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
// CHECK ORDER IS FILLED TEST
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

// ============================================================================
// ORDER QUANTITY FILL TESTS
// ============================================================================
TEST(CMEOrderTests, ApplyingPartialFillReducesRemainingQuantity)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    bool fillApplied = order.applyFill(CMEQuantity(40));

    EXPECT_TRUE(fillApplied);
    EXPECT_EQ(order.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(60));
    EXPECT_FALSE(order.isOrderFilled());
}

TEST(CMEOrderTests, ApplyingFullFillReducesRemainingQuantityToZero)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    bool fillApplied = order.applyFill(CMEQuantity(100));

    EXPECT_TRUE(fillApplied);
    EXPECT_EQ(order.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(0));
    EXPECT_TRUE(order.isOrderFilled());
}

TEST(CMEOrderTests, RejectsFillLargerThanRemainingQuantity)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    bool fillApplied = order.applyFill(CMEQuantity(101));

    EXPECT_FALSE(fillApplied);
    EXPECT_EQ(order.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(100));
    EXPECT_FALSE(order.isOrderFilled());
}

TEST(CMEOrderTests, RejectsZeroFillQuantity)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    bool fillApplied = order.applyFill(CMEQuantity(0));

    EXPECT_FALSE(fillApplied);
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(100));
}
TEST(CMEOrderTests, RejectsNegativeFillQuantity)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    bool fillApplied = order.applyFill(CMEQuantity(-1));

    EXPECT_FALSE(fillApplied);
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(100));
}

// ============================================================================
// MARKET ORDER TESTS
// ============================================================================
TEST(CMEOrderTests, ConstructorStoresMarketOrderFlag)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25),
        true);

    EXPECT_TRUE(order.isMarket());
}
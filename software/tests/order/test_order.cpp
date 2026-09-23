#include <gtest/gtest.h>

#include "cme/order/order.hpp"

// ============================================================================
// CME ORDER OBJECT TEST SUITE
// ============================================================================

// ============================================================================
// ORDER CREATION TESTS
// ============================================================================

TEST(CMEOrderTests, ConstructorStoresAllOrderInformation)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_EQ(order.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(order.getOrderSymbol(), CMESymbol("BTC-GBP"));
    EXPECT_EQ(order.getOrderSide(), CMESide::BUY);
    EXPECT_EQ(order.getOrderPrice(), CMEPrice(50000));
    EXPECT_EQ(order.getOrderOriginalQuantity(), CMEQuantity(25));
    EXPECT_EQ(order.getOrderRemainingQuantity(), CMEQuantity(25));
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
        CMEQuantity(100));

    bool fillApplied =
        order.applyFill(CMEQuantity(40));

    EXPECT_TRUE(fillApplied);
    EXPECT_EQ(
        order.getOrderOriginalQuantity(),
        CMEQuantity(100));
    EXPECT_EQ(
        order.getOrderRemainingQuantity(),
        CMEQuantity(60));
    EXPECT_FALSE(order.isOrderFilled());
}

TEST(CMEOrderTests, ApplyingFullFillFillsOrder)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100));

    bool fillApplied =
        order.applyFill(CMEQuantity(100));

    EXPECT_TRUE(fillApplied);
    EXPECT_EQ(
        order.getOrderRemainingQuantity(),
        CMEQuantity(0));
    EXPECT_TRUE(order.isOrderFilled());
}

TEST(CMEOrderTests, RejectsFillLargerThanRemainingQuantity)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100));

    bool fillApplied =
        order.applyFill(CMEQuantity(101));

    EXPECT_FALSE(fillApplied);
    EXPECT_EQ(
        order.getOrderRemainingQuantity(),
        CMEQuantity(100));
}

TEST(CMEOrderTests, RejectsInvalidFillQuantity)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100));

    EXPECT_FALSE(
        order.applyFill(CMEQuantity(0)));

    EXPECT_FALSE(
        order.applyFill(CMEQuantity(-1)));

    EXPECT_EQ(
        order.getOrderRemainingQuantity(),
        CMEQuantity(100));
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
        CMEPrice(0),
        CMEQuantity(25),
        true);

    EXPECT_TRUE(order.isMarket());
}

// ============================================================================
// TIME IN FORCE TESTS
// ============================================================================

TEST(CMEOrderTests, ConstructorStoresTimeInForce)
{
    CMEOrder defaultOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder iocOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25),
        false,
        CMETimeInForce::IOC);

    CMEOrder fokOrder(
        CMEOrderId(1003),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25),
        false,
        CMETimeInForce::FOK);

    EXPECT_EQ(
        defaultOrder.getTimeInForce(),
        CMETimeInForce::GTC);

    EXPECT_EQ(
        iocOrder.getTimeInForce(),
        CMETimeInForce::IOC);

    EXPECT_EQ(
        fokOrder.getTimeInForce(),
        CMETimeInForce::FOK);
}
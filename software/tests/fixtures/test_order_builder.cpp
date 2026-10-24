#include "gtest/gtest.h"

#include "order_builder.hpp"

// ============================================================================
// CME ORDER BUILDER TEST SUITE
// ============================================================================

// ============================================================================
// DEFAULT VALUE TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithDefaultValues)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder.build();

    EXPECT_EQ(
        order.getOrderId(),
        CMEOrderId(1));

    EXPECT_EQ(
        order.getOrderSymbol(),
        CMESymbol("BTC-GBP"));

    EXPECT_EQ(
        order.getOrderSide(),
        CMESide::BUY);

    EXPECT_EQ(
        order.getOrderPrice(),
        CMEPrice(50000));

    EXPECT_EQ(
        order.getOrderOriginalQuantity(),
        CMEQuantity(10));

    EXPECT_FALSE(
        order.isMarket());

    EXPECT_EQ(
        order.getTimeInForce(),
        CMETimeInForce::GTC);
}

// ============================================================================
// ORDER ID TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithSpecifiedOrderId)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withOrderId(CMEOrderId(500))
            .build();

    EXPECT_EQ(
        order.getOrderId(),
        CMEOrderId(500));
}

// ============================================================================
// SYMBOL TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithSpecifiedSymbol)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withSymbol(CMESymbol("ETH-GBP"))
            .build();

    EXPECT_EQ(
        order.getOrderSymbol(),
        CMESymbol("ETH-GBP"));
}

// ============================================================================
// SIDE TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithSpecifiedSide)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withSide(CMESide::SELL)
            .build();

    EXPECT_EQ(
        order.getOrderSide(),
        CMESide::SELL);
}

// ============================================================================
// PRICE TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithSpecifiedPrice)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withPrice(CMEPrice(51000))
            .build();

    EXPECT_EQ(
        order.getOrderPrice(),
        CMEPrice(51000));
}

// ============================================================================
// QUANTITY TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithSpecifiedQuantity)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withQuantity(CMEQuantity(25))
            .build();

    EXPECT_EQ(
        order.getOrderOriginalQuantity(),
        CMEQuantity(25));

    EXPECT_EQ(
        order.getOrderRemainingQuantity(),
        CMEQuantity(25));
}

// ============================================================================
// MARKET ORDER TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsMarketOrder)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withMarketOrder(true)
            .build();

    EXPECT_TRUE(
        order.isMarket());
}

// ============================================================================
// TIME IN FORCE TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithSpecifiedTimeInForce)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withTimeInForce(CMETimeInForce::IOC)
            .build();

    EXPECT_EQ(
        order.getTimeInForce(),
        CMETimeInForce::IOC);
}

// ============================================================================
// CHAINED BUILDER TESTS
// ============================================================================

TEST(CMEOrderBuilderTests, BuildsOrderWithMultipleSpecifiedValues)
{
    CMEOrderBuilder builder;

    CMEOrder order =
        builder
            .withOrderId(CMEOrderId(1001))
            .withSymbol(CMESymbol("ETH-GBP"))
            .withSide(CMESide::SELL)
            .withPrice(CMEPrice(3000))
            .withQuantity(CMEQuantity(50))
            .withTimeInForce(CMETimeInForce::IOC)
            .build();

    EXPECT_EQ(
        order.getOrderId(),
        CMEOrderId(1001));

    EXPECT_EQ(
        order.getOrderSymbol(),
        CMESymbol("ETH-GBP"));

    EXPECT_EQ(
        order.getOrderSide(),
        CMESide::SELL);

    EXPECT_EQ(
        order.getOrderPrice(),
        CMEPrice(3000));

    EXPECT_EQ(
        order.getOrderOriginalQuantity(),
        CMEQuantity(50));

    EXPECT_EQ(
        order.getTimeInForce(),
        CMETimeInForce::IOC);
}
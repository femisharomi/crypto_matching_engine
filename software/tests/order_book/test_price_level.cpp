#include "gtest/gtest.h"

#include "cme/order_book/price_level.hpp"

// ============================================================================
// CME PRICE LEVEL TEST SUITE
// ============================================================================

// ============================================================================
// PRICE LEVEL CREATION TESTS
// ============================================================================

TEST(CMEPriceLevelTests, NewPriceLevelStoresItsPriceAndStartsEmpty)
{
    CMEPriceLevel levelPrice(50000);

    EXPECT_EQ(levelPrice.getPrice(), CMEPrice(50000));
    EXPECT_TRUE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 0);
}

// ============================================================================
// ADD ORDER TESTS
// ============================================================================

TEST(CMEPriceLevelTests, AddsOrderWithMatchingPrice)
{
    CMEPriceLevel levelPrice(50000);

    CMEOrder newOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(50));

    bool orderAdded = levelPrice.addOrder(newOrder);

    EXPECT_TRUE(orderAdded);
    EXPECT_FALSE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 1);
    EXPECT_EQ(
        levelPrice.getFrontOrder().getOrderId(),
        newOrder.getOrderId());
}

TEST(CMEPriceLevelTests, RejectsOrderWithDifferentPrice)
{
    CMEPriceLevel levelPrice(50000);

    CMEOrder newOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50001),
        CMEQuantity(50));

    bool orderAdded = levelPrice.addOrder(newOrder);

    EXPECT_FALSE(orderAdded);
    EXPECT_TRUE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 0);
}

// ============================================================================
// FIRST-IN FIRST-OUT TESTS
// ============================================================================

TEST(CMEPriceLevelTests, KeepsOrdersInFirstInFirstOutOrder)
{
    CMEPriceLevel levelPrice(50000);

    CMEOrder firstOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(50));

    CMEOrder secondOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(50));

    levelPrice.addOrder(firstOrder);
    levelPrice.addOrder(secondOrder);

    EXPECT_EQ(levelPrice.getOrderCount(), 2);
    EXPECT_EQ(
        levelPrice.getFrontOrder().getOrderId(),
        firstOrder.getOrderId());
}

// ============================================================================
// REMOVE ORDER TESTS
// ============================================================================

TEST(CMEPriceLevelTests, RemovesTheFrontOrder)
{
    CMEPriceLevel levelPrice(50000);

    CMEOrder firstOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(50));

    CMEOrder secondOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(50));

    levelPrice.addOrder(firstOrder);
    levelPrice.addOrder(secondOrder);

    levelPrice.removeFrontOrder();

    EXPECT_EQ(levelPrice.getOrderCount(), 1);
    EXPECT_EQ(
        levelPrice.getFrontOrder().getOrderId(),
        secondOrder.getOrderId());
}
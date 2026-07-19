#include "gtest/gtest.h"
#include "cme/order_book/price_level.hpp"

// ============================================================================
// CME PRICE LEVEL TEST SUITE
// ============================================================================

// ============================================================================
// 1. PRICE LEVEL CREATION TESTS
// ============================================================================
TEST(CMEPriceLevelTests, NewPriceLevelStoresItsPriceAndStartsEmpty)
{
    CMEPriceLevel levelPrice(50000);

    EXPECT_EQ(levelPrice.getPrice(), CMEPrice(50000));
    EXPECT_TRUE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 0);
}

// ============================================================================
// 2. ADD ORDER TESTS
// ============================================================================
TEST(CMEPriceLevelTests, AddsOrderWithMatchingPrice)
{
    CMEPriceLevel levelPrice(50000);
    CMEOrder newOrder(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    bool orderAdded = levelPrice.addOrder(newOrder);

    EXPECT_TRUE(orderAdded);
    EXPECT_FALSE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 1);
    EXPECT_EQ(levelPrice.getFrontOrder().getOrderId(), newOrder.getOrderId());
}

TEST(CMEPriceLevelTests, RejectsOrderWithDifferentPrice)
{
    CMEPriceLevel levelPrice(50000);
    CMEOrder newOrder(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50001), CMEQuantity(50));

    bool orderAdded = levelPrice.addOrder(newOrder);
    EXPECT_FALSE(orderAdded);
    EXPECT_TRUE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 0);
}

// ============================================================================
// 3. FIRST-IN FIRST-OUT TESTS
// ============================================================================
TEST(CMEPriceLevelTests, KeepsOrdersInFirstInFirstOutOrder)
{
    CMEPriceLevel levelPrice(50000);
    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    levelPrice.addOrder(newOrder1);
    levelPrice.addOrder(newOrder2);

    EXPECT_EQ(levelPrice.getOrderCount(), 2);

    EXPECT_EQ(levelPrice.getFrontOrder().getOrderId(), newOrder1.getOrderId());
}

// ============================================================================
// 4. REMOVE ORDER TESTS
// ============================================================================
TEST(CMEPriceLevelTests, RemovesTheFrontOrder)
{
    CMEPriceLevel levelPrice(50000);
    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    bool orderAdded1 = levelPrice.addOrder(newOrder1);
    bool orderAdded2 = levelPrice.addOrder(newOrder2);

    levelPrice.removeFrontOrder();

    EXPECT_EQ(levelPrice.getOrderCount(), 1);

    EXPECT_EQ(levelPrice.getFrontOrder().getOrderId(), newOrder2.getOrderId());
}

TEST(CMEPriceLevelTests, CannotRemoveOrderFromEmptyPriceLevel)
{
    CMEPriceLevel levelPrice(50000);

    EXPECT_FALSE(levelPrice.removeFrontOrder());
    EXPECT_TRUE(levelPrice.isEmpty());
    EXPECT_EQ(levelPrice.getOrderCount(), 0);
}

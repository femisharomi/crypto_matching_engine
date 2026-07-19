#include "gtest/gtest.h"
#include "cme/order_book/order_book.hpp"

// ============================================================================
// CME ORDER BOOK TEST SUITE
// ============================================================================

// ============================================================================
// 1. ORDER BOOK CREATION TESTS
// ============================================================================
TEST(CMEOrderBookTests, NewOrderBookStoresItsSymbolAndStartsEmpty)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    EXPECT_EQ(orderBook.getSymbol().value, "BTC-GBP");
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}
// ============================================================================
// 2. BUY ORDER TESTS
// ============================================================================
TEST(CMEOrderBookTests, AddsBuyOrderToBuySide)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addOrder(newOrder));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, AddsBuyOrdersAtSamePriceToOneLevel)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(25));

    EXPECT_TRUE(orderBook.addOrder(newOrder1));
    EXPECT_TRUE(orderBook.addOrder(newOrder2));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 2);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, CreatesSeparateBuyLevelsForDifferentPrices)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(49000), CMEQuantity(25));

    EXPECT_TRUE(orderBook.addOrder(newOrder1));
    EXPECT_TRUE(orderBook.addOrder(newOrder2));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 2);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(49000)).getOrderCount(), 1);
}
// ============================================================================
// 3. SELL ORDER TESTS
// ============================================================================
TEST(CMEOrderBookTests, AddsSellOrderToSellSide)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addOrder(newOrder));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);

    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(51000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(51000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, CreatesSeparateSellLevelsForDifferentPrices)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51000), CMEQuantity(25));

    EXPECT_TRUE(orderBook.addOrder(newOrder1));
    EXPECT_TRUE(orderBook.addOrder(newOrder2));

    EXPECT_EQ(orderBook.getSellLevelCount(), 2);

    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(51000)).getOrderCount(), 1);
}

// ============================================================================
// 4. ORDER BOOK SIDE SEPARATION TESTS
// ============================================================================
TEST(CMEOrderBookTests, StoresSamePriceSeparatelyOnBuyAndSellSides)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(50000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addOrder(newOrder1));
    EXPECT_TRUE(orderBook.addOrder(newOrder2));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1001);
    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1002);
}
// ============================================================================
// 5. SYMBOL REJECTION TESTS
// ============================================================================
TEST(CMEOrderBookTests, RejectsOrderForDifferentSymbol)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("ETH-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_FALSE(orderBook.addOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}
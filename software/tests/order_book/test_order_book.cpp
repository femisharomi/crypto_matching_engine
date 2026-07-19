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
    
}

TEST(CMEOrderBookTests, AddsBuyOrdersAtSamePriceToOneLevel)
{

}

TEST(CMEOrderBookTests, CreatesSeparateBuyLevelsForDifferentPrices)
{

}
// ============================================================================
// 3. SELL ORDER TESTS
// ============================================================================
TEST(CMEOrderBookTests, AddsSellOrderToSellSide)
{

}

TEST(CMEOrderBookTests, CreatesSeparateSellLevelsForDifferentPrices)
{

}

// ============================================================================
// 4. ORDER BOOK SIDE SEPARATION TESTS
// ============================================================================
TEST(CMEOrderBookTests, StoresSamePriceSeparatelyOnBuyAndSellSides)
{

}
// ============================================================================
// 5. SYMBOL REJECTION TESTS
// ============================================================================
TEST(CMEOrderBookTests, RejectsOrderForDifferentSymbol)
{

}
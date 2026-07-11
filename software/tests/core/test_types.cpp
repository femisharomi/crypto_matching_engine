#include <gtest/gtest.h>
#include "cme/core/types.hpp"

// ============================================================================
// CME CORE TYPES TEST SUITE
// ============================================================================

// ============================================================================
// 1. CME SIDE TESTS
// ============================================================================

/* Checks that the buy side is different from the sell side. */
TEST(CMECoreTypesTests, BuyIsDifferentFromSell)
{
    CMESide buy_side = CMESide::buy;
    CMESide sell_side = CMESide::sell;

    EXPECT_NE(buy_side, sell_side);
}

// ============================================================================
// 2. CME PRICE TESTS
// ============================================================================

/* Checks that two prices containing the same value are equal. */
TEST(CMECoreTypesTests, PricesWithTheSameValueAreEqual)
{
    CMEPrice first_price(15050);
    CMEPrice second_price(15050);

    EXPECT_EQ(first_price, second_price);
}

/* Checks that two prices containing different values are not equal. */
TEST(CMECoreTypesTests, PricesWithDifferentValuesAreNotEqual)
{
    CMEPrice first_price(15050);
    CMEPrice second_price(15051);

    EXPECT_NE(first_price, second_price);
}

/* Checks that a lower price is less than a higher price. */
TEST(CMECoreTypesTests, LowerPriceIsLessThanHigherPrice)
{
    CMEPrice lower_price(9999);
    CMEPrice higher_price(10000);

    EXPECT_LT(lower_price, higher_price);
}

/* Checks that a higher price is greater than a lower price. */
TEST(CMECoreTypesTests, HigherPriceIsGreaterThanLowerPrice)
{
    CMEPrice higher_price(50025);
    CMEPrice lower_price(49900);

    EXPECT_GT(higher_price, lower_price);
}

/* Checks that a lower price is less than or equal to a higher price. */
TEST(CMECoreTypesTests, LowerPriceIsLessThanOrEqualToHigherPrice)
{
    CMEPrice lower_price(9999);
    CMEPrice higher_price(10000);

    EXPECT_LE(lower_price, higher_price);
}

/* Checks that an equal price is less than or equal to another equal price. */
TEST(CMECoreTypesTests, EqualPricesAreLessThanOrEqualToEachOther)
{
    CMEPrice first_price(10000);
    CMEPrice second_price(10000);

    EXPECT_LE(first_price, second_price);
}

/* Checks that a higher price is greater than or equal to a lower price. */
TEST(CMECoreTypesTests, HigherPriceIsGreaterThanOrEqualToLowerPrice)
{
    CMEPrice higher_price(10000);
    CMEPrice lower_price(9999);

    EXPECT_GE(higher_price, lower_price);
}

/* Checks that an equal price is greater than or equal to another equal price. */
TEST(CMECoreTypesTests, EqualPricesAreGreaterThanOrEqualToEachOther)
{
    CMEPrice first_price(10000);
    CMEPrice second_price(10000);

    EXPECT_GE(first_price, second_price);
}

// ============================================================================
// 3. CME QUANTITY TESTS
// ============================================================================

/* Checks that two quantities containing the same value are equal. */
TEST(CMECoreTypesTests, QuantitiesWithTheSameValueAreEqual)
{
    CMEQuantity first_quantity(500);
    CMEQuantity second_quantity(500);

    EXPECT_EQ(first_quantity, second_quantity);
}

/* Checks that two quantities containing different values are not equal. */
TEST(CMECoreTypesTests, QuantitiesWithDifferentValuesAreNotEqual)
{
    CMEQuantity first_quantity(500);
    CMEQuantity second_quantity(501);

    EXPECT_NE(first_quantity, second_quantity);
}

/* Checks that a smaller quantity is less than a larger quantity. */
TEST(CMECoreTypesTests, SmallerQuantityIsLessThanLargerQuantity)
{
    CMEQuantity smaller_quantity(10);
    CMEQuantity larger_quantity(100);

    EXPECT_LT(smaller_quantity, larger_quantity);
}

/* Checks that a larger quantity is greater than a smaller quantity. */
TEST(CMECoreTypesTests, LargerQuantityIsGreaterThanSmallerQuantity)
{
    CMEQuantity larger_quantity(100);
    CMEQuantity smaller_quantity(10);

    EXPECT_GT(larger_quantity, smaller_quantity);
}

/* Checks that a smaller quantity is less than or equal to a larger quantity. */
TEST(CMECoreTypesTests, SmallerQuantityIsLessThanOrEqualToLargerQuantity)
{
    CMEQuantity smaller_quantity(10);
    CMEQuantity larger_quantity(100);

    EXPECT_LE(smaller_quantity, larger_quantity);
}

/* Checks that equal quantities are less than or equal to each other. */
TEST(CMECoreTypesTests, EqualQuantitiesAreLessThanOrEqualToEachOther)
{
    CMEQuantity first_quantity(100);
    CMEQuantity second_quantity(100);

    EXPECT_LE(first_quantity, second_quantity);
}

/* Checks that a larger quantity is greater than or equal to a smaller quantity. */
TEST(CMECoreTypesTests, LargerQuantityIsGreaterThanOrEqualToSmallerQuantity)
{
    CMEQuantity larger_quantity(100);
    CMEQuantity smaller_quantity(10);

    EXPECT_GE(larger_quantity, smaller_quantity);
}

/* Checks that equal quantities are greater than or equal to each other. */
TEST(CMECoreTypesTests, EqualQuantitiesAreGreaterThanOrEqualToEachOther)
{
    CMEQuantity first_quantity(100);
    CMEQuantity second_quantity(100);

    EXPECT_GE(first_quantity, second_quantity);
}

// ============================================================================
// 4. CME ORDER ID TESTS
// ============================================================================

/* Checks that two order IDs containing the same value are equal. */
TEST(CMECoreTypesTests, OrderIdsWithTheSameValueAreEqual)
{
    CMEOrderId first_order_id(987654321);
    CMEOrderId second_order_id(987654321);

    EXPECT_EQ(first_order_id, second_order_id);
}

/* Checks that two order IDs containing different values are not equal. */
TEST(CMECoreTypesTests, OrderIdsWithDifferentValuesAreNotEqual)
{
    CMEOrderId first_order_id(10001);
    CMEOrderId second_order_id(10002);

    EXPECT_NE(first_order_id, second_order_id);
}

// ============================================================================
// 5. CME SYMBOL TESTS
// ============================================================================

/* Checks that two symbols containing the same value are equal. */
TEST(CMECoreTypesTests, SymbolsWithTheSameValueAreEqual)
{
    CMESymbol first_symbol("BTC-GBP");
    CMESymbol second_symbol("BTC-GBP");

    EXPECT_EQ(first_symbol, second_symbol);
}

/* Checks that two symbols containing different values are not equal. */
TEST(CMECoreTypesTests, SymbolsWithDifferentValuesAreNotEqual)
{
    CMESymbol first_symbol("BTC-GBP");
    CMESymbol second_symbol("ETH-GBP");

    EXPECT_NE(first_symbol, second_symbol);
}
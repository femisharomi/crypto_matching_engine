#include <gtest/gtest.h>

#include "cme/core/types.hpp"

// ============================================================================
// CME CORE TYPES TEST SUITE
// ============================================================================

// ============================================================================
// CME SIDE TESTS
// ============================================================================

TEST(CMECoreTypesTests, BuyIsDifferentFromSell)
{
    CMESide buySide = CMESide::BUY;
    CMESide sellSide = CMESide::SELL;

    EXPECT_NE(buySide, sellSide);
}

// ============================================================================
// CME PRICE TESTS
// ============================================================================

TEST(CMECoreTypesTests, PricesWithTheSameValueAreEqual)
{
    CMEPrice firstPrice(15050);
    CMEPrice secondPrice(15050);

    EXPECT_EQ(firstPrice, secondPrice);
}

TEST(CMECoreTypesTests, LowerPriceIsLessThanHigherPrice)
{
    CMEPrice lowerPrice(9999);
    CMEPrice higherPrice(10000);

    EXPECT_LT(lowerPrice, higherPrice);
}

// ============================================================================
// CME QUANTITY TESTS
// ============================================================================

TEST(CMECoreTypesTests, QuantitiesWithTheSameValueAreEqual)
{
    CMEQuantity firstQuantity(500);
    CMEQuantity secondQuantity(500);

    EXPECT_EQ(firstQuantity, secondQuantity);
}

TEST(CMECoreTypesTests, SmallerQuantityIsLessThanLargerQuantity)
{
    CMEQuantity smallerQuantity(10);
    CMEQuantity largerQuantity(100);

    EXPECT_LT(smallerQuantity, largerQuantity);
}

// ============================================================================
// CME ORDER ID TESTS
// ============================================================================

TEST(CMECoreTypesTests, OrderIdsWithTheSameValueAreEqual)
{
    CMEOrderId firstOrderId(987654321);
    CMEOrderId secondOrderId(987654321);

    EXPECT_EQ(firstOrderId, secondOrderId);
}

// ============================================================================
// CME SYMBOL TESTS
// ============================================================================

TEST(CMECoreTypesTests, SymbolsWithTheSameValueAreEqual)
{
    CMESymbol firstSymbol("BTC-GBP");
    CMESymbol secondSymbol("BTC-GBP");

    EXPECT_EQ(firstSymbol, secondSymbol);
}

// ============================================================================
// TIME IN FORCE TESTS
// ============================================================================

TEST(CMECoreTypesTests, TimeInForceValuesAreDifferent)
{
    CMETimeInForce goodTillCancelled = CMETimeInForce::GTC;
    CMETimeInForce immediateOrCancel = CMETimeInForce::IOC;
    CMETimeInForce fillOrKill = CMETimeInForce::FOK;

    EXPECT_NE(goodTillCancelled, immediateOrCancel);
    EXPECT_NE(immediateOrCancel, fillOrKill);
    EXPECT_NE(goodTillCancelled, fillOrKill);
}
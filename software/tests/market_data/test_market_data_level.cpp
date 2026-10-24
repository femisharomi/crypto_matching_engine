#include "gtest/gtest.h"

#include "cme/market_data/market_data_level.hpp"

// ============================================================================
// CME MARKET DATA LEVEL TEST SUITE
// ============================================================================

// ============================================================================
// MARKET DATA LEVEL TESTS
// ============================================================================

TEST(CMEMarketDataLevelTests, StoresPriceAndAggregatedQuantity)
{
    CMEMarketDataLevel level(
        CMEPrice(50000),
        CMEQuantity(75));

    EXPECT_EQ(
        level.getPrice(),
        CMEPrice(50000));

    EXPECT_EQ(
        level.getQuantity(),
        CMEQuantity(75));
}
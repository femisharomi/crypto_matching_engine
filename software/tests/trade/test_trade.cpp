#include "gtest/gtest.h"
#include "cme/trade/trade.hpp"

// ============================================================================
// CME TRADE TEST SUITE
// ============================================================================

// ============================================================================
// TRADE MODEL TESTS
// ============================================================================
TEST(CMETradeTests, NewTradeStoresAllExecutionDetails)
{
    CMETrade trade(
        CMETradeId(5001),
        CMEOrderId(1001),
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_EQ(trade.getTradeId().value, 5001);
    EXPECT_EQ(trade.getBuyOrderId().value, 1001);
    EXPECT_EQ(trade.getSellOrderId().value, 1002);
    EXPECT_EQ(trade.getTradeSymbol().value, "BTC-GBP");
    EXPECT_EQ(trade.getTradePrice().value, 50000);
    EXPECT_EQ(trade.getTradeQuantity().value, 25);
}

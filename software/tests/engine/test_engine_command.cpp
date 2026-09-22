#include "gtest/gtest.h"

#include "cme/engine/engine_command.hpp"

// ============================================================================
// CME ENGINE COMMAND TEST SUITE
// ============================================================================

// ============================================================================
// ENGINE COMMAND TESTS
// ============================================================================

TEST(CMEEngineCommandTests, SubmitCommandStoresSubmittedOrder)
{
    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEEngineCommand command(order);

    EXPECT_EQ(
        command.getCommandType(),
        CMEEngineCommandType::SUBMIT_ORDER);

    EXPECT_EQ(command.getSymbol(), CMESymbol("BTC-GBP"));

    ASSERT_TRUE(command.getOrder().has_value());

    EXPECT_EQ(
        command.getOrder()->getOrderId(),
        CMEOrderId(1001));

    EXPECT_FALSE(command.getOrderId().has_value());
    EXPECT_FALSE(command.getNewPrice().has_value());
    EXPECT_FALSE(command.getNewQuantity().has_value());
}

TEST(CMEEngineCommandTests, CancelCommandStoresSymbolAndOrderId)
{
    CMEEngineCommand command =
        CMEEngineCommand::createCancelCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001));

    EXPECT_EQ(
        command.getCommandType(),
        CMEEngineCommandType::CANCEL_ORDER);

    EXPECT_EQ(command.getSymbol(), CMESymbol("BTC-GBP"));

    ASSERT_TRUE(command.getOrderId().has_value());

    EXPECT_EQ(
        command.getOrderId().value(),
        CMEOrderId(1001));

    EXPECT_FALSE(command.getOrder().has_value());
    EXPECT_FALSE(command.getNewPrice().has_value());
    EXPECT_FALSE(command.getNewQuantity().has_value());
}

TEST(CMEEngineCommandTests, ModifyCommandStoresReplacementValues)
{
    CMEEngineCommand command =
        CMEEngineCommand::createModifyCommand(
            CMESymbol("BTC-GBP"),
            CMEOrderId(1001),
            CMEPrice(51000),
            CMEQuantity(50));

    EXPECT_EQ(
        command.getCommandType(),
        CMEEngineCommandType::MODIFY_ORDER);

    EXPECT_EQ(command.getSymbol(), CMESymbol("BTC-GBP"));

    ASSERT_TRUE(command.getOrderId().has_value());
    ASSERT_TRUE(command.getNewPrice().has_value());
    ASSERT_TRUE(command.getNewQuantity().has_value());

    EXPECT_EQ(
        command.getOrderId().value(),
        CMEOrderId(1001));

    EXPECT_EQ(
        command.getNewPrice().value(),
        CMEPrice(51000));

    EXPECT_EQ(
        command.getNewQuantity().value(),
        CMEQuantity(50));

    EXPECT_FALSE(command.getOrder().has_value());
}
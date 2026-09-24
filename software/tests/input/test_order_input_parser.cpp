#include "gtest/gtest.h"

#include <optional>

#include "cme/input/order_input_parser.hpp"

// ============================================================================
// ORDER INPUT PARSER TESTS
// ============================================================================

TEST(CMEOrderInputParserTests, ParsesValidBuyOrder)
{
    CMEOrderInputParser parser;

    std::optional<CMEEngineCommand> command =
        parser.parse(
            "BUY BTC-GBP 50000 10",
            CMEOrderId(1));

    EXPECT_TRUE(command.has_value());
}

TEST(CMEOrderInputParserTests, ParsesValidSellOrder)
{
    CMEOrderInputParser parser;

    std::optional<CMEEngineCommand> command =
        parser.parse(
            "SELL BTC-GBP 51000 5",
            CMEOrderId(2));

    EXPECT_TRUE(command.has_value());
}

TEST(CMEOrderInputParserTests, ParsesValidCancellation)
{
    CMEOrderInputParser parser;

    std::optional<CMEEngineCommand> command =
        parser.parse(
            "CANCEL BTC-GBP 1",
            CMEOrderId(3));

    EXPECT_TRUE(command.has_value());
}

TEST(CMEOrderInputParserTests, RejectsInvalidInput)
{
    CMEOrderInputParser parser;

    std::optional<CMEEngineCommand> command =
        parser.parse(
            "INVALID BTC-GBP 50000 10",
            CMEOrderId(1));

    EXPECT_FALSE(command.has_value());
}

TEST(CMEOrderInputParserTests, RejectsIncompleteOrder)
{
    CMEOrderInputParser parser;

    std::optional<CMEEngineCommand> command =
        parser.parse(
            "BUY BTC-GBP 50000",
            CMEOrderId(1));

    EXPECT_FALSE(command.has_value());
}
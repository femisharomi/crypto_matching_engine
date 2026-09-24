#include "gtest/gtest.h"

#include <string>

#include "cme/input/file_order_reader.hpp"

// ============================================================================
// FILE ORDER READER TESTS
// ============================================================================

TEST(CMEFileOrderReaderTests, OpensExistingOrderFile)
{
    CMEFileOrderReader reader(
        "software/tests/input/test_orders.txt");

    EXPECT_TRUE(reader.isOpen());
}

TEST(CMEFileOrderReaderTests, ReadsOrdersInFileOrder)
{
    CMEFileOrderReader reader(
        "software/tests/input/test_orders.txt");

    ASSERT_TRUE(reader.isOpen());

    std::string line;

    ASSERT_TRUE(reader.readNextLine(line));
    EXPECT_EQ(line, "BUY BTC-GBP 50000 10");

    ASSERT_TRUE(reader.readNextLine(line));
    EXPECT_EQ(line, "SELL BTC-GBP 51000 5");

    ASSERT_TRUE(reader.readNextLine(line));
    EXPECT_EQ(line, "CANCEL BTC-GBP 1");

    EXPECT_FALSE(reader.readNextLine(line));
}
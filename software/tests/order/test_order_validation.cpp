#include <gtest/gtest.h>

#include "cme/order/order_validation.hpp"

// ============================================================================
// CME ORDER VALIDATION TEST SUITE
// ============================================================================

// ============================================================================
// VALID ORDER TESTS
// ============================================================================

TEST(CMEOrderValidationTests, AcceptsValidOrder)
{
    CMEOrderValidator validator;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrderValidationResult result =
        validator.validateOrder(order);

    EXPECT_EQ(
        result,
        CMEOrderValidationResult::VALID);
}

// ============================================================================
// ORDER ID VALIDATION TESTS
// ============================================================================

TEST(CMEOrderValidationTests, RejectsZeroOrderId)
{
    CMEOrderValidator validator;

    CMEOrder order(
        CMEOrderId(0),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrderValidationResult result =
        validator.validateOrder(order);

    EXPECT_EQ(
        result,
        CMEOrderValidationResult::INVALID_ORDER_ID);
}

// ============================================================================
// SYMBOL VALIDATION TESTS
// ============================================================================

TEST(CMEOrderValidationTests, RejectsEmptySymbol)
{
    CMEOrderValidator validator;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol(""),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrderValidationResult result =
        validator.validateOrder(order);

    EXPECT_EQ(
        result,
        CMEOrderValidationResult::EMPTY_SYMBOL);
}

// ============================================================================
// PRICE VALIDATION TESTS
// ============================================================================

TEST(CMEOrderValidationTests, RejectsInvalidPrice)
{
    CMEOrderValidator validator;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(0),
        CMEQuantity(25));

    CMEOrderValidationResult result =
        validator.validateOrder(order);

    EXPECT_EQ(
        result,
        CMEOrderValidationResult::INVALID_PRICE);
}

// ============================================================================
// QUANTITY VALIDATION TESTS
// ============================================================================

TEST(CMEOrderValidationTests, RejectsInvalidQuantity)
{
    CMEOrderValidator validator;

    CMEOrder order(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(0));

    CMEOrderValidationResult result =
        validator.validateOrder(order);

    EXPECT_EQ(
        result,
        CMEOrderValidationResult::INVALID_QUANTITY);
}

// ============================================================================
// VALIDATION ORDER TESTS
// ============================================================================

TEST(CMEOrderValidationTests, ReturnsFirstValidationError)
{
    CMEOrderValidator validator;

    CMEOrder order(
        CMEOrderId(0),
        CMESymbol(""),
        CMESide::BUY,
        CMEPrice(-50000),
        CMEQuantity(-25));

    CMEOrderValidationResult result =
        validator.validateOrder(order);

    EXPECT_EQ(
        result,
        CMEOrderValidationResult::INVALID_ORDER_ID);
}
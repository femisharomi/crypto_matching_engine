#include <gtest/gtest.h>
#include "cme/order/order_validation.hpp"

// ============================================================================
// CME ORDER VALIDATION TEST SUITE
// ============================================================================

// ============================================================================
// 1. VALID ORDER TESTS
// ============================================================================

// AcceptsValidOrder
TEST(CMEOrderValidationTests, AcceptsValidOrder)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(1001),
    CMESymbol("BTC-GBP"),
    CMESide::BUY,
    CMEPrice(50000),
    CMEQuantity(25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::VALID);

}

// ============================================================================
// 2. ORDER ID VALIDATION TESTS
// ============================================================================

// RejectsZeroOrderId
TEST(CMEOrderValidationTests, RejectsZeroOrderId)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(0),
    CMESymbol("BTC-GBP"),
    CMESide::BUY,
    CMEPrice(50000),
    CMEQuantity(25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::INVALID_ORDER_ID);

}

// ============================================================================
// 3. SYMBOL VALIDATION TESTS
// ============================================================================

// RejectsEmptySymbol
TEST(CMEOrderValidationTests, RejectsEmptySymbol)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(1001),
    CMESymbol(""),
    CMESide::BUY,
    CMEPrice(50000),
    CMEQuantity(25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::EMPTY_SYMBOL);

}

// ============================================================================
// 4. PRICE VALIDATION TESTS
// ============================================================================

// RejectsZeroPrice
TEST(CMEOrderValidationTests, RejectsZeroPrice)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(1001),
    CMESymbol("BTC-GBP"),
    CMESide::BUY,
    CMEPrice(0),
    CMEQuantity(25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::INVALID_PRICE);

}

// RejectsNegativePrice
TEST(CMEOrderValidationTests, RejectsNegativePrice)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(1001),
    CMESymbol("BTC-GBP"),
    CMESide::BUY,
    CMEPrice(-5),
    CMEQuantity(25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::INVALID_PRICE);

}

// ============================================================================
// 5. QUANTITY VALIDATION TESTS
// ============================================================================

// RejectsZeroQuantity
TEST(CMEOrderValidationTests, RejectsZeroQuantity)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(1001),
    CMESymbol("BTC-GBP"),
    CMESide::BUY,
    CMEPrice(50000),
    CMEQuantity(0)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::INVALID_QUANTITY);

}

// RejectsNegativeQuantity
TEST(CMEOrderValidationTests, RejectsNegativeQuantity)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(1001),
    CMESymbol("BTC-GBP"),
    CMESide::BUY,
    CMEPrice(50000),
    CMEQuantity(-25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::INVALID_QUANTITY);

}

// ============================================================================
// 6. VALIDATION ORDER TESTS
// ============================================================================

// ReturnsFirstValidationError
TEST(CMEOrderValidationTests, ReturnsFirstValidationError)
{
    CMEOrderValidator validator;

    CMEOrder order(
    CMEOrderId(0),
    CMESymbol(""),
    CMESide::BUY,
    CMEPrice(-50000),
    CMEQuantity(-25)
    );

    CMEOrderValidationResult result = validator.validateOrder(order);

    EXPECT_EQ(result, CMEOrderValidationResult::INVALID_ORDER_ID);
}
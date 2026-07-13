#ifndef CME_ORDER_ORDER_VALIDATION_HPP
#define CME_ORDER_ORDER_VALIDATION_HPP

#include "cme/order/order.hpp"

/* Represents the possible results produced when validating an order. */
enum class CMEOrderValidationResult
{
    VALID, //The order passed every validation rule.
    INVALID_ORDER_ID, //No valid order ID was supplied.
    EMPTY_SYMBOL, // The symbol contained an empty string.
    INVALID_PRICE, // The price was zero or negative.
    INVALID_QUANTITY // The quantity was zero or negative.
};

/* Checks whether an order satisfies the engine's basic validation rules. */
class CMEOrderValidator {
public:

    /* Validates an order and returns the first validation failure found. */
    CMEOrderValidationResult validateOrder(const CMEOrder& order) const;

private:
    //TBA
};
#endif //CME_ORDER_ORDER_VALIDATION_HPP
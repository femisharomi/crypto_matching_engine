#ifndef ORDER_VALIDATION_HPP
#define ORDER_VALIDATION_HPP

#include "cme/order/order.hpp"

enum class CMEOrderValidationResult
{
    VALID, //The order passed every validation rule.
    INVALID_ORDER_ID, //No valid order ID was supplied.
    EMPTY_SYMBOL, // The symbol contained an empty string.
    INVALID_PRICE, // The price was zero or negative.
    INVALID_QUANTITY // The quantity was zero or negative.
};

class CMEOrderValidator {
public:
    CMEOrderValidationResult validateOrder(const CMEOrder& order) const;

private:
    //TBA
};
#endif //ORDER_VALIDATION_HPP
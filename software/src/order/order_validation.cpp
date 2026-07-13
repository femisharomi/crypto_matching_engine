#include "cme/order/order_validation.hpp"

CMEOrderValidationResult CMEOrderValidator::validateOrder(const CMEOrder& order) const
{
    if (order.getOrderId().value == 0) 
    {
        return CMEOrderValidationResult::INVALID_ORDER_ID;
    }
    
    if (order.getOrderSymbol().value.empty()) 
    {
        return CMEOrderValidationResult::EMPTY_SYMBOL;
    }
    
    if (order.getOrderPrice().value <= 0) 
    {
        return CMEOrderValidationResult::INVALID_PRICE; 
    }
    
    if (order.getOrderOriginalQuantity().value <= 0) 
    {
        return CMEOrderValidationResult::INVALID_QUANTITY; 
    }

    return CMEOrderValidationResult::VALID; 
}

    
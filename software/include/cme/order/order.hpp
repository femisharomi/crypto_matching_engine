#ifndef CME_ORDER_ORDER_HPP
#define CME_ORDER_ORDER_HPP

#include "cme/core/types.hpp"

class CMEOrder {
public:    
    // Constructor 
    CMEOrder(
        CMEOrderId order_id,
        CMESymbol symbol,
        CMESide side,
        CMEPrice price,
        CMEQuantity quantity
    );

    CMEOrderId getOrderId() const;

    CMESymbol getOrderSymbol() const;

    CMESide getOrderSide() const;

    CMEPrice getOrderPrice() const;

    /* This gets the original quantity the order started with. */
    CMEQuantity getOrderOriginalQuantity() const; 

    /* Returns the quantity that has not yet been filled. */
    CMEQuantity getOrderRemainingQuantity() const; 

    bool isOrderFilled() const;

    bool applyFill(CMEQuantity fillQuantity); 

private:
    CMEOrderId orderId;
    CMESymbol orderSymbol;
    CMESide orderSide;
    CMEPrice orderPrice;
    CMEQuantity originalQuantity;
    CMEQuantity remainingQuantity;
};
#endif //CME_ORDER_ORDER_HPP
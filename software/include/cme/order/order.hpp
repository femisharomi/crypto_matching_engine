#ifndef CME_ORDER_ORDER_HPP
#define CME_ORDER_ORDER_HPP

#include "cme/core/types.hpp"

/**
 * @brief Constructs an immutable representation of an order.
 * 
 * @param order_id Unique exchange identifier assigned to this order.
 * @param symbol   The specific product or financial instrument token.
 * @param side     Order direction (BUY/BID or SELL/ASK).
 * @param price    The limit price represented in scaled integer format.
 * @param quantity The total contract or share volume requested.
 */
class CMEOrder {
public:    
    // Constructor 
    CMEOrder(
        CMEOrderId order_id,
        CMESymbol symbol,
        CMESide side,
        CMEPrice price,
        CMEQuantity quantity,
        bool marketOrder = false
    );

    CMEOrderId getOrderId() const;

    CMESymbol getOrderSymbol() const;

    CMESide getOrderSide() const;

    CMEPrice getOrderPrice() const;

    /* This gets the original quantity the order started with. */
    CMEQuantity getOrderOriginalQuantity() const; 

    /* Returns the quantity that has not yet been filled. */
    CMEQuantity getOrderRemainingQuantity() const; 

    /* Checks if the order has been completely executed. */
    bool isOrderFilled() const;

    /* Reduces the remaining order volume by the executed fill quantity. */
    bool applyFill(CMEQuantity fillQuantity); 

    /* Returns whether this order is a marker order. */
    bool isMarket() const;

private:
    CMEOrderId orderId;
    CMESymbol orderSymbol;
    CMESide orderSide;
    CMEPrice orderPrice;
    CMEQuantity originalQuantity;
    CMEQuantity remainingQuantity;
    
    // Indicates whether this order is a market order.
    bool isMarketOrder;
};
#endif //CME_ORDER_ORDER_HPP
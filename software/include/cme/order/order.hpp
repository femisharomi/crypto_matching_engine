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
    /* Constructs a new order with the specified details. */
    CMEOrder(
        CMEOrderId order_id,
        CMESymbol symbol,
        CMESide side,
        CMEPrice price,
        CMEQuantity quantity,
        bool marketOrder = false,
        CMETimeInForce timeInForce = CMETimeInForce::GTC
    );

    /* Returns the unique identifier of the order. */
    CMEOrderId getOrderId() const;

    /* Returns the trading symbol for the order. */
    CMESymbol getOrderSymbol() const;

    /* Returns whether the order is a BUY or SELL order. */
    CMESide getOrderSide() const;

    //* Returns the limit price of the order. */
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

    /* Returns the Time In Force assigned to this order. */
    CMETimeInForce getTimeInForce() const;


private:
    // Unique identifier assigned to this order.
    CMEOrderId orderId;

    // Trading instrument that this order belongs to (e.g. BTC-GBP).
    CMESymbol orderSymbol;

    // Indicates whether the order is a BUY or SELL order.
    CMESide orderSide;

    // The limit price of the order. Ignored for market orders.
    CMEPrice orderPrice;

    // Total quantity requested when the order was created.
    CMEQuantity originalQuantity;

    // Quantity that has not yet been filled.
    CMEQuantity remainingQuantity;

    // Indicates whether this order is a market order.
    bool isMarketOrder;

    // The Time In Force assigned to this order.
    CMETimeInForce orderTimeInForce;
};
#endif //CME_ORDER_ORDER_HPP
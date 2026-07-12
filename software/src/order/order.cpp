#include "cme/order/order.hpp"

CMEOrder::CMEOrder(
    CMEOrderId order_id,
    CMESymbol symbol,
    CMESide side,
    CMEPrice price,
    CMEQuantity quantity
) : orderId(order_id), orderSymbol(symbol), orderSide(side), orderPrice(price), originalQuantity(quantity), remainingQuantity(quantity){}

CMEOrderId CMEOrder::getOrderId() const
{
    return orderId;
}

CMESymbol CMEOrder::getOrderSymbol() const
{
    return orderSymbol;
}

CMESide CMEOrder::getOrderSide() const
{
    return orderSide;
}

CMEPrice CMEOrder::getOrderPrice() const
{
    return orderPrice;
}

/* This gets the original quantity the order started with. */
CMEQuantity CMEOrder::getOrderOriginalQuantity() const
{
    return originalQuantity;
}

/* Returns the quantity that has not yet been filled. */
CMEQuantity CMEOrder::getOrderRemainingQuantity() const
{
    return remainingQuantity;
}

bool CMEOrder::isOrderFilled() const
{
    return remainingQuantity.value == 0;
}
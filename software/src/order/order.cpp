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

/* Returns if the order has been filled. */
bool CMEOrder::isOrderFilled() const
{
    return remainingQuantity.value == 0;
}

/* Reduces the remaining quantity when a valid fill quantity is supplied. */
bool CMEOrder::applyFill(CMEQuantity fillQuantity)
{
    if(fillQuantity.value <= 0 || fillQuantity.value > remainingQuantity.value) return false;

    remainingQuantity.value -= fillQuantity.value;

    return true;
}
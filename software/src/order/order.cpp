#include "cme/order/order.hpp"

CMEOrder::CMEOrder(
    CMEOrderId order_id,
    CMESymbol symbol,
    CMESide side,
    CMEPrice price,
    CMEQuantity quantity,
    bool marketOrder
) : orderId(order_id), 
    orderSymbol(symbol), 
    orderSide(side), 
    orderPrice(price), 
    originalQuantity(quantity), 
    remainingQuantity(quantity),
    isMarketOrder(marketOrder){}

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

CMEQuantity CMEOrder::getOrderOriginalQuantity() const
{
    return originalQuantity;
}

CMEQuantity CMEOrder::getOrderRemainingQuantity() const
{
    return remainingQuantity;
}

bool CMEOrder::isOrderFilled() const
{
    return remainingQuantity.value == 0;
}

bool CMEOrder::applyFill(CMEQuantity fillQuantity)
{
    if(fillQuantity.value <= 0 || fillQuantity.value > remainingQuantity.value) return false;

    remainingQuantity.value -= fillQuantity.value;

    return true;
}

bool CMEOrder::isMarket() const
{
    return isMarketOrder;
}
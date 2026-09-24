#include "order_builder.hpp"

CMEOrderBuilder::CMEOrderBuilder()
    : orderId(CMEOrderId(1)),
      symbol(CMESymbol("BTC-GBP")),
      side(CMESide::BUY),
      price(CMEPrice(50000)),
      quantity(CMEQuantity(10)),
      marketOrder(false),
      timeInForce(CMETimeInForce::GTC)
{
}

CMEOrderBuilder& CMEOrderBuilder::withOrderId(CMEOrderId newOrderId)
{
    orderId = newOrderId;
    return *this;
}

CMEOrderBuilder& CMEOrderBuilder::withSymbol(CMESymbol newSymbol)
{
    symbol = newSymbol;
    return *this;
}

CMEOrderBuilder& CMEOrderBuilder::withSide(CMESide newSide)
{
    side = newSide;
    return *this;
}

CMEOrderBuilder& CMEOrderBuilder::withPrice(CMEPrice newPrice)
{
    price = newPrice;
    return *this;
}

CMEOrderBuilder& CMEOrderBuilder::withQuantity(CMEQuantity newQuantity)
{
    quantity = newQuantity;
    return *this;
}

CMEOrderBuilder& CMEOrderBuilder::withMarketOrder(bool isMarketOrder)
{
    marketOrder = isMarketOrder;
    return *this;
}

CMEOrderBuilder& CMEOrderBuilder::withTimeInForce(
    CMETimeInForce newTimeInForce)
{
    timeInForce = newTimeInForce;
    return *this;
}

CMEOrder CMEOrderBuilder::build() const
{
    return CMEOrder(
        orderId,
        symbol,
        side,
        price,
        quantity,
        marketOrder,
        timeInForce);
}
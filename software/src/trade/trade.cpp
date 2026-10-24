#include "cme/trade/trade.hpp"

CMETrade::CMETrade(CMETradeId tradeIdValue, 
                   CMEOrderId buyOrderIdValue, 
                   CMEOrderId sellOrderIdValue, 
                   CMESymbol tradeSymbolValue, 
                   CMEPrice tradePriceValue, 
                   CMEQuantity tradeQuantityValue) : tradeId(tradeIdValue), buyOrderId(buyOrderIdValue), sellOrderId(sellOrderIdValue),
                                                     tradeSymbol(tradeSymbolValue), tradePrice(tradePriceValue), tradeQuantity(tradeQuantityValue)
{

}

CMETradeId CMETrade::getTradeId() const
{
    return tradeId;
}

CMEOrderId CMETrade::getBuyOrderId() const
{
    return buyOrderId;
}

CMEOrderId CMETrade::getSellOrderId() const
{
    return sellOrderId;
}

CMESymbol CMETrade::getTradeSymbol() const
{
    return tradeSymbol;
}

CMEPrice CMETrade::getTradePrice() const
{
    return tradePrice;
}

CMEQuantity CMETrade::getTradeQuantity() const
{
    return tradeQuantity;
}
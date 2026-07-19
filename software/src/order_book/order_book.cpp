#include "cme/order_book/order_book.hpp"

CMEOrderBook::CMEOrderBook(CMESymbol symbol) : bookSymbol(symbol)
{
}

CMESymbol CMEOrderBook::getSymbol() const
{
    return bookSymbol;
}

bool CMEOrderBook::addOrder(const CMEOrder &order)
{
    if(order.getOrderSymbol() != bookSymbol) return false;

    CMEPrice priceObj = order.getOrderPrice();

    switch(order.getOrderSide())
    {
        case CMESide::BUY:
            // Inserts a new CMEPriceLevel(priceObj) ONLY if the key priceObj.value doesn't exist yet
            buyLevels.try_emplace(priceObj.value, priceObj);
            return buyLevels.at(priceObj.value).addOrder(order);
            
        case CMESide::SELL:
            // Inserts a new CMEPriceLevel(priceObj) ONLY if the key priceObj.value doesn't exist yet
            sellLevels.try_emplace(priceObj.value, priceObj);
            return sellLevels.at(priceObj.value).addOrder(order);
            
        default: 
            throw std::runtime_error("Unknown market side encountered!");
    }

    return false; 
}

std::size_t CMEOrderBook::getBuyLevelCount() const
{
    return buyLevels.size();
}

std::size_t CMEOrderBook::getSellLevelCount() const
{
    return sellLevels.size();
}

const CMEPriceLevel& CMEOrderBook::getBuyLevel(CMEPrice price) const
{
    return buyLevels.at(price.value);
}

const CMEPriceLevel& CMEOrderBook::getSellLevel(CMEPrice price) const
{
    return sellLevels.at(price.value);
}
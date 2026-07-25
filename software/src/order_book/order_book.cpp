#include "cme/order_book/order_book.hpp"

CMEOrderBook::CMEOrderBook(CMESymbol symbol) : bookSymbol(symbol)
{
}

CMESymbol CMEOrderBook::getSymbol() const
{
    return bookSymbol;
}

bool CMEOrderBook::addLimitOrder(const CMEOrder &order)
{
    // Check to ensure the order is valid before processing it into the Limit Order book.
    CMEOrderValidationResult isValid = orderValidator.validateOrder(order);
    if(isValid == CMEOrderValidationResult::VALID)
    {
        if(order.getOrderSymbol() == bookSymbol)
        {
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
                    throw std::runtime_error("Function: CMEOrderBook::addLimitOrder() - Unknown market side encountered!");
            }
        }
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

bool CMEOrderBook::hasBuyLevels() const
{
    return !buyLevels.empty();
}

bool CMEOrderBook::hasSellLevels() const
{
    return !sellLevels.empty();
}

CMEPrice CMEOrderBook::getBestBid() const
{
    // 1. Check whether any buy levels exist and throw if empty
    if(buyLevels.empty()) throw std::runtime_error("Function: CMEOrderBook::getBestBid() - No Buy levels exist!");
    
    // 2. Extract the final element using rbegin()
    std::map<std::int64_t,CMEPriceLevel>::const_reverse_iterator bestBid = buyLevels.rbegin();
    
    // 3. Return CMEPrice object 
    return bestBid->second.getPrice();
}

CMEPrice CMEOrderBook::getBestAsk() const
{
    // 1. Check whether any sell levels exist and throw if empty
    if(sellLevels.empty()) throw std::runtime_error("Function: CMEOrderBook::getBestAsk() - No Sell levels exist!");
    
    // 2. Extract the final element 
    std::map<std::int64_t,CMEPriceLevel>::const_iterator bestAsk = sellLevels.begin();
    
    // 3. Return CMEPrice object 
    return bestAsk->second.getPrice();
}
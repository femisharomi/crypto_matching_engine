#include "cme/order_book/order_book.hpp"
#include <stdexcept>

CMEOrderBook::CMEOrderBook(CMESymbol symbol) : bookSymbol(symbol)
{
    nextTradeId = 1;
}

CMESymbol CMEOrderBook::getSymbol() const
{
    return bookSymbol;
}

bool CMEOrderBook::addLimitOrder(CMEOrder incomingOrder)
{
    CMEOrderValidationResult validationResult =
        orderValidator.validateOrder(incomingOrder);

    if (validationResult != CMEOrderValidationResult::VALID)
    {
        return false;
    }

    if (incomingOrder.getOrderSymbol() != bookSymbol)
    {
        return false;
    }

    if (tryMatchOrder(incomingOrder))
    {
        return true;
    }

    CMEPrice orderPrice = incomingOrder.getOrderPrice();

    switch (incomingOrder.getOrderSide())
    {
        case CMESide::BUY:
            buyLevels.try_emplace(orderPrice.value, orderPrice);
            return buyLevels.at(orderPrice.value).addOrder(incomingOrder);

        case CMESide::SELL:
            sellLevels.try_emplace(orderPrice.value, orderPrice);
            return sellLevels.at(orderPrice.value).addOrder(incomingOrder);

        default:
            throw std::runtime_error(
                "Function: CMEOrderBook::addLimitOrder() - Unknown market side encountered!");
    }
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

bool CMEOrderBook::canMatch(const CMEOrder &incomingOrder) const
{
    switch(incomingOrder.getOrderSide())
    {
        case CMESide::BUY:
            if(!hasSellLevels()) return false;

            return incomingOrder.getOrderPrice() >= getBestAsk();

        case CMESide::SELL:
            if(!hasBuyLevels()) return false;

           return incomingOrder.getOrderPrice() <= getBestBid(); 
           
        default:
            throw std::runtime_error("Function: CMEOrderBook::canMatch() - Unknown market side encountered!");
    }
}

bool CMEOrderBook::tryMatchOrder(CMEOrder& incomingOrder)
{
    // Return immediately if prices do not cross.
    if (!canMatch(incomingOrder))
    {
        return false;
    }

    switch (incomingOrder.getOrderSide())
    {
        case CMESide::BUY:
        {
            CMEPrice bestAsk = getBestAsk();
            CMEPriceLevel& sellLevel = sellLevels.at(bestAsk.value);
            CMEOrder& restingOrder = sellLevel.getFrontOrder();

            CMEQuantity incomingRemaining = incomingOrder.getOrderRemainingQuantity();
            CMEQuantity restingRemaining = restingOrder.getOrderRemainingQuantity();

            if (incomingRemaining == restingRemaining)
            {
                sellLevel.removeFrontOrder();
                createTrade(incomingOrder, restingOrder, restingRemaining);
                removeEmptyLevel(CMESide::SELL, bestAsk);
                return true;
            }

            if (incomingRemaining < restingRemaining)
            {
                if (!restingOrder.applyFill(incomingRemaining))
                {
                    throw std::runtime_error(
                        "Function: CMEOrderBook::tryMatchOrder() - Failed to apply fill to resting order!");
                }
                createTrade(incomingOrder, restingOrder, incomingRemaining);
                return true;
            }

            if (!incomingOrder.applyFill(restingRemaining))
            {
                throw std::runtime_error(
                    "Function: CMEOrderBook::tryMatchOrder() - Failed to apply fill to incoming order!");
            }

            sellLevel.removeFrontOrder();
            createTrade(incomingOrder, restingOrder, restingRemaining);
            removeEmptyLevel(CMESide::SELL, bestAsk);
            return false;
        }

        case CMESide::SELL:
        {
            CMEPrice bestBid = getBestBid();
            CMEPriceLevel& buyLevel = buyLevels.at(bestBid.value);
            CMEOrder& restingOrder = buyLevel.getFrontOrder();

            CMEQuantity incomingRemaining = incomingOrder.getOrderRemainingQuantity();
            CMEQuantity restingRemaining = restingOrder.getOrderRemainingQuantity();

            if (incomingRemaining == restingRemaining)
            {
                buyLevel.removeFrontOrder();
                createTrade(incomingOrder, restingOrder, restingRemaining);
                removeEmptyLevel(CMESide::BUY, bestBid);
                return true;
            }

            if (incomingRemaining < restingRemaining)
            {
                if (!restingOrder.applyFill(incomingRemaining))
                {
                    throw std::runtime_error(
                        "Function: CMEOrderBook::tryMatchOrder() - Failed to apply fill to resting order!");
                }
                createTrade(incomingOrder, restingOrder, incomingRemaining);
                return true;
            }

            if (!incomingOrder.applyFill(restingRemaining))
            {
                throw std::runtime_error(
                    "Function: CMEOrderBook::tryMatchOrder() - Failed to apply fill to incoming order!");
            }

            buyLevel.removeFrontOrder();
            createTrade(incomingOrder, restingOrder, restingRemaining);
            removeEmptyLevel(CMESide::BUY, bestBid);
            return false;
        }

        default:
            throw std::runtime_error(
                "Function: CMEOrderBook::tryMatchOrder() - Unknown market side encountered!");
    }
}

void CMEOrderBook::removeEmptyLevel(CMESide side, CMEPrice levelPrice)
{
    switch (side)
    {
        case CMESide::BUY:
        {
            std::map<std::int64_t, CMEPriceLevel>::iterator levelIterator = buyLevels.find(levelPrice.value);

            if (levelIterator != buyLevels.end() &&
                levelIterator->second.isEmpty())
            {
                buyLevels.erase(levelIterator);
            }

            break;
        }

        case CMESide::SELL:
        {
            std::map<std::int64_t, CMEPriceLevel>::iterator levelIterator = sellLevels.find(levelPrice.value);

            if (levelIterator != sellLevels.end() &&
                levelIterator->second.isEmpty())
            {
                sellLevels.erase(levelIterator);
            }

            break;
        }
        
        default:
            throw std::runtime_error(
                "Function: CMEOrderBook::removeEmptyLevel() - Unknown market side encountered!");
    }
}

const std::optional<CMETrade>& CMEOrderBook::getLastTrade() const
{
    return lastTrade;
}

void CMEOrderBook::createTrade(const CMEOrder& incomingOrder, const CMEOrder& restingOrder, CMEQuantity tradeQuantity)
{
    if(incomingOrder.getOrderSide() == CMESide::BUY)
    {
        // The Incoming order is BUY
        lastTrade = CMETrade(nextTradeId, 
                            incomingOrder.getOrderId(), 
                            restingOrder.getOrderId(), 
                            restingOrder.getOrderSymbol(), 
                            restingOrder.getOrderPrice(), 
                            tradeQuantity);
    }
    else
    {
        // The Incoming order is SELL
        lastTrade = CMETrade(nextTradeId, 
                            restingOrder.getOrderId(), 
                            incomingOrder.getOrderId(), 
                            restingOrder.getOrderSymbol(), 
                            restingOrder.getOrderPrice(), 
                            tradeQuantity);
    }

    nextTradeId++;
}
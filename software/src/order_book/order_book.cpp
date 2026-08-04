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

bool CMEOrderBook::addOrder(CMEOrder incomingOrder)
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

    if (incomingOrder.getTimeInForce() == CMETimeInForce::FOK)
    {
        if (!canFullyMatch(incomingOrder))
            return false;
    }

    // Attempt to match the incoming order.
    if (tryMatchOrder(incomingOrder))
    {
        // The order was completely filled.
        return true;
    }

    // Market orders never rest in the order book.
    if (incomingOrder.isMarket())
    {
        // Distinguish market IOC from market GTC.
        if (incomingOrder.getTimeInForce() == CMETimeInForce::IOC)
        {
            return true;
        }
        return false;
    }

    // Discard any unfilled quantity from an IOC order.
    if (incomingOrder.getTimeInForce() == CMETimeInForce::IOC)
    {
        return true;
    }

    // Unfilled GTC limit orders rest in the order book.
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
            "Function: CMEOrderBook::addOrder() - Unknown market side encountered!");
    }
}

bool CMEOrderBook::canMatch(const CMEOrder &incomingOrder) const
{
    switch (incomingOrder.getOrderSide())
    {
    case CMESide::BUY:
        if (!hasSellLevels())
            return false;
        if (incomingOrder.isMarket())
            return true;

        return incomingOrder.getOrderPrice() >= getBestAsk();

    case CMESide::SELL:
        if (!hasBuyLevels())
            return false;
        if (incomingOrder.isMarket())
            return true;

        return incomingOrder.getOrderPrice() <= getBestBid();

    default:
        throw std::runtime_error("Function: CMEOrderBook::canMatch() - Unknown market side encountered!");
    }
}

bool CMEOrderBook::canFullyMatch(const CMEOrder &incomingOrder) const
{
    CMEQuantity remainingQuantity = incomingOrder.getOrderRemainingQuantity();

    switch (incomingOrder.getOrderSide())
    {
    case CMESide::BUY:
    {
        if (!hasSellLevels())
        {
            return false;
        }

        for (std::map<std::int64_t, CMEPriceLevel>::const_iterator it = sellLevels.begin();
             it != sellLevels.end();
             ++it)
        {
            // Limit orders cannot match above their limit price.
            if (!incomingOrder.isMarket() &&
                it->second.getPrice() > incomingOrder.getOrderPrice())
            {
                break;
            }

            remainingQuantity.value -=
                it->second.getTotalRemainingQuantity().value;

            if (remainingQuantity.value <= 0)
            {
                return true;
            }
        }

        return false;
    }

    case CMESide::SELL:
    {
        if (!hasBuyLevels())
        {
            return false;
        }

        for (std::map<std::int64_t, CMEPriceLevel>::const_reverse_iterator it = buyLevels.rbegin();
             it != buyLevels.rend();
             ++it)
        {
            // Limit orders cannot match below their limit price.
            if (!incomingOrder.isMarket() &&
                it->second.getPrice() < incomingOrder.getOrderPrice())
            {
                break;
            }

            remainingQuantity.value -=
                it->second.getTotalRemainingQuantity().value;

            if (remainingQuantity.value <= 0)
            {
                return true;
            }
        }

        return false;
    }

    default:
        throw std::runtime_error(
            "Function: CMEOrderBook::canFullyMatch() - Unknown market side encountered!");
    }
}

bool CMEOrderBook::tryMatchOrder(CMEOrder &incomingOrder)
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
        while (canMatch(incomingOrder))
        {
            CMEPrice bestAsk = getBestAsk();
            CMEPriceLevel &sellLevel = sellLevels.at(bestAsk.value);
            CMEOrder &restingOrder = sellLevel.getFrontOrder();

            CMEQuantity incomingRemaining =
                incomingOrder.getOrderRemainingQuantity();

            CMEQuantity restingRemaining =
                restingOrder.getOrderRemainingQuantity();

            if (incomingRemaining == restingRemaining)
            {
                createTrade(incomingOrder, restingOrder, restingRemaining);
                sellLevel.removeFrontOrder();
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

            createTrade(incomingOrder, restingOrder, restingRemaining);

            sellLevel.removeFrontOrder();
            removeEmptyLevel(CMESide::SELL, bestAsk);
        }

        return false;
    }
    case CMESide::SELL:
    {
        while (canMatch(incomingOrder))
        {
            CMEPrice bestBid = getBestBid();
            CMEPriceLevel &buyLevel = buyLevels.at(bestBid.value);
            CMEOrder &restingOrder = buyLevel.getFrontOrder();

            CMEQuantity incomingRemaining =
                incomingOrder.getOrderRemainingQuantity();

            CMEQuantity restingRemaining =
                restingOrder.getOrderRemainingQuantity();

            if (incomingRemaining == restingRemaining)
            {
                createTrade(incomingOrder, restingOrder, restingRemaining);
                buyLevel.removeFrontOrder();
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

            createTrade(incomingOrder, restingOrder, restingRemaining);

            buyLevel.removeFrontOrder();
            removeEmptyLevel(CMESide::BUY, bestBid);
        }

        return false;
    }

    default:
        throw std::runtime_error("Function: CMEOrderBook::tryMatchOrder() - Unknown market side encountered!");
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

const CMEPriceLevel &CMEOrderBook::getBuyLevel(CMEPrice price) const
{
    return buyLevels.at(price.value);
}

const CMEPriceLevel &CMEOrderBook::getSellLevel(CMEPrice price) const
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
    if (buyLevels.empty())
        throw std::runtime_error("Function: CMEOrderBook::getBestBid() - No Buy levels exist!");

    // 2. Extract the final element using rbegin()
    std::map<std::int64_t, CMEPriceLevel>::const_reverse_iterator bestBid = buyLevels.rbegin();

    // 3. Return CMEPrice object
    return bestBid->second.getPrice();
}

CMEPrice CMEOrderBook::getBestAsk() const
{
    // 1. Check whether any sell levels exist and throw if empty
    if (sellLevels.empty())
        throw std::runtime_error("Function: CMEOrderBook::getBestAsk() - No Sell levels exist!");

    // 2. Extract the final element
    std::map<std::int64_t, CMEPriceLevel>::const_iterator bestAsk = sellLevels.begin();

    // 3. Return CMEPrice object
    return bestAsk->second.getPrice();
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
        throw std::runtime_error("Function: CMEOrderBook::removeEmptyLevel() - Unknown market side encountered!");
    }
}

const std::optional<CMETrade> &CMEOrderBook::getLastTrade() const
{
    return lastTrade;
}

void CMEOrderBook::createTrade(const CMEOrder &incomingOrder, const CMEOrder &restingOrder, CMEQuantity tradeQuantity)
{
    if (incomingOrder.getOrderSide() == CMESide::BUY)
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

bool CMEOrderBook::cancelOrder(CMEOrderId orderId)
{
    // 1. Search and remove from buy side
    for (std::map<std::int64_t, CMEPriceLevel>::iterator it = buyLevels.begin(); it != buyLevels.end();)
    {
        if (it->second.removeOrder(orderId))
        {
            if (it->second.isEmpty())
            {
                it = buyLevels.erase(it);
            }
            return true;
        }
        ++it;
    }

    // 2. Search and remove from sell side
    for (std::map<std::int64_t, CMEPriceLevel>::iterator it = sellLevels.begin(); it != sellLevels.end();)
    {
        if (it->second.removeOrder(orderId))
        {
            if (it->second.isEmpty())
            {
                it = sellLevels.erase(it);
            }
            return true;
        }
        ++it;
    }

    // 3. Order not found in any level
    return false;
}

std::optional<CMEOrder> CMEOrderBook::findOrder(CMEOrderId orderId) const
{
    // Check every buy price level.
    for (std::map<std::int64_t, CMEPriceLevel>::const_iterator it = buyLevels.begin();
         it != buyLevels.end();
         ++it)
    {
        if (it->second.containsOrder(orderId))
        {
            return it->second.getOrder(orderId);
        }
    }

    // Check every sell price level.
    for (std::map<std::int64_t, CMEPriceLevel>::const_iterator it = sellLevels.begin();
         it != sellLevels.end();
         ++it)
    {
        if (it->second.containsOrder(orderId))
        {
            return it->second.getOrder(orderId);
        }
    }

    return std::nullopt;
}

bool CMEOrderBook::modifyOrder(CMEOrderId orderId, CMEPrice newPrice, CMEQuantity newQuantity)
{
    std::optional<CMEOrder> foundOrder = findOrder(orderId);

    if (!foundOrder.has_value())
    {
        return false;
    }

    CMEOrder replacementOrder(foundOrder->getOrderId(),
                              foundOrder->getOrderSymbol(),
                              foundOrder->getOrderSide(),
                              newPrice,
                              newQuantity);

    if (orderValidator.validateOrder(replacementOrder) != CMEOrderValidationResult::VALID)
    {
        return false;
    }

    if (!cancelOrder(orderId))
        return false;

    return addOrder(replacementOrder);
}

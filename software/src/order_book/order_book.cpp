#include "cme/order_book/order_book.hpp"
#include <stdexcept>

CMEOrderBook::CMEOrderBook(
    CMESymbol symbol,
    CMETradePublisher* publisher,
    CMEClock* bookclock)
    : bookSymbol(symbol),
      tradePublisher(publisher),
      clock(bookclock),
      nextTradeId(1),
      sequenceNumber(0)
{

}

CMESymbol CMEOrderBook::getSymbol() const
{
    return bookSymbol;
}

bool CMEOrderBook::addOrder(CMEOrder incomingOrder)
{
    bool isMarketOrder = incomingOrder.isMarket();
    CMETimeInForce timeInForce = incomingOrder.getTimeInForce();

    CMEMatchingResult result = processOrder(incomingOrder);

    if (result.getStatus() == CMEMatchingStatus::REJECTED)
    {
        return false;
    }

    // Preserve the existing behaviour of ordinary market orders:
    // if any quantity remains and is cancelled, addOrder returns false.
    if (result.getStatus() == CMEMatchingStatus::CANCELLED &&
        isMarketOrder &&
        timeInForce != CMETimeInForce::IOC)
    {
        return false;
    }

    return true;
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

std::uint64_t CMEOrderBook::getSequenceNumber() const
{
    return sequenceNumber;
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

    if(tradePublisher != nullptr)
    {
        tradePublisher->publishTrade(lastTrade.value());
    }

    nextTradeId++;
}

bool CMEOrderBook::removeOrderWithoutSequenceUpdate(CMEOrderId orderId)
{
    // Search and remove from buy side.
    for (std::map<std::int64_t, CMEPriceLevel>::iterator it = buyLevels.begin();
         it != buyLevels.end();
         ++it)
    {
        if (it->second.removeOrder(orderId))
        {
            if (it->second.isEmpty())
            {
                buyLevels.erase(it);
            }

            return true;
        }
    }

    // Search and remove from sell side.
    for (std::map<std::int64_t, CMEPriceLevel>::iterator it = sellLevels.begin();
         it != sellLevels.end();
         ++it)
    {
        if (it->second.removeOrder(orderId))
        {
            if (it->second.isEmpty())
            {
                sellLevels.erase(it);
            }

            return true;
        }
    }

    return false;
}

bool CMEOrderBook::cancelOrder(CMEOrderId orderId)
{
    if (!removeOrderWithoutSequenceUpdate(orderId))
    {
        return false;
    }

    sequenceNumber++;

    return true;
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

bool CMEOrderBook::modifyOrder(
    CMEOrderId orderId,
    CMEPrice newPrice,
    CMEQuantity newQuantity)
{
    std::optional<CMEOrder> foundOrder = findOrder(orderId);

    if (!foundOrder.has_value())
    {
        return false;
    }

    CMEOrder replacementOrder(
        foundOrder->getOrderId(),
        foundOrder->getOrderSymbol(),
        foundOrder->getOrderSide(),
        newPrice,
        newQuantity);

    if (orderValidator.validateOrder(replacementOrder) !=
        CMEOrderValidationResult::VALID)
    {
        return false;
    }

    if (!removeOrderWithoutSequenceUpdate(orderId))
    {
        return false;
    }

    return addOrder(replacementOrder);
}

CMEMatchingResult CMEOrderBook::processOrder(CMEOrder incomingOrder)
{
    std::uint64_t startingTradeId = nextTradeId;

    CMEOrderValidationResult validationResult =
        orderValidator.validateOrder(incomingOrder);

    // Reject invalid orders.
    if (validationResult != CMEOrderValidationResult::VALID)
    {
        CMEOrderRejection rejection =
            createValidationRejection(incomingOrder, validationResult);

        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::REJECTED,
            std::nullopt,
            rejection);
    }

    // Reject orders intended for a different order book.
    if (incomingOrder.getOrderSymbol() != bookSymbol)
    {
        CMEOrderRejection rejection(
            incomingOrder.getOrderId(),
            CMEOrderRejectionReason::WRONG_SYMBOL,
            "Order symbol does not match this order book.");

        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::REJECTED,
            std::nullopt,
            rejection);
    }

    // FOK orders must be completely matchable before any matching occurs.
    if (incomingOrder.getTimeInForce() == CMETimeInForce::FOK)
    {
        if (!canFullyMatch(incomingOrder))
        {
            CMEOrderRejection rejection(
                incomingOrder.getOrderId(),
                CMEOrderRejectionReason::FILL_OR_KILL_NOT_POSSIBLE,
                "Fill or kill order cannot be completely matched.");

            return CMEMatchingResult(
                incomingOrder.getOrderId(),
                CMEMatchingStatus::REJECTED,
                std::nullopt,
                rejection);
        }
    }

    // Attempt to match the order.
    bool completelyFilled = tryMatchOrder(incomingOrder);

    // Check whether this specific order generated at least one trade.
    bool tradeGenerated = nextTradeId > startingTradeId;

    if (completelyFilled)
    {
        if (tradeGenerated)
        {
            return CMEMatchingResult(
                incomingOrder.getOrderId(),
                CMEMatchingStatus::FILLED,
                lastTrade,
                std::nullopt);
        }

        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::FILLED,
            std::nullopt,
            std::nullopt);
    }

    // Market orders never rest in the order book.
    if (incomingOrder.isMarket())
    {
        if (tradeGenerated)
        {
            return CMEMatchingResult(
                incomingOrder.getOrderId(),
                CMEMatchingStatus::CANCELLED,
                lastTrade,
                std::nullopt);
        }

        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::CANCELLED,
            std::nullopt,
            std::nullopt);
    }

    // IOC orders cancel any quantity that remains after matching.
    if (incomingOrder.getTimeInForce() == CMETimeInForce::IOC)
    {
        if (tradeGenerated)
        {
            return CMEMatchingResult(
                incomingOrder.getOrderId(),
                CMEMatchingStatus::CANCELLED,
                lastTrade,
                std::nullopt);
        }

        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::CANCELLED,
            std::nullopt,
            std::nullopt);
    }

    // Any remaining GTC limit quantity rests in the order book.
    CMEPrice orderPrice = incomingOrder.getOrderPrice();
    bool orderAdded = false;

    switch (incomingOrder.getOrderSide())
    {
        case CMESide::BUY:
            buyLevels.try_emplace(orderPrice.value, orderPrice);
            orderAdded =
                buyLevels.at(orderPrice.value).addOrder(incomingOrder);
            break;

        case CMESide::SELL:
            sellLevels.try_emplace(orderPrice.value, orderPrice);
            orderAdded =
                sellLevels.at(orderPrice.value).addOrder(incomingOrder);
            break;

        default:
            throw std::runtime_error(
                "Function: CMEOrderBook::processOrder() - Unknown market side encountered!");
    }

    if (!orderAdded)
    {
        CMEOrderRejection rejection(
            incomingOrder.getOrderId(),
            CMEOrderRejectionReason::UNKNOWN,
            "Order could not be added to the order book.");

        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::REJECTED,
            std::nullopt,
            rejection);
    }

    sequenceNumber++;

    // The order traded but still has remaining quantity resting.
    if (tradeGenerated)
    {
        return CMEMatchingResult(
            incomingOrder.getOrderId(),
            CMEMatchingStatus::PARTIALLY_FILLED,
            lastTrade,
            std::nullopt);
    }

    // Nothing matched, so the entire order is resting.
    return CMEMatchingResult(
        incomingOrder.getOrderId(),
        CMEMatchingStatus::RESTING,
        std::nullopt,
        std::nullopt);
}

CMEOrderRejection CMEOrderBook::createValidationRejection(const CMEOrder& order, CMEOrderValidationResult validationResult) const
{
    switch(validationResult)
    {
        case CMEOrderValidationResult::VALID:
            return CMEOrderRejection(order.getOrderId(), CMEOrderRejectionReason::NONE, "Valid Order.");
        case CMEOrderValidationResult::INVALID_ORDER_ID:
            return CMEOrderRejection(order.getOrderId(), CMEOrderRejectionReason::INVALID_ORDER_ID, "Order identifier must be greater than zero.");
        case CMEOrderValidationResult::EMPTY_SYMBOL:
            return CMEOrderRejection(order.getOrderId(), CMEOrderRejectionReason::EMPTY_SYMBOL, "Order symbol must not be empty.");       
        case CMEOrderValidationResult::INVALID_PRICE:
            return CMEOrderRejection(order.getOrderId(), CMEOrderRejectionReason::INVALID_PRICE, "Order price must be greater than zero.");         
        case CMEOrderValidationResult::INVALID_QUANTITY:
            return CMEOrderRejection(order.getOrderId(), CMEOrderRejectionReason::INVALID_QUANTITY, "Order quantity must be greater than zero.");      
        default: 
            return CMEOrderRejection(order.getOrderId(), CMEOrderRejectionReason::UNKNOWN, "Unknown order validation failure.");
    }
}

CMEMarketDataSnapshot CMEOrderBook::getMarketDataSnapshot() const
{
    std::optional<CMEPrice> snapshotBestBid = std::nullopt;
    std::optional<CMEQuantity> snapshotBestBidQuantity = std::nullopt;
    std::optional<CMEPrice> snapshotBestAsk = std::nullopt;
    std::optional<CMEQuantity> snapshotBestAskQuantity = std::nullopt;
    std::vector<CMEMarketDataLevel> snapshotBidLevels;
    std::vector<CMEMarketDataLevel> snapshotAskLevels;
    std::uint64_t snapshotTimestamp = 0;

    if (hasBuyLevels())
    {
        CMEPrice bestBid = getBestBid();
        const CMEPriceLevel& bestBidLevel = getBuyLevel(bestBid);

        snapshotBestBid = bestBid;
        snapshotBestBidQuantity =
            bestBidLevel.getTotalRemainingQuantity();
    }

    if (hasSellLevels())
    {
        CMEPrice bestAsk = getBestAsk();
        const CMEPriceLevel& bestAskLevel = getSellLevel(bestAsk);

        snapshotBestAsk = bestAsk;
        snapshotBestAskQuantity =
            bestAskLevel.getTotalRemainingQuantity();
    }

    for (std::map<std::int64_t, CMEPriceLevel>::const_reverse_iterator it =
             buyLevels.rbegin();
         it != buyLevels.rend();
         ++it)
    {
        CMEPrice levelPrice = it->second.getPrice();
        CMEQuantity levelQuantity =
            it->second.getTotalRemainingQuantity();

        CMEMarketDataLevel level(
            levelPrice,
            levelQuantity);

        snapshotBidLevels.push_back(level);
    }

    for (std::map<std::int64_t, CMEPriceLevel>::const_iterator it =
             sellLevels.begin();
         it != sellLevels.end();
         ++it)
    {
        CMEPrice levelPrice = it->second.getPrice();
        CMEQuantity levelQuantity =
            it->second.getTotalRemainingQuantity();

        CMEMarketDataLevel level(
            levelPrice,
            levelQuantity);

        snapshotAskLevels.push_back(level);
    }

    if(clock != nullptr)
    {
        snapshotTimestamp = clock->now();
    }

    return CMEMarketDataSnapshot(
        bookSymbol,
        snapshotBestBid,
        snapshotBestBidQuantity,
        snapshotBestAsk,
        snapshotBestAskQuantity,
        buyLevels.size(),
        sellLevels.size(),
        snapshotBidLevels,
        snapshotAskLevels,
        sequenceNumber,
        snapshotTimestamp);
}
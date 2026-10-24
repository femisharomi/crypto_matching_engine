#include "cme/engine/matching_engine.hpp"
#include "cme/trade/trade_publisher.hpp"


CMEMatchingEngine::CMEMatchingEngine(CMETradePublisher* publisher, CMEClock* engineclock) : 
                                    tradePublisher(publisher), clock(engineclock)
{
    
}

CMEEngineEvent CMEMatchingEngine::processCommandWithEvent(const CMEEngineCommand& command)
{
    switch (command.getCommandType())
    {
        case CMEEngineCommandType::SUBMIT_ORDER:
        {
            if (!command.getOrder().has_value())
            {
                return CMEEngineEvent(
                    CMEEngineEventType::COMMAND_REJECTED,
                    command.getSymbol(),
                    CMEOrderId(0),
                    std::nullopt);
            }

            CMEOrder submittedOrder = command.getOrder().value();

            CMEMatchingResult result =
                processOrder(submittedOrder);

            return CMEEngineEvent(
                CMEEngineEventType::ORDER_PROCESSED,
                submittedOrder.getOrderSymbol(),
                submittedOrder.getOrderId(),
                result);
        }

        case CMEEngineCommandType::CANCEL_ORDER:
        {
            if (!command.getOrderId().has_value())
            {
                return CMEEngineEvent(
                    CMEEngineEventType::COMMAND_REJECTED,
                    command.getSymbol(),
                    CMEOrderId(0),
                    std::nullopt);
            }

            CMEOrderId orderId =
                command.getOrderId().value();

            if (!containsOrderBook(command.getSymbol()))
            {
                return CMEEngineEvent(
                    CMEEngineEventType::COMMAND_REJECTED,
                    command.getSymbol(),
                    orderId,
                    std::nullopt);
            }

            CMEOrderBook& orderBook =
                getMutableOrderBook(command.getSymbol());

            bool cancellationSucceeded =
                orderBook.cancelOrder(orderId);

            if (cancellationSucceeded)
            {
                return CMEEngineEvent(
                    CMEEngineEventType::ORDER_CANCELLED,
                    command.getSymbol(),
                    orderId,
                    std::nullopt);
            }

            return CMEEngineEvent(
                CMEEngineEventType::COMMAND_REJECTED,
                command.getSymbol(),
                orderId,
                std::nullopt);
        }

        case CMEEngineCommandType::MODIFY_ORDER:
        {
            if (!command.getOrderId().has_value() ||
                !command.getNewPrice().has_value() ||
                !command.getNewQuantity().has_value())
            {
                return CMEEngineEvent(
                    CMEEngineEventType::COMMAND_REJECTED,
                    command.getSymbol(),
                    CMEOrderId(0),
                    std::nullopt);
            }

            CMEOrderId orderId =
                command.getOrderId().value();

            if (!containsOrderBook(command.getSymbol()))
            {
                return CMEEngineEvent(
                    CMEEngineEventType::COMMAND_REJECTED,
                    command.getSymbol(),
                    orderId,
                    std::nullopt);
            }

            CMEOrderBook& orderBook =
                getMutableOrderBook(command.getSymbol());

            bool modificationSucceeded =
                orderBook.modifyOrder(
                    orderId,
                    command.getNewPrice().value(),
                    command.getNewQuantity().value());

            if (modificationSucceeded)
            {
                return CMEEngineEvent(
                    CMEEngineEventType::ORDER_MODIFIED,
                    command.getSymbol(),
                    orderId,
                    std::nullopt);
            }

            return CMEEngineEvent(
                CMEEngineEventType::COMMAND_REJECTED,
                command.getSymbol(),
                orderId,
                std::nullopt);
        }

        default:
            throw std::runtime_error(
                "Function: CMEMatchingEngine::processCommandWithEvent() - Unknown command type encountered!");
    }
}

bool CMEMatchingEngine::processCommand(
    const CMEEngineCommand& command)
{
    CMEEngineEvent event =
        processCommandWithEvent(command);

    return event.getEventType() !=
           CMEEngineEventType::COMMAND_REJECTED;
}

CMEMatchingResult CMEMatchingEngine::processOrder(CMEOrder incomingOrder)
{
    // Check whether an order book already exists for the order symbol.
    if (!containsOrderBook(incomingOrder.getOrderSymbol()))
    {
        // Order book doesn't exist, so create one.
        orderBooks.try_emplace(
            incomingOrder.getOrderSymbol().value,
            incomingOrder.getOrderSymbol(),
            tradePublisher,
            clock);
    }

    // Retrieve the correct order book.
    return orderBooks.at(
        incomingOrder.getOrderSymbol().value).processOrder(incomingOrder);
}

std::size_t CMEMatchingEngine::getOrderBookCount() const
{
    return orderBooks.size();
}

bool CMEMatchingEngine::containsOrderBook(CMESymbol symbol) const
{
    std::map<std::string, CMEOrderBook>::const_iterator it = orderBooks.find(symbol.value);

    return it != orderBooks.end();
}

const CMEOrderBook& CMEMatchingEngine::getOrderBook(CMESymbol symbol) const
{
    return orderBooks.at(symbol.value);
}

CMEOrderBook& CMEMatchingEngine::getMutableOrderBook(CMESymbol symbol)
{
    return orderBooks.at(symbol.value);
}
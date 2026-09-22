#include "cme/engine/matching_engine.hpp"


CMEMatchingEngine::CMEMatchingEngine()
{
    // No work to perform
}

bool CMEMatchingEngine::processCommand(const CMEEngineCommand& command)
{
    switch(command.getCommandType())
    {
        case CMEEngineCommandType::SUBMIT_ORDER:
        {
            if(!command.getOrder().has_value())
            {
                return false;
            }
            
            CMEMatchingResult result = processOrder(command.getOrder().value());
            return result.getStatus() != CMEMatchingStatus::REJECTED;
        }
        case CMEEngineCommandType::CANCEL_ORDER:
        {
            if(!command.getOrderId().has_value())
            {
                return false;
            }

            if(!containsOrderBook(command.getSymbol()))
            {
                return false;
            }

            CMEOrderBook& orderBook = getMutableOrderBook(command.getSymbol());

            return orderBook.cancelOrder(command.getOrderId().value());
        }
        case CMEEngineCommandType::MODIFY_ORDER:
        {
            if (!command.getOrderId().has_value() ||
                !command.getNewPrice().has_value() ||
                !command.getNewQuantity().has_value())
            {
                return false;
            }

            if (!containsOrderBook(command.getSymbol()))
            {
                return false;
            }

            CMEOrderBook& orderBook = getMutableOrderBook(command.getSymbol());

            return orderBook.modifyOrder(
                command.getOrderId().value(),
                command.getNewPrice().value(),
                command.getNewQuantity().value());
        }
        default:
            throw std::runtime_error(
                "Function: CMEMatchingEngine::processCommand() - Unknown command type encountered!");
    }
}

CMEMatchingResult CMEMatchingEngine::processOrder(CMEOrder incomingOrder)
{
    // Check whether an order book already exists for the order symbol.
    if(!containsOrderBook(incomingOrder.getOrderSymbol()))
    {
        // Order book doesnt exist, so create one
        orderBooks.try_emplace(incomingOrder.getOrderSymbol().value, incomingOrder.getOrderSymbol());
    }

    // Retrieve the correct order book.
    return orderBooks.at(incomingOrder.getOrderSymbol().value).processOrder(incomingOrder);
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
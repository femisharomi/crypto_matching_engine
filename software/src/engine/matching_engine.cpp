#include "cme/engine/matching_engine.hpp"


CMEMatchingEngine::CMEMatchingEngine()
{
    // No work to perform
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
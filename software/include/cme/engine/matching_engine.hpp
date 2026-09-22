#ifndef CME_ENGINE_MATCHING_ENGINE_HPP
#define CME_ENGINE_MATCHING_ENGINE_HPP

#include <cstddef>
#include <map>

#include "cme/core/types.hpp"
#include "cme/matching/matching_result.hpp"
#include "cme/order_book/order_book.hpp"
#include "cme/engine/engine_command.hpp"
#include "cme/engine/engine_event.hpp"
#include "cme/trade/trade_publisher.hpp"

/* Stores and manages multiple order books. */
class CMEMatchingEngine
{
public:
    /* Creates an empty matching engine using the supplied trade publisher when one is provided. */
    CMEMatchingEngine(CMETradePublisher* publisher = nullptr);

    /* Processes an order using the correct order book. */
    CMEMatchingResult processOrder(CMEOrder incomingOrder); 

    /* Returns the number of order books currently managed by the engine. */
    std::size_t getOrderBookCount() const;

    /* Returns whether an order book exists for the supplied symbol. */
    bool containsOrderBook(CMESymbol symbol) const;

    /* Returns the order book for the supplied symbol. */
    const CMEOrderBook& getOrderBook(CMESymbol symbol) const;

    /* Processes one command using the appropriate order book. */
    bool processCommand(const CMEEngineCommand& command);

    /* Processes one command and returns an event describing the result. */
    CMEEngineEvent processCommandWithEvent(const CMEEngineCommand& command);

private:
    // The order books indexed by trading symbol.
    std::map<std::string, CMEOrderBook> orderBooks;

    /* Returns modifiable access to the order book for the supplied symbol. */
    CMEOrderBook& getMutableOrderBook(CMESymbol symbol);

    // The trade publisher supplied to order books created by this engine.
    CMETradePublisher* tradePublisher;
};

#endif // CME_ENGINE_MATCHING_ENGINE_HPP
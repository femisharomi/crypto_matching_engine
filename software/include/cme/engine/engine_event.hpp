#ifndef CME_ENGINE_ENGINE_EVENT_HPP
#define CME_ENGINE_ENGINE_EVENT_HPP

#include <optional>

#include "cme/core/types.hpp"
#include "cme/matching/matching_result.hpp"

/* Represents an event produced after the matching engine processes a command. */
enum class CMEEngineEventType
{
    // An order submission was processed.
    ORDER_PROCESSED,

    // An existing order was successfully cancelled.
    ORDER_CANCELLED,

    // An existing order was successfully modified.
    ORDER_MODIFIED,

    // A command could not be completed.
    COMMAND_REJECTED
};

/* Stores information describing the result of an engine command. */
class CMEEngineEvent
{
public: 
    /* Creates an engine event from the supplied event details. */
    CMEEngineEvent(CMEEngineEventType type, 
                   CMESymbol symbol, 
                   CMEOrderId orderId, 
                   std::optional<CMEMatchingResult> result);

    /* Returns the type of event produced by the matching engine. */
    CMEEngineEventType getEventType() const;

    /* Returns the trading symbol associated with this event. */
    CMESymbol getSymbol() const;

    /* Returns the order identifier associated with this event. */
    CMEOrderId getOrderId() const;

    /* Returns the matching result when this event contains one. */
    const std::optional<CMEMatchingResult>& getMatchingResult() const;

    /* Returns whether this event contains a matching result. */
    bool hasMatchingResult() const;

private:
    // The type of event produced by the matching engine.
    CMEEngineEventType eventType;

    // The trading symbol associated with this event.
    CMESymbol eventSymbol;

    // The order identifier associated with this event.
    CMEOrderId eventOrderId;

    // The matching result produced by an order submission, when one exists.
    std::optional<CMEMatchingResult> matchingResult;
};

#endif // CME_ENGINE_ENGINE_EVENT_HPP
#include "cme/engine/engine_event.hpp"

CMEEngineEvent::CMEEngineEvent(CMEEngineEventType type, CMESymbol symbol, CMEOrderId orderId, std::optional<CMEMatchingResult> result) :
                eventType(type), eventSymbol(symbol), eventOrderId(orderId), matchingResult(result)
{

}


CMEEngineEventType CMEEngineEvent::getEventType() const
{
    return eventType;
}


CMESymbol CMEEngineEvent::getSymbol() const
{
    return eventSymbol;
}

CMEOrderId CMEEngineEvent::getOrderId() const
{
    return eventOrderId;
}

const std::optional<CMEMatchingResult>& CMEEngineEvent::getMatchingResult() const
{
    return matchingResult;
}

bool CMEEngineEvent::hasMatchingResult() const
{
    return matchingResult.has_value();
}
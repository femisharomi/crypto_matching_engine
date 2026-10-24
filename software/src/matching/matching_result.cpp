#include "cme/matching/matching_result.hpp"

CMEMatchingResult::CMEMatchingResult(CMEOrderId processedOrderId,
                                    CMEMatchingStatus matchingStatus,
                                    std::optional<CMETrade> generatedTrade,
                                    std::optional<CMEOrderRejection> orderRejection):
                                    orderId(processedOrderId), status(matchingStatus),
                                    trade(generatedTrade), rejection(orderRejection)
{
    
}

CMEOrderId CMEMatchingResult::getOrderId() const
{
    return orderId;
}

CMEMatchingStatus CMEMatchingResult::getStatus() const
{
    return status;
}

const std::optional<CMETrade>& CMEMatchingResult::getTrade() const
{
    return trade;
}

const std::optional<CMEOrderRejection>& CMEMatchingResult::getRejection() const
{
    return rejection;
}

bool CMEMatchingResult::hasTrade() const
{
    return trade.has_value();
}

bool CMEMatchingResult::hasRejection() const
{
    return rejection.has_value();
}
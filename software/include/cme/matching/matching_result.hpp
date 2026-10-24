#ifndef CME_MATCHING_MATCHING_RESULT_HPP
#define CME_MATCHING_MATCHING_RESULT_HPP

#include <optional>
#include "cme/order/order.hpp"
#include "cme/order/order_rejection.hpp"
#include "cme/trade/trade.hpp"

/* Represents the final processing state of an order. */
enum class CMEMatchingStatus
{
    RESTING, // The order was accepted and is resting in the order book.
    PARTIALLY_FILLED, // The order traded but still has quantity remaining.
    FILLED, // The order was completely filled.
    CANCELLED, // The remaining order quantity was cancelled.
    REJECTED // The order was rejected before being accepted.
};

/* Stores the result produced when an order is processed by the matching engine. */
class CMEMatchingResult
{
public:
    /* Creates a matching result from the supplied processing details. */
    CMEMatchingResult(CMEOrderId processedOrderId,
                      CMEMatchingStatus matchingStatus,
                      std::optional<CMETrade> generatedTrade,
                      std::optional<CMEOrderRejection> orderRejection);

    /* Returns the identifier of the processed order. */
    CMEOrderId getOrderId() const;

    /* Returns the final processing status of the order. */
    CMEMatchingStatus getStatus() const;

    /* Returns the generated trade when one exists. */
    const std::optional<CMETrade>& getTrade() const;

    /* Returns the rejection information when the order was rejected. */
    const std::optional<CMEOrderRejection>& getRejection() const;

    /* Returns whether this matching result contains a generated trade. */
    bool hasTrade() const;

    /* Returns whether this matching result contains rejection information. */
    bool hasRejection() const;

private:
    // The identifier of the processed order.
    CMEOrderId orderId;

    // The final processing status of the order.
    CMEMatchingStatus status;

    // The trade generated while processing the order, when one exists.
    std::optional<CMETrade> trade;

    // The rejection information when the order was rejected.
    std::optional<CMEOrderRejection> rejection;
};

#endif // CME_MATCHING_MATCHING_RESULT_HPP
#ifndef CME_ORDER_ORDER_REJECTION_HPP
#define CME_ORDER_ORDER_REJECTION_HPP

#include <string>
#include "cme/core/types.hpp"


/* Represents why an order was rejected. */
enum class CMEOrderRejectionReason
{
    NONE,
    INVALID_ORDER_ID,
    EMPTY_SYMBOL,
    INVALID_PRICE,
    INVALID_QUANTITY,
    WRONG_SYMBOL,
    FILL_OR_KILL_NOT_POSSIBLE,
    UNKNOWN
};

/* Stores the details describing why an order was rejected. */
class CMEOrderRejection
{
public:
    /* Creates a rejection describing one rejected order. */
    CMEOrderRejection(CMEOrderId rejectedOrderId, 
                      CMEOrderRejectionReason rejectionReason, 
                      std::string rejectionMessage);

    /* Creates a rejection describing one rejected order. */
    CMEOrderId getOrderId() const;

    /* Returns the rejection reason. */
    CMEOrderRejectionReason getReason() const;

    /* Returns the rejection message. */
    std::string getMessage() const;
    
private:
    // The identifier of the rejected order.
    CMEOrderId orderId;

    // The reason the order was rejected.
    CMEOrderRejectionReason reason;

    // A human-readable explanation of the rejection.
    std::string message;
};

#endif // CME_ORDER_ORDER_REJECTION_HPP
#include "cme/order/order_rejection.hpp"


CMEOrderRejection::CMEOrderRejection(CMEOrderId rejectedOrderId, 
                                    CMEOrderRejectionReason rejectionReason, 
                                    std::string rejectionMessage) 
                : orderId(rejectedOrderId), reason(rejectionReason), message(rejectionMessage)
{

}

CMEOrderId CMEOrderRejection::getOrderId() const
{
    return orderId;
}

CMEOrderRejectionReason CMEOrderRejection::getReason() const
{
    return reason;
}

std::string CMEOrderRejection::getMessage() const
{
    return message;
}
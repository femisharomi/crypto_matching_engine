#ifndef CME_ENGINE_ENGINE_COMMAND_HPP
#define CME_ENGINE_ENGINE_COMMAND_HPP

#include <optional>

#include "cme/order/order.hpp"

/* Represents the operation requested from the matching engine. */
enum class CMEEngineCommandType
{
    // Submit a new order to the matching engine.
    SUBMIT_ORDER,

    // Cancel an existing resting order.
    CANCEL_ORDER,

    // Replace the price and quantity of an existing resting order.
    MODIFY_ORDER
};

/* Stores one operation to be processed by the matching engine. */
class CMEEngineCommand
{
public:
    /* Creates a command that submits a new order. */
    CMEEngineCommand(CMEOrder submittedOrder);

    /* Creates a command that cancels an existing order. */
    static CMEEngineCommand createCancelCommand(CMESymbol symbol, CMEOrderId existingOrderId);

    /* Creates a command that modifies an existing order. */
    static CMEEngineCommand createModifyCommand(CMESymbol symbol, CMEOrderId existingOrderId, CMEPrice replacementPrice, CMEQuantity replacementQuantity);

    /* Returns the operation requested by this command. */
    CMEEngineCommandType getCommandType() const;

    /* Returns the trading symbol associated with this command. */
    CMESymbol getSymbol() const;

    /* Returns the submitted order when this is an order submission command. */
    const std::optional<CMEOrder>& getOrder() const; 

    /* Returns the targeted order identifier when one exists. */
    const std::optional<CMEOrderId>& getOrderId() const;

    /* Returns the replacement price when this is a modification command. */
    const std::optional<CMEPrice>& getNewPrice() const;

    /* Returns the replacement quantity when this is a modification command. */
    const std::optional<CMEQuantity>& getNewQuantity() const;

private:
    // The operation requested from the matching engine.
    CMEEngineCommandType commandType; 

    // The trading symbol associated with this command.
    CMESymbol commandSymbol; 

    // The order supplied by a submit command, when one exists.
    std::optional<CMEOrder> order; 

    // The existing order identifier targeted by this command, when one exists.
    std::optional<CMEOrderId> orderId;

    // The replacement price supplied by a modification command, when one exists.
    std::optional<CMEPrice> newPrice;

    // The replacement quantity supplied by a modification command, when one exists.
    std::optional<CMEQuantity> newQuantity;

    /* Creates a command from the supplied command details. */
    CMEEngineCommand(CMEEngineCommandType type,
                    CMESymbol symbol,
                    std::optional<CMEOrder> submittedOrder,
                    std::optional<CMEOrderId> existingOrderId,
                    std::optional<CMEPrice> replacementPrice,
                    std::optional<CMEQuantity> replacementQuantity);
};


#endif // CME_ENGINE_ENGINE_COMMAND_HPP
#include "cme/engine/engine_command.hpp"

/* Creates a command that submits a new order. */
CMEEngineCommand::CMEEngineCommand(CMEOrder submittedOrder) :
    commandType(CMEEngineCommandType::SUBMIT_ORDER),
    commandSymbol(submittedOrder.getOrderSymbol()),
    order(submittedOrder),
    orderId(std::nullopt),
    newPrice(std::nullopt),
    newQuantity(std::nullopt)
{

}

/* Creates a command from the supplied command details. */
CMEEngineCommand::CMEEngineCommand(CMEEngineCommandType type,
                CMESymbol symbol,
                std::optional<CMEOrder> submittedOrder,
                std::optional<CMEOrderId> existingOrderId,
                std::optional<CMEPrice> replacementPrice,
                std::optional<CMEQuantity> replacementQuantity) :
                commandType(type),
                commandSymbol(symbol),
                order(submittedOrder),
                orderId(existingOrderId),
                newPrice(replacementPrice),
                newQuantity(replacementQuantity)
{

}

CMEEngineCommand CMEEngineCommand::createCancelCommand(CMESymbol symbol, CMEOrderId existingOrderId)
{
    return CMEEngineCommand(CMEEngineCommandType::CANCEL_ORDER, symbol, std::nullopt, existingOrderId, std::nullopt, std::nullopt);
}

CMEEngineCommand CMEEngineCommand::createModifyCommand(CMESymbol symbol, CMEOrderId existingOrderId, CMEPrice replacementPrice, CMEQuantity replacementQuantity)
{
    return CMEEngineCommand(CMEEngineCommandType::MODIFY_ORDER, symbol, std::nullopt, existingOrderId, replacementPrice, replacementQuantity);
}

CMEEngineCommandType CMEEngineCommand::getCommandType() const
{
    return commandType;
}

CMESymbol CMEEngineCommand::getSymbol() const
{
    return commandSymbol;
}

const std::optional<CMEOrder>& CMEEngineCommand::getOrder() const
{
    return order;
}

const std::optional<CMEOrderId>& CMEEngineCommand::getOrderId() const
{
    return orderId;
}

const std::optional<CMEPrice>& CMEEngineCommand::getNewPrice() const
{
    return newPrice;
}

const std::optional<CMEQuantity>& CMEEngineCommand::getNewQuantity() const
{
    return newQuantity;
}
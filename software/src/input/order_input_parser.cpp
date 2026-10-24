#include <cstdint>
#include <sstream>
#include <string>

#include "cme/input/order_input_parser.hpp"

std::optional<CMEEngineCommand> CMEOrderInputParser::parse(
    const std::string& input,
    CMEOrderId newOrderId) const
{
    std::istringstream inputStream(input);

    std::string command;

    if (!(inputStream >> command))
    {
        return std::nullopt;
    }

    if (command == "BUY" || command == "SELL")
    {
        std::string symbolValue;
        std::int64_t priceValue;
        std::uint64_t quantityValue;

        if (!(inputStream >> symbolValue >> priceValue >> quantityValue))
        {
            return std::nullopt;
        }

        CMESide side = CMESide::BUY;

        if (command == "SELL")
        {
            side = CMESide::SELL;
        }

        CMEOrder order(
            newOrderId,
            CMESymbol(symbolValue),
            side,
            CMEPrice(priceValue),
            CMEQuantity(quantityValue),
            false,
            CMETimeInForce::GTC);

        return CMEEngineCommand(order);
    }

    if (command == "CANCEL")
    {
        std::string symbolValue;
        std::uint64_t orderIdValue;

        if (!(inputStream >> symbolValue >> orderIdValue))
        {
            return std::nullopt;
        }

        return CMEEngineCommand::createCancelCommand(
            CMESymbol(symbolValue),
            CMEOrderId(orderIdValue));
    }

    return std::nullopt;
}
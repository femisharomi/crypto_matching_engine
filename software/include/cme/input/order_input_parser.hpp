#ifndef CME_INPUT_ORDER_INPUT_PARSER_HPP
#define CME_INPUT_ORDER_INPUT_PARSER_HPP

#include <optional>
#include <string>

#include "cme/engine/engine_command.hpp"

/* Converts text order input into matching engine commands. */
class CMEOrderInputParser
{
public:
    /* Parses text input into an engine command when the input is valid. */
    std::optional<CMEEngineCommand> parse(
        const std::string& input,
        CMEOrderId newOrderId) const;
};

#endif // CME_INPUT_ORDER_INPUT_PARSER_HPP
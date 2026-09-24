#include <optional>
#include <string>

#include "cme/input/simulated_order_feed.hpp"

CMESimulatedOrderFeed::CMESimulatedOrderFeed(
    const std::string& filePath,
    CMEMatchingEngine* matchingEngine)
    : fileReader(filePath),
      parser(),
      engine(matchingEngine),
      nextOrderId(1)
{
}

bool CMESimulatedOrderFeed::isOpen() const
{
    return fileReader.isOpen();
}

std::size_t CMESimulatedOrderFeed::processAll()
{
    if (!fileReader.isOpen() || engine == nullptr)
    {
        return 0;
    }

    std::size_t processedCommandCount = 0;
    std::string line;

    while (fileReader.readNextLine(line))
    {
        std::optional<CMEEngineCommand> command =
            parser.parse(
                line,
                CMEOrderId(nextOrderId));

        if (!command.has_value())
        {
            continue;
        }

        bool isSubmitCommand =
            command->getCommandType() ==
            CMEEngineCommandType::SUBMIT_ORDER;

        if (isSubmitCommand)
        {
            nextOrderId++;
        }

        bool commandProcessed =
            engine->processCommand(
                command.value());

        if (commandProcessed)
        {
            processedCommandCount++;
        }
    }

    return processedCommandCount;
}

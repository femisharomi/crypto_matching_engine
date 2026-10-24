#ifndef CME_INPUT_SIMULATED_ORDER_FEED_HPP
#define CME_INPUT_SIMULATED_ORDER_FEED_HPP

#include <cstddef>
#include <cstdint>
#include <string>

#include "cme/engine/matching_engine.hpp"
#include "cme/input/file_order_reader.hpp"
#include "cme/input/order_input_parser.hpp"

/* Replays order commands from a file through the matching engine. */
class CMESimulatedOrderFeed
{
public:
    /* Creates a simulated order feed using the supplied file and matching engine. */
    CMESimulatedOrderFeed(
        const std::string& filePath,
        CMEMatchingEngine* matchingEngine);

    /* Returns whether the simulated order input file was opened successfully. */
    bool isOpen() const;

    /* Processes all remaining valid commands from the simulated order input file. */
    std::size_t processAll();

private:
    // Reads order command lines from the simulated input file.
    CMEFileOrderReader fileReader;

    // Converts input lines into matching engine commands.
    CMEOrderInputParser parser;

    // The non-owning matching engine that receives simulated commands.
    CMEMatchingEngine* engine;

    // The identifier assigned to the next submitted order.
    std::uint64_t nextOrderId;
};

#endif // CME_INPUT_SIMULATED_ORDER_FEED_HPP
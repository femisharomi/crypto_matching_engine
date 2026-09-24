#include <cstddef>
#include <iostream>
#include "cme/engine/matching_engine.hpp"
#include "cme/input/simulated_order_feed.hpp"
#include "cme/trade/in_memory_trade_publisher.hpp"

int main(int argc, char* argv[])
{
    std::cout << "crypto_matching_engine Started" << std::endl;

    if (argc != 2)
    {
        std::cout
            << "Usage: cme_app <order_file>"
            << std::endl;

        return 1;
    }

    std::cout
        << "============================================================"
        << std::endl;

    std::cout
        << "                CRYPTO MATCHING ENGINE"
        << std::endl;

    std::cout
        << "============================================================"
        << std::endl;

    std::cout
        << std::endl;

    std::cout
        << "Loading order feed: "
        << argv[1]
        << std::endl;

    std::cout
        << std::endl;

    CMEInMemoryTradePublisher tradePublisher;

    CMEMatchingEngine engine(
        &tradePublisher);

    CMESimulatedOrderFeed feed(
        argv[1],
        &engine);

    if (!feed.isOpen())
    {
        std::cout
            << "Failed to open order file."
            << std::endl;

        return 1;
    }

    std::size_t processedCommandCount =
        feed.processAll();

    std::cout
        << "Order feed processed successfully."
        << std::endl;

    std::cout
        << std::endl;

    std::cout
        << "============================================================"
        << std::endl;

    std::cout
        << "                     TRADES"
        << std::endl;

    std::cout
        << "============================================================"
        << std::endl;

    std::cout
        << std::endl;

    const std::vector<CMETrade>& trades =
        tradePublisher.getTrades();

    if (trades.empty())
    {
        std::cout
            << "No trades executed."
            << std::endl;
    }
    else
    {
        for (const CMETrade& trade : trades)
        {
            std::cout
                << "Trade ID:  "
                << trade.getTradeId().value
                << std::endl;

            std::cout
                << "Symbol:    "
                << trade.getTradeSymbol().value
                << std::endl;

            std::cout
                << "Price:     "
                << trade.getTradePrice().value
                << std::endl;

            std::cout
                << "Quantity:  "
                << trade.getTradeQuantity().value
                << std::endl;

            std::cout
                << "Buy ID:    "
                << trade.getBuyOrderId().value
                << std::endl;

            std::cout
                << "Sell ID:   "
                << trade.getSellOrderId().value
                << std::endl;

            std::cout
                << "------------------------------------------------------------"
                << std::endl;
        }
    }

    std::cout
        << std::endl;

    std::cout
        << "============================================================"
        << std::endl;

    std::cout
        << "                 SESSION SUMMARY"
        << std::endl;

    std::cout
        << "============================================================"
        << std::endl;

    std::cout
        << "Commands processed: "
        << processedCommandCount
        << std::endl;

    std::cout
        << "Trades executed:    "
        << tradePublisher.getTradeCount()
        << std::endl;

    std::cout
        << "============================================================"
        << std::endl;


    std::cout << "crypto_matching_engine Ended!" << std::endl;
    return 0;
}
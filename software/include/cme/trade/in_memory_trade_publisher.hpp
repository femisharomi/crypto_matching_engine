#ifndef CME_TRADE_IN_MEMORY_TRADE_PUBLISHER_HPP
#define CME_TRADE_IN_MEMORY_TRADE_PUBLISHER_HPP

#include <cstddef>
#include <vector>
#include "cme/trade/trade_publisher.hpp"

/* Stores published trades in memory for later inspection. */
class CMEInMemoryTradePublisher : public CMETradePublisher
{
public:
    /* Creates an empty in-memory trade publisher. */
    CMEInMemoryTradePublisher();

    /* Stores a published trade in memory. */
    void publishTrade(const CMETrade& trade) override;

    /* Returns the number of trades currently stored. */
    std::size_t getTradeCount() const;

    /* Returns the trade stored at the supplied position. */
    const CMETrade& getTrade(std::size_t index) const;

    /* Returns all trades currently stored in publication order. */
    const std::vector<CMETrade>& getTrades() const;

    /* Removes all trades currently stored by the publisher. */
    void clear();

private:
    // The trades published to this publisher in publication order.
    std::vector<CMETrade> trades;

};

#endif // CME_TRADE_IN_MEMORY_TRADE_PUBLISHER_HPP
#ifndef CME_MARKET_DATA_MARKET_DATA_SNAPSHOT_HPP
#define CME_MARKET_DATA_MARKET_DATA_SNAPSHOT_HPP

#include <cstddef>
#include <optional>

#include "cme/core/types.hpp"

/* Stores a read-only snapshot of the current state of one order book. */
class CMEMarketDataSnapshot
{
public:
    /* Creates a market data snapshot from the supplied order book state. */
    CMEMarketDataSnapshot(CMESymbol snapshotSymbol, std::optional<CMEPrice> snapshotBestBid, 
                        std::optional<CMEPrice> snapshotBestAsk, std::size_t snapshotBuyLevelCount,
                        std::size_t snapshotSellLevelCount);

    /* Returns the trading symbol represented by this snapshot. */
    CMESymbol getSymbol() const;

    /* Returns the highest buy price when one exists. */
    const std::optional<CMEPrice>& getBestBid() const;

    /* Returns the lowest sell price when one exists. */
    const std::optional<CMEPrice>& getBestAsk() const;

    /* Returns the number of buy price levels in this snapshot. */
    std::size_t getBuyLevelCount() const;

    /* Returns the number of sell price levels in this snapshot. */
    std::size_t getSellLevelCount() const;

private:
    // The trading symbol represented by this snapshot.
    CMESymbol symbol; 

    // The highest buy price currently available, when one exists.
    std::optional<CMEPrice> bestBid;

    // The lowest sell price currently available, when one exists.
    std::optional<CMEPrice> bestAsk;

    // The number of buy price levels represented by this snapshot.
    std::size_t buyLevelCount;

    // The number of sell price levels represented by this snapshot.
    std::size_t sellLevelCount;
};

#endif // CME_MARKET_DATA_MARKET_DATA_SNAPSHOT_HPP
#ifndef CME_MARKET_DATA_MARKET_DATA_SNAPSHOT_HPP
#define CME_MARKET_DATA_MARKET_DATA_SNAPSHOT_HPP

#include <cstddef>
#include <optional>
#include <vector>

#include "cme/core/types.hpp"
#include "cme/market_data/market_data_level.hpp"

/* Stores a read-only snapshot of the current state of one order book. */
class CMEMarketDataSnapshot
{
public:
    /* Creates a market data snapshot from the supplied order book state. */
    CMEMarketDataSnapshot(CMESymbol snapshotSymbol, std::optional<CMEPrice> snapshotBestBid, std::optional<CMEQuantity> snapshotBestBidQuantity, 
                        std::optional<CMEPrice> snapshotBestAsk, std::optional<CMEQuantity> snapshotBestAskQuantity, 
                        std::size_t snapshotBuyLevelCount, std::size_t snapshotSellLevelCount, std::vector<CMEMarketDataLevel> snapshotBidLevels, 
                        std::vector<CMEMarketDataLevel> snapshotAskLevels);

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

    /* Returns the total remaining quantity available at the best bid when one exists. */
    const std::optional<CMEQuantity>& getBestBidQuantity() const;

    /* Returns the total remaining quantity available at the best ask when one exists. */
    const std::optional<CMEQuantity>& getBestAskQuantity() const;

    /* Returns all aggregated buy levels ordered from best bid to worst bid. */
    const std::vector<CMEMarketDataLevel>& getBidLevels() const;

    /* Returns all aggregated sell levels ordered from best ask to worst ask. */
    const std::vector<CMEMarketDataLevel>& getAskLevels() const;

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

    // The total remaining quantity available at the best bid, when one exists.
    std::optional<CMEQuantity> bestBidQuantity;

    // The total remaining quantity available at the best ask, when one exists.
    std::optional<CMEQuantity> bestAskQuantity;

    // The aggregated buy price levels ordered from best bid to worst bid.
    std::vector<CMEMarketDataLevel> bidLevels;

    // The aggregated sell price levels ordered from best ask to worst ask.
    std::vector<CMEMarketDataLevel> askLevels;
};

#endif // CME_MARKET_DATA_MARKET_DATA_SNAPSHOT_HPP
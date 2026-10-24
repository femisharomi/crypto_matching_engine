#ifndef CME_MARKET_DATA_MARKET_DATA_LEVEL_HPP
#define CME_MARKET_DATA_MARKET_DATA_LEVEL_HPP

#include "cme/core/types.hpp"

/* Stores the aggregated quantity available at one price level. */
class CMEMarketDataLevel
{
public:
    /* Creates a market data level from a price and aggregated quantity. */
    CMEMarketDataLevel(CMEPrice levelPrice, CMEQuantity levelQuantity);

    /* Returns the price represented by this market data level. */
    CMEPrice getPrice() const;

    /* Returns the total remaining quantity available at this price. */
    CMEQuantity getQuantity() const;

private:
    // The price represented by this market data level.
    CMEPrice price;

    // The total remaining quantity available at this price.
    CMEQuantity quantity;
};

#endif // CME_MARKET_DATA_MARKET_DATA_LEVEL_HPP
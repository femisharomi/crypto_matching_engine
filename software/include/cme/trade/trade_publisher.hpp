#ifndef CME_TRADE_TRADE_PUBLISHER_HPP
#define CME_TRADE_TRADE_PUBLISHER_HPP

#include "cme/trade/trade.hpp"

/* Defines the interface used to publish generated trades. */
class CMETradePublisher
{
public:
    /* Destroys the trade publisher interface. */
    virtual ~CMETradePublisher() = default;

    /* Publishes a generated trade to the configured trade consumer. */
    virtual void publishTrade(const CMETrade& trade) = 0;
};
#endif // CME_TRADE_TRADE_PUBLISHER_HPP
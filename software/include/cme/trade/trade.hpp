#ifndef CME_TRADE_TRADE_HPP
#define CME_TRADE_TRADE_HPP

#include <cstdint>
#include "cme/order/order.hpp"

struct CMETradeId { 
    std::uint64_t value; //The numeric value of this trade identifier

    CMETradeId(std::uint64_t tradeIdValue) : value(tradeIdValue){};
};

class CMETrade {
public:
    /* Creates a completed trade from the supplied execution details. */
    CMETrade(CMETradeId tradeIdValue, 
             CMEOrderId buyOrderIdValue, 
             CMEOrderId sellOrderIdValue, 
             CMESymbol tradeSymbolValue, 
             CMEPrice tradePriceValue, 
             CMEQuantity tradeQuantityValue);

    /* Returns the unique identifier assigned to this trade. */
    CMETradeId getTradeId() const;

    /* Returns the identifier of the buy order involved in this trade. */
    CMEOrderId getBuyOrderId() const;

    /* Returns the identifier of the sell order involved in this trade. */
    CMEOrderId getSellOrderId() const;

    /* Returns the trading symbol associated with this trade. */
    CMESymbol getTradeSymbol() const;

    /* Returns the price at which this trade executed. */
    CMEPrice getTradePrice() const;

    /* Returns the quantity executed by this trade. */
    CMEQuantity getTradeQuantity() const;

private:
    CMETradeId tradeId; // The unique identifier assigned to this trade.

    CMEOrderId buyOrderId; // The identifier of the buy order involved in this trade.

    CMEOrderId sellOrderId; // The identifier of the sell order involved in this trade.

    CMESymbol tradeSymbol; // The trading symbol associated with this trade. 

    CMEPrice tradePrice; // The price at which this trade executed

    CMEQuantity tradeQuantity; // The quantity executed by this trade. 
};

#endif // CME_TRADE_TRADE_HPP
#include "cme/trade/in_memory_trade_publisher.hpp"

CMEInMemoryTradePublisher::CMEInMemoryTradePublisher()
{
}

void CMEInMemoryTradePublisher::publishTrade(const CMETrade& trade)
{
    trades.push_back(trade);
}

std::size_t CMEInMemoryTradePublisher::getTradeCount() const
{
    return trades.size();
}

const CMETrade& CMEInMemoryTradePublisher::getTrade(std::size_t index) const
{
    return trades.at(index);
}

const std::vector<CMETrade>& CMEInMemoryTradePublisher::getTrades() const
{
    return trades;
}

void CMEInMemoryTradePublisher::clear()
{
    trades.clear();
}
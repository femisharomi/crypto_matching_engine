#include "cme/market_data/market_data_snapshot.hpp"

CMEMarketDataSnapshot::CMEMarketDataSnapshot(CMESymbol snapshotSymbol, std::optional<CMEPrice> snapshotBestBid, std::optional<CMEQuantity> snapshotBestBidQuantity,
                    std::optional<CMEPrice> snapshotBestAsk, std::optional<CMEQuantity> snapshotBestAskQuantity, std::size_t snapshotBuyLevelCount,
                    std::size_t snapshotSellLevelCount) : 
                    symbol(snapshotSymbol), bestBid(snapshotBestBid), bestBidQuantity(snapshotBestBidQuantity),
                    bestAsk(snapshotBestAsk), bestAskQuantity(snapshotBestAskQuantity),buyLevelCount(snapshotBuyLevelCount), 
                    sellLevelCount(snapshotSellLevelCount)
{
    
}

CMESymbol CMEMarketDataSnapshot::getSymbol() const
{
    return symbol;
}

const std::optional<CMEPrice>& CMEMarketDataSnapshot::getBestBid() const
{
    return bestBid;
}

const std::optional<CMEPrice>& CMEMarketDataSnapshot::getBestAsk() const
{
    return bestAsk;
}

std::size_t CMEMarketDataSnapshot::getBuyLevelCount() const
{
    return buyLevelCount;
}

std::size_t CMEMarketDataSnapshot::getSellLevelCount() const
{
    return sellLevelCount;
}

const std::optional<CMEQuantity>& CMEMarketDataSnapshot::getBestBidQuantity() const
{
    return bestBidQuantity;
}

const std::optional<CMEQuantity>& CMEMarketDataSnapshot::getBestAskQuantity() const
{
    return bestAskQuantity;
}
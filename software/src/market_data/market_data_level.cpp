#include "cme/market_data/market_data_level.hpp"

CMEMarketDataLevel::CMEMarketDataLevel(CMEPrice levelPrice, CMEQuantity levelQuantity) :
                                    price(levelPrice), quantity(levelQuantity)
{

}

CMEPrice CMEMarketDataLevel::getPrice() const
{
    return price;
}

CMEQuantity CMEMarketDataLevel::getQuantity() const
{
    return quantity;
}
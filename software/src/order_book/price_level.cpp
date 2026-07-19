#include "cme/order_book/price_level.hpp"

CMEPriceLevel::CMEPriceLevel(CMEPrice price): levelPrice(price)
{
}

CMEPrice CMEPriceLevel::getPrice() const
{
    return levelPrice;
}

bool CMEPriceLevel::addOrder(CMEOrder &order)
{
    if(order.getOrderPrice().value != levelPrice.value) return false;

    orders.push_back(order);
    return true;
}

std::size_t CMEPriceLevel::getOrderCount() const
{
    return orders.size();
}

bool CMEPriceLevel::isEmpty() const
{
    return orders.empty();
}

const CMEOrder& CMEPriceLevel::getFrontOrder() const
{
    return orders.front();
}

/* Removes the order that has waited at this price level for the longest time. */
bool CMEPriceLevel::removeFrontOrder()
{
    if(orders.empty()) return false;
    
    orders.pop_front();
    return true;
}
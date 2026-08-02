#include "cme/order_book/price_level.hpp"
#include <stdexcept>

CMEPriceLevel::CMEPriceLevel(CMEPrice price) : levelPrice(price)
{
}

CMEPrice CMEPriceLevel::getPrice() const
{
    return levelPrice;
}

bool CMEPriceLevel::addOrder(const CMEOrder &order)
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

CMEOrder& CMEPriceLevel::getFrontOrder()
{
    return orders.front();
}

bool CMEPriceLevel::removeFrontOrder()
{
    if(orders.empty()) return false;
    
    orders.pop_front();
    return true;
}

bool CMEPriceLevel::removeOrder(CMEOrderId orderId)
{
    // Start with an explicit iterator
    for (std::deque<CMEOrder>::iterator it = orders.begin(); it != orders.end();)
    {
        if (it->getOrderId() == orderId)
        {
            it = orders.erase(it); 
            return true; // Return immediately if order IDs are unique
        }
        else
        {
            // Only advance the iterator if we DID NOT erase an element
            ++it;
        }
    }
    
    return false; // Return false if the orderId was not found
}

const CMEOrder& CMEPriceLevel::getOrder(CMEOrderId orderId) const
{
    // Use a range-based for loop for clean, non-modifying read access
    for (const CMEOrder& order : orders)
    {
        if (order.getOrderId() == orderId)
        {
            return order;
        }
    }

    // Fix the exception message to reflect the correct class name
    throw std::runtime_error("Function: CMEPriceLevel::getOrder() - The requested order could not be found.");
}


bool CMEPriceLevel::containsOrder(CMEOrderId orderId) const
{
    for (const CMEOrder& order : orders)
    {
        if (order.getOrderId() == orderId)
        {
            return true;
        }
    }

    return false;
}
#ifndef CME_ORDER_BOOK_PRICE_LEVEL_HPP
#define CME_ORDER_BOOK_PRICE_LEVEL_HPP

#include <cstddef>
#include <deque> 
#include "cme/order/order.hpp"

/* Stored orders that belong to one price in first-in, first-out order. */
class CMEPriceLevel{
    public:
    /* Creates a price level for one specific price. */
    CMEPriceLevel(CMEPrice price);

    /* Returns the price represented by this price level. */
    CMEPrice getPrice() const;

    /* Adds an order when its price matches this price level. */
    bool addOrder(const CMEOrder &order);

    /* Returns the number of orders currently stored at this price level. */
    std::size_t getOrderCount() const;

    /* Returns whether this price level currently contains no orders. */
    bool isEmpty() const;

    /* Returns the order that has waited at this price level for the longest time. */
    const CMEOrder& getFrontOrder() const;

    /* Removes the order that has waited at this price level for the longest time. */
    bool removeFrontOrder();

    private:
    // The single price represented by this price level.
    CMEPrice levelPrice; 
    // The orders stored in FIFO order.
    std::deque<CMEOrder> orders; 
};
#endif // CME_ORDER_BOOK_PRICE_LEVEL_HPP
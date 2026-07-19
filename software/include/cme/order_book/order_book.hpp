#ifndef CME_ORDER_BOOK_ORDER_BOOK_HPP
#define CME_ORDER_BOOK_ORDER_BOOK_HPP

#include <cstddef>
#include <map>

#include "cme/order/order.hpp"
#include "cme/order_book/price_level.hpp"

/* Stores the buy and sell price levels for one trading symbol. */
class CMEOrderBook
{
    public:
    /* Creates an empty order book for one trading symbol. */
    CMEOrderBook(CMESymbol symbol);

    /* Returns the trading symbol represented by this order book. */
    CMESymbol getSymbol() const;

    /* Adds an order to the correct side of the order book. */
    bool addOrder(const CMEOrder &order);

    /* Returns the number of buy price levels currently stored. */
    std::size_t getBuyLevelCount() const;

    /* Returns the number of sell price levels currently stored. */
    std::size_t getSellLevelCount() const;

    /* Returns the buy price level stored at a specific price. */
    const CMEPriceLevel& getBuyLevel(CMEPrice price) const;

    /* Returns the sell price level stored at a specific price. */
    const CMEPriceLevel& getSellLevel(CMEPrice price) const;

    private:
    // The trading symbol represented by this order book.
    CMESymbol bookSymbol; 

    // The buy price levels indexed by their numeric price.
    std::map<std::int64_t, CMEPriceLevel> buyLevels; 

    // The sell price levels indexed by their numeric price.
    std::map<int64_t, CMEPriceLevel> sellLevels;


};
#endif // CME_ORDER_BOOK_ORDER_BOOK_HPP
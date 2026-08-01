#ifndef CME_ORDER_BOOK_ORDER_BOOK_HPP
#define CME_ORDER_BOOK_ORDER_BOOK_HPP

#include <cstddef>
#include <map>

#include "cme/order/order.hpp"
#include "cme/order_book/price_level.hpp"
#include "cme/order/order_validation.hpp"

/* Stores the buy and sell price levels for one trading symbol. */
class CMEOrderBook
{
public:
    /* Creates an empty order book for one trading symbol. */
    explicit CMEOrderBook(CMESymbol symbol);

    /* Returns the trading symbol represented by this order book. */
    CMESymbol getSymbol() const;

    /* Validates and adds a limit order to the correct side of the order book. */
    bool addLimitOrder(CMEOrder incomingOrder);

    /* Returns the number of buy price levels currently stored. */
    std::size_t getBuyLevelCount() const;

    /* Returns the number of sell price levels currently stored. */
    std::size_t getSellLevelCount() const;

    /* Returns the buy price level stored at a specific price. */
    const CMEPriceLevel& getBuyLevel(CMEPrice price) const;

    /* Returns the sell price level stored at a specific price. */
    const CMEPriceLevel& getSellLevel(CMEPrice price) const;

    /* Returns the highest buy price currently stored. */
    CMEPrice getBestBid() const;

    /* Returns the lowest sell price currently stored. */
    CMEPrice getBestAsk() const;

private:
    // The trading symbol represented by this order book.
    CMESymbol bookSymbol; 

    // The buy price levels indexed by their numeric price.
    std::map<std::int64_t, CMEPriceLevel> buyLevels; 

    // The sell price levels indexed by their numeric price.
    std::map<std::int64_t, CMEPriceLevel> sellLevels;

    // The validator used to check orders before they enter the order book.
    CMEOrderValidator orderValidator;

    /* Returns whether any buy price levels exist. */
    bool hasBuyLevels() const;

    /* Returns whether any sell price levels exist. */
    bool hasSellLevels() const;

    /* Returns whether an incoming order crosses the best opposite price. */
    bool canMatch(const CMEOrder &incomingOrder) const;

    /* Attempts to match an incoming order against the best opposite order. */
    bool tryMatchOrder(CMEOrder &incomingOrder);

    /* Removes an empty price level from the selected side of the order book. */
    void removeEmptyLevel(CMESide side, CMEPrice price);
};
#endif // CME_ORDER_BOOK_ORDER_BOOK_HPP
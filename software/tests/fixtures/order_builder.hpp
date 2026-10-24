#ifndef CME_TESTS_FIXTURES_ORDER_BUILDER_HPP
#define CME_TESTS_FIXTURES_ORDER_BUILDER_HPP

#include "cme/order/order.hpp"

/* Builds CMEOrder objects with convenient default values for tests. */
class CMEOrderBuilder
{
public:
    /* Creates an order builder with default test values. */
    CMEOrderBuilder();

    /* Sets the order identifier used by the builder. */
    CMEOrderBuilder& withOrderId(CMEOrderId newOrderId);

    /* Sets the symbol used by the builder. */
    CMEOrderBuilder& withSymbol(CMESymbol newSymbol);

    /* Sets the order side used by the builder. */
    CMEOrderBuilder& withSide(CMESide newSide);

    /* Sets the price used by the builder. */
    CMEOrderBuilder& withPrice(CMEPrice newPrice);

    /* Sets the quantity used by the builder. */
    CMEOrderBuilder& withQuantity(CMEQuantity newQuantity);

    /* Sets whether the built order is a market order. */
    CMEOrderBuilder& withMarketOrder(bool isMarketOrder);

    /* Sets the time in force used by the builder. */
    CMEOrderBuilder& withTimeInForce(CMETimeInForce newTimeInForce);

    /* Creates an order using the builder's current values. */
    CMEOrder build() const;

private:
    // The order identifier used when building the order.
    CMEOrderId orderId;

    // The symbol used when building the order.
    CMESymbol symbol;

    // The order side used when building the order.
    CMESide side;

    // The price used when building the order.
    CMEPrice price;

    // The quantity used when building the order.
    CMEQuantity quantity;

    // Whether the built order is a market order.
    bool marketOrder;

    // The time in force used when building the order.
    CMETimeInForce timeInForce;
};

#endif // CME_TESTS_FIXTURES_ORDER_BUILDER_HPP
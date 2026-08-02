#include "gtest/gtest.h"
#include "cme/order_book/order_book.hpp"

// ============================================================================
// CME ORDER BOOK TEST SUITE
// ============================================================================

// ============================================================================
// ORDER BOOK CREATION TESTS
// ============================================================================
TEST(CMEOrderBookTests, NewOrderBookStoresItsSymbolAndStartsEmpty)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    EXPECT_EQ(orderBook.getSymbol().value, "BTC-GBP");
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}
// ============================================================================
// BUY ORDER TESTS
// ============================================================================
TEST(CMEOrderBookTests, AddsBuyOrderToBuySide)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, AddsBuyOrdersAtSamePriceToOneLevel)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder1));
    EXPECT_TRUE(orderBook.addLimitOrder(newOrder2));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 2);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, CreatesSeparateBuyLevelsForDifferentPrices)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(49000), CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder1));
    EXPECT_TRUE(orderBook.addLimitOrder(newOrder2));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 2);

    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(49000)).getOrderCount(), 1);
}
// ============================================================================
// SELL ORDER TESTS
// ============================================================================
TEST(CMEOrderBookTests, AddsSellOrderToSellSide)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);

    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(51000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(51000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, CreatesSeparateSellLevelsForDifferentPrices)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51000), CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder1));
    EXPECT_TRUE(orderBook.addLimitOrder(newOrder2));

    EXPECT_EQ(orderBook.getSellLevelCount(), 2);

    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getSellLevel(CMEPrice(51000)).getOrderCount(), 1);
}

// ============================================================================
// SYMBOL REJECTION TESTS
// ============================================================================
TEST(CMEOrderBookTests, RejectsOrderForDifferentSymbol)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("ETH-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

// ============================================================================
// LIMIT ORDER VALIDATION TESTS
// ============================================================================
TEST(CMEOrderBookTests, RejectsLimitOrderWithZeroOrderId)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(0), CMESymbol("ETH-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, RejectsLimitOrderWithEmptySymbol)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1), CMESymbol(""), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, RejectsLimitOrderWithZeroPrice)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(0), CMEQuantity(50));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, RejectsLimitOrderWithNegativePrice)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(-5), CMEQuantity(50));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, RejectsLimitOrderWithZeroQuantity)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50), CMEQuantity(0));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, RejectsLimitOrderWithNegativeQuantity)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50), CMEQuantity(-5));

    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

// ============================================================================
// REJECTED ORDER STATE TESTS
// ============================================================================
TEST(CMEOrderBookTests, RejectedLimitOrderDoesNotModifyExistingLevels)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder1));
    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 1);

    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(0));
    EXPECT_FALSE(orderBook.addLimitOrder(newOrder2));
    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(), 1);
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1001);
}

TEST(CMEOrderBookTests, AcceptsValidLimitOrderAfterRejectedOrder)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(0), CMEQuantity(50));
    EXPECT_FALSE(orderBook.addLimitOrder(newOrder1));

    CMEOrder newOrder2(CMEOrderId(1002), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));

    EXPECT_TRUE(orderBook.addLimitOrder(newOrder2));
    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
  
    EXPECT_EQ(orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder().getOrderId().value, 1002);
}

// ============================================================================
// BEST PRICE TESTS
// ============================================================================
TEST(CMEOrderBookTests, ThrowsWhenNoBestBidExists)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    EXPECT_THROW(orderBook.getBestBid(), std::runtime_error);
}

TEST(CMEOrderBookTests, ThrowsWhenNoBestAskExists)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    EXPECT_THROW(orderBook.getBestAsk(), std::runtime_error);
}

TEST(CMEOrderBookTests, ReturnsSingleBuyPriceAsBestBid)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(502), CMEQuantity(50));
    orderBook.addLimitOrder(newOrder1);

    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(502));
}

TEST(CMEOrderBookTests, ReturnsHighestBuyPrice)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(49800), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrder3(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(49900), CMEQuantity(50));
    orderBook.addLimitOrder(newOrder1);
    orderBook.addLimitOrder(newOrder2);
    orderBook.addLimitOrder(newOrder3);

    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(50000));
}

TEST(CMEOrderBookTests, ReturnsSingleSellPriceAsBestAsk)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(50000), CMEQuantity(50));
    orderBook.addLimitOrder(newOrder1);

    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(50000));
}

TEST(CMEOrderBookTests, ReturnsLowestSellPrice)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrder1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51000), CMEQuantity(50));
    CMEOrder newOrder2(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(50900), CMEQuantity(50));
    CMEOrder newOrder3(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51200), CMEQuantity(50));
    orderBook.addLimitOrder(newOrder1);
    orderBook.addLimitOrder(newOrder2);
    orderBook.addLimitOrder(newOrder3);

    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(50900));
}

TEST(CMEOrderBookTests, ReturnsCorrectBestBidAndBestAsk)
{
    CMESymbol btc_gbp("BTC-GBP");
    CMEOrderBook orderBook(btc_gbp);

    CMEOrder newOrderBuy1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(49800), CMEQuantity(50));
    CMEOrder newOrderSell1(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51000), CMEQuantity(50));
    CMEOrder newOrderBuy2(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(50000), CMEQuantity(50));
    CMEOrder newOrderSell2(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(50900), CMEQuantity(50));
    CMEOrder newOrderBuy3(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::BUY, CMEPrice(49900), CMEQuantity(50));
    CMEOrder newOrderSell3(CMEOrderId(1001), CMESymbol("BTC-GBP"), CMESide::SELL, CMEPrice(51200), CMEQuantity(50));

    orderBook.addLimitOrder(newOrderSell1);
    orderBook.addLimitOrder(newOrderSell2);
    orderBook.addLimitOrder(newOrderSell3);
    orderBook.addLimitOrder(newOrderBuy1);
    orderBook.addLimitOrder(newOrderBuy2);
    orderBook.addLimitOrder(newOrderBuy3);

    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(50900));
    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(50000));
}

// ============================================================================
// BASIC LIMIT MATCHING TESTS
// ============================================================================
TEST(CMEOrderBookTests, MatchesBuyOrderWithBestAsk)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder sellOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    CMEOrder buyOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
}

TEST(CMEOrderBookTests, MatchesBuyOrderPricedAboveBestAsk)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder sellOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    orderBook.addLimitOrder(sellOrder);

    CMEOrder buyOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(51000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, MatchesSellOrderWithBestBid)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    orderBook.addLimitOrder(buyOrder);

    CMEOrder sellOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, MatchesSellOrderPricedBelowBestBid)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    orderBook.addLimitOrder(buyOrder);

    CMEOrder sellOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(49000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

// ============================================================================
// PARTIAL FILL MATCHING TESTS
// ============================================================================
TEST(CMEOrderBookTests, SmallerIncomingBuyPartiallyFillsRestingSell)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    CMEOrder incomingBuy(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(40)
    );

    EXPECT_TRUE(orderBook.addLimitOrder(restingSell));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuy));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    const CMEOrder& remainingSell =
        orderBook.getSellLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(remainingSell.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(remainingSell.getOrderRemainingQuantity(), CMEQuantity(60));
    EXPECT_FALSE(remainingSell.isOrderFilled());
}

TEST(CMEOrderBookTests, SmallerIncomingSellPartiallyFillsRestingBuy)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingBuy(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    CMEOrder incomingSell(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(30)
    );

    EXPECT_TRUE(orderBook.addLimitOrder(restingBuy));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingSell));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    const CMEOrder& remainingBuy =
        orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(remainingBuy.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(remainingBuy.getOrderRemainingQuantity(), CMEQuantity(70));
    EXPECT_FALSE(remainingBuy.isOrderFilled());
}

TEST(CMEOrderBookTests, LargerIncomingBuyRemainsAfterFillingRestingSell)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(40)
    );

    CMEOrder incomingBuy(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    EXPECT_TRUE(orderBook.addLimitOrder(restingSell));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuy));

    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);

    const CMEOrder& remainingBuy =
        orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(remainingBuy.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(remainingBuy.getOrderRemainingQuantity(), CMEQuantity(60));
    EXPECT_FALSE(remainingBuy.isOrderFilled());
}

TEST(CMEOrderBookTests, LargerIncomingSellRemainsAfterFillingRestingBuy)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingBuy(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25)
    );

    CMEOrder incomingSell(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(75)
    );

    EXPECT_TRUE(orderBook.addLimitOrder(restingBuy));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingSell));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    const CMEOrder& remainingSell =
        orderBook.getSellLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(remainingSell.getOrderOriginalQuantity(), CMEQuantity(75));
    EXPECT_EQ(remainingSell.getOrderRemainingQuantity(), CMEQuantity(50));
    EXPECT_FALSE(remainingSell.isOrderFilled());
}

TEST(CMEOrderBookTests, PartiallyFilledRestingOrderKeepsFifoPosition)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder firstSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(100)
    );

    CMEOrder secondSell(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(50)
    );

    CMEOrder incomingBuy(
        CMEOrderId(1003),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(40)
    );

    EXPECT_TRUE(orderBook.addLimitOrder(firstSell));
    EXPECT_TRUE(orderBook.addLimitOrder(secondSell));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuy));

    const CMEPriceLevel& level =
        orderBook.getSellLevel(CMEPrice(50000));

    EXPECT_EQ(level.getOrderCount(), 2);
    EXPECT_EQ(level.getFrontOrder().getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(
        level.getFrontOrder().getOrderRemainingQuantity(),
        CMEQuantity(60)
    );
}

// ============================================================================
// TRADE GENERATION TESTS
// ============================================================================

TEST(CMEOrderBookTests, EqualQuantityMatchCreatesTrade)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder incomingBuy(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(restingSell));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuy));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(orderBook.getLastTrade()->getTradeId().value, 1);
    EXPECT_EQ(orderBook.getLastTrade()->getBuyOrderId().value, 1002);
    EXPECT_EQ(orderBook.getLastTrade()->getSellOrderId().value, 1001);
    EXPECT_EQ(orderBook.getLastTrade()->getTradeSymbol().value, "BTC-GBP");
    EXPECT_EQ(orderBook.getLastTrade()->getTradePrice().value, 50000);
    EXPECT_EQ(orderBook.getLastTrade()->getTradeQuantity().value, 25);
}

TEST(CMEOrderBookTests, PartialFillCreatesCorrectTradeQuantity)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(100));

    CMEOrder incomingBuy(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(40));

    EXPECT_TRUE(orderBook.addLimitOrder(restingSell));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuy));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(orderBook.getLastTrade()->getTradeId().value, 1);
    EXPECT_EQ(orderBook.getLastTrade()->getBuyOrderId().value, 1002);
    EXPECT_EQ(orderBook.getLastTrade()->getSellOrderId().value, 1001);
    EXPECT_EQ(orderBook.getLastTrade()->getTradePrice().value, 50000);
    EXPECT_EQ(orderBook.getLastTrade()->getTradeQuantity().value, 40);
}

TEST(CMEOrderBookTests, LargerIncomingOrderCreatesCorrectTrade)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder incomingBuy(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(100));

    EXPECT_TRUE(orderBook.addLimitOrder(restingSell));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuy));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(orderBook.getLastTrade()->getTradeId().value, 1);
    EXPECT_EQ(orderBook.getLastTrade()->getBuyOrderId().value, 1002);
    EXPECT_EQ(orderBook.getLastTrade()->getSellOrderId().value, 1001);
    EXPECT_EQ(orderBook.getLastTrade()->getTradePrice().value, 50000);
    EXPECT_EQ(orderBook.getLastTrade()->getTradeQuantity().value, 25);

    EXPECT_EQ(
        orderBook.getBuyLevel(CMEPrice(50000))
            .getFrontOrder()
            .getOrderRemainingQuantity().value,
        75);
}

TEST(CMEOrderBookTests, TradeIdentifiersIncreaseSequentially)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSellOne(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder incomingBuyOne(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(restingSellOne));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuyOne));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());
    EXPECT_EQ(orderBook.getLastTrade()->getTradeId().value, 1);

    CMEOrder restingSellTwo(
        CMEOrderId(1003),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(51000),
        CMEQuantity(10));

    CMEOrder incomingBuyTwo(
        CMEOrderId(1004),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(51000),
        CMEQuantity(10));

    EXPECT_TRUE(orderBook.addLimitOrder(restingSellTwo));
    EXPECT_TRUE(orderBook.addLimitOrder(incomingBuyTwo));

    ASSERT_TRUE(orderBook.getLastTrade().has_value());
    EXPECT_EQ(orderBook.getLastTrade()->getTradeId().value, 2);
}

// ============================================================================
// ORDER CANCELLATION TESTS
// ============================================================================

TEST(CMEOrderBookTests, CancelsBuyOrder)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);

    EXPECT_TRUE(orderBook.cancelOrder(CMEOrderId(1001)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, CancelsSellOrder)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder sellOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    EXPECT_TRUE(orderBook.cancelOrder(CMEOrderId(1001)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, CancellingUnknownOrderReturnsFalse)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_FALSE(orderBook.cancelOrder(CMEOrderId(9999)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    EXPECT_EQ(
        orderBook.getBuyLevel(CMEPrice(50000)).getOrderCount(),
        1);
}

TEST(CMEOrderBookTests, CancellingOneOrderLeavesOtherOrdersAtSamePrice)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder firstOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder secondOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(50));

    EXPECT_TRUE(orderBook.addLimitOrder(firstOrder));
    EXPECT_TRUE(orderBook.addLimitOrder(secondOrder));

    EXPECT_TRUE(orderBook.cancelOrder(CMEOrderId(1001)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);

    const CMEPriceLevel& level =
        orderBook.getBuyLevel(CMEPrice(50000));

    EXPECT_EQ(level.getOrderCount(), 1);
    EXPECT_EQ(level.getFrontOrder().getOrderId().value, 1002);
    EXPECT_EQ(level.getFrontOrder().getOrderRemainingQuantity().value, 50);
}

TEST(CMEOrderBookTests, CancellingLastOrderRemovesPriceLevel)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder sellOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);

    EXPECT_TRUE(orderBook.cancelOrder(CMEOrderId(1001)));

    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, CancellingOrderAtOnePriceDoesNotRemoveOtherPriceLevels)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder firstOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder secondOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(49000),
        CMEQuantity(50));

    EXPECT_TRUE(orderBook.addLimitOrder(firstOrder));
    EXPECT_TRUE(orderBook.addLimitOrder(secondOrder));

    EXPECT_TRUE(orderBook.cancelOrder(CMEOrderId(1001)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);

    EXPECT_EQ(
        orderBook.getBestBid(),
        CMEPrice(49000));

    EXPECT_EQ(
        orderBook.getBuyLevel(CMEPrice(49000)).getOrderCount(),
        1);

    EXPECT_EQ(
        orderBook.getBuyLevel(CMEPrice(49000))
            .getFrontOrder()
            .getOrderId().value,
        1002);
}

// ============================================================================
// ORDER MODIFICATION TESTS
// ============================================================================
TEST(CMEOrderBookTests, ModifiesBuyOrderPrice)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_TRUE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(51000),
            CMEQuantity(25)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(51000));

    const CMEOrder& modifiedOrder =
        orderBook.getBuyLevel(CMEPrice(51000)).getFrontOrder();

    EXPECT_EQ(modifiedOrder.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(modifiedOrder.getOrderPrice(), CMEPrice(51000));
    EXPECT_EQ(modifiedOrder.getOrderOriginalQuantity(), CMEQuantity(25));
    EXPECT_EQ(modifiedOrder.getOrderRemainingQuantity(), CMEQuantity(25));
}

TEST(CMEOrderBookTests, ModifiesSellOrderPrice)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder sellOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(51000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    EXPECT_TRUE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(50000),
            CMEQuantity(25)));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);
    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(50000));

    const CMEOrder& modifiedOrder =
        orderBook.getSellLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(modifiedOrder.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(modifiedOrder.getOrderPrice(), CMEPrice(50000));
}

TEST(CMEOrderBookTests, ModifiesOrderQuantity)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_TRUE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(50000),
            CMEQuantity(100)));

    const CMEOrder& modifiedOrder =
        orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(modifiedOrder.getOrderOriginalQuantity(), CMEQuantity(100));
    EXPECT_EQ(modifiedOrder.getOrderRemainingQuantity(), CMEQuantity(100));
}

TEST(CMEOrderBookTests, ModifiedOrderLosesFifoPriority)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder firstOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    CMEOrder secondOrder(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(firstOrder));
    EXPECT_TRUE(orderBook.addLimitOrder(secondOrder));

    EXPECT_TRUE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(50000),
            CMEQuantity(50)));

    const CMEPriceLevel& level =
        orderBook.getBuyLevel(CMEPrice(50000));

    EXPECT_EQ(level.getOrderCount(), 2);
    EXPECT_EQ(level.getFrontOrder().getOrderId(), CMEOrderId(1002));
}

TEST(CMEOrderBookTests, ModifyingUnknownOrderReturnsFalse)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    EXPECT_FALSE(
        orderBook.modifyOrder(
            CMEOrderId(9999),
            CMEPrice(50000),
            CMEQuantity(25)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);
}

TEST(CMEOrderBookTests, RejectsModificationWithZeroPrice)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder buyOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(buyOrder));

    EXPECT_FALSE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(0),
            CMEQuantity(25)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 1);
    EXPECT_EQ(orderBook.getBestBid(), CMEPrice(50000));

    const CMEOrder& originalOrder =
        orderBook.getBuyLevel(CMEPrice(50000)).getFrontOrder();

    EXPECT_EQ(originalOrder.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(originalOrder.getOrderPrice(), CMEPrice(50000));
    EXPECT_EQ(originalOrder.getOrderRemainingQuantity(), CMEQuantity(25));
}

TEST(CMEOrderBookTests, RejectsModificationWithZeroQuantity)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder sellOrder(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(51000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(sellOrder));

    EXPECT_FALSE(
        orderBook.modifyOrder(
            CMEOrderId(1001),
            CMEPrice(51000),
            CMEQuantity(0)));

    EXPECT_EQ(orderBook.getSellLevelCount(), 1);
    EXPECT_EQ(orderBook.getBestAsk(), CMEPrice(51000));

    const CMEOrder& originalOrder =
        orderBook.getSellLevel(CMEPrice(51000)).getFrontOrder();

    EXPECT_EQ(originalOrder.getOrderId(), CMEOrderId(1001));
    EXPECT_EQ(originalOrder.getOrderRemainingQuantity(), CMEQuantity(25));
}

TEST(CMEOrderBookTests, ModificationCanImmediatelyMatchOppositeOrder)
{
    CMEOrderBook orderBook(CMESymbol("BTC-GBP"));

    CMEOrder restingSell(
        CMEOrderId(1001),
        CMESymbol("BTC-GBP"),
        CMESide::SELL,
        CMEPrice(51000),
        CMEQuantity(25));

    CMEOrder restingBuy(
        CMEOrderId(1002),
        CMESymbol("BTC-GBP"),
        CMESide::BUY,
        CMEPrice(50000),
        CMEQuantity(25));

    EXPECT_TRUE(orderBook.addLimitOrder(restingSell));
    EXPECT_TRUE(orderBook.addLimitOrder(restingBuy));

    EXPECT_TRUE(
        orderBook.modifyOrder(
            CMEOrderId(1002),
            CMEPrice(51000),
            CMEQuantity(25)));

    EXPECT_EQ(orderBook.getBuyLevelCount(), 0);
    EXPECT_EQ(orderBook.getSellLevelCount(), 0);

    ASSERT_TRUE(orderBook.getLastTrade().has_value());

    EXPECT_EQ(orderBook.getLastTrade()->getBuyOrderId(), CMEOrderId(1002));
    EXPECT_EQ(orderBook.getLastTrade()->getSellOrderId(), CMEOrderId(1001));
    EXPECT_EQ(orderBook.getLastTrade()->getTradePrice(), CMEPrice(51000));
    EXPECT_EQ(orderBook.getLastTrade()->getTradeQuantity(), CMEQuantity(25));
}
#include "OrderBook.h"

void OrderBook::AddSellOrder(int value, double price) {
    orders_sell.push({value, price, Order::Type::sell});
}

void OrderBook::AddBuyOrder(int value, double price) {
    orders_buy.push({value, price, Order::Type::sell});
}

const Order& OrderBook::GetSellOrder() const {
    return orders_sell.top();
}

const Order& OrderBook::GetBuyOrder() const {
    return orders_buy.top();
}
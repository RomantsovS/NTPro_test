#include "OrderBook.h"

void OrderBook::AddBuyOrder(std::shared_ptr<Order> order) {
    orders_buy.push(order);
}

void OrderBook::AddSellOrder(std::shared_ptr<Order> order) {
    orders_sell.push(order);
}

std::optional<std::shared_ptr<Order>> OrderBook::GetBuyOrder() {
    if (!orders_buy.empty()) return orders_buy.top();

    return std::nullopt;
}

std::optional<std::shared_ptr<Order>> OrderBook::GetSellOrder() {
    if (!orders_sell.empty()) return orders_sell.top();
    return std::nullopt;
}

std::optional<OrdersPair> OrderBook::MakeDeal() {
    if (orders_buy.empty() || orders_sell.empty()) return std::nullopt;

    std::shared_ptr<Order> order_buy = orders_buy.top();
    std::shared_ptr<Order> order_sell = orders_sell.top();

    if (order_buy->GetPrice() < order_sell->GetPrice()) return std::nullopt;

    orders_buy.pop();
    orders_sell.pop();

    return OrdersPair{ order_buy, order_sell };
}
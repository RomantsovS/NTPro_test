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
    if (!orders_buy.empty()) return orders_sell.top();
    return std::nullopt;
}

std::optional<Deal> OrderBook::MakeDeal() {
    if (orders_buy.empty() || orders_sell.empty()) return std::nullopt;

    std::shared_ptr<Order> order_buy = orders_buy.top();
    std::shared_ptr<Order> order_sell = orders_sell.top();

    if (order_buy->GetPrice() < order_sell->GetPrice()) return std::nullopt;

    orders_buy.pop();
    orders_sell.pop();

    int min_amount = std::min(order_buy->GetAmount(), order_sell->GetAmount());

    order_buy->SetAmount(order_buy->GetAmount() - min_amount);
    order_sell->SetAmount(order_sell->GetAmount() - min_amount);

    if (order_buy->GetAmount() > 0) orders_buy.push(order_buy);
    if (order_sell->GetAmount() > 0) orders_sell.push(order_sell);

    return Deal(min_amount, order_sell->GetPrice(), order_buy->GetClientID(), order_buy->GetID(),
                order_sell->GetClientID(), order_sell->GetID(), std::chrono::system_clock::now());
}
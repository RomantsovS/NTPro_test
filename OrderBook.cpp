#include "OrderBook.h"

void OrderBook::AddBuyOrder(int value, double price, client_id_type client_id) {
	orders_buy.push({ value, price, Order::Type::buy, client_id,  std::chrono::system_clock::now() });
}

void OrderBook::AddSellOrder(int value, double price, client_id_type client_id) {
	orders_sell.push({ value, price, Order::Type::sell, client_id,  std::chrono::system_clock::now() });
}

const std::optional<std::reference_wrapper<const Order>> OrderBook::GetBuyOrder() const {
	if (!orders_buy.empty())
		return orders_buy.top();
	return std::nullopt;
}

const std::optional<std::reference_wrapper<const Order>> OrderBook::GetSellOrder() const {
	if (!orders_buy.empty())
		return orders_sell.top();
	return std::nullopt;
}

std::optional<Deal> OrderBook::MakeDeal() {
	if (orders_buy.empty() || orders_sell.empty())
		return std::nullopt;

	Order order_buy = orders_buy.top();
	Order order_sell = orders_sell.top();

	if (order_buy.GetPrice() < order_sell.GetPrice())
		return std::nullopt;

	orders_buy.pop();
	orders_sell.pop();

	int min_amount = std::min(order_buy.GetAmount(), order_sell.GetAmount());

	order_buy.SetAmount(order_buy.GetAmount() - min_amount);
	order_sell.SetAmount(order_sell.GetAmount() - min_amount);

	if (order_buy.GetAmount() > 0)
		orders_buy.push(order_buy);
	if (order_sell.GetAmount() > 0)
		orders_sell.push(order_sell);

	return Deal(min_amount, order_sell.GetPrice(), order_buy.GetClientID(), order_sell.GetClientID(), std::chrono::system_clock::now());
}
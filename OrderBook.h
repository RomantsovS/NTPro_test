#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <queue>
#include <optional>

#include "Order.h"
#include "Deal.h"

class OrderBook {
public:
	void AddBuyOrder(int value, double price, client_id_type client_id);
	void AddSellOrder(int value, double price, client_id_type client_id);

	const std::optional<std::reference_wrapper<const Order>> GetBuyOrder() const;
	const std::optional<std::reference_wrapper<const Order>> GetSellOrder() const;

	std::optional<Deal> MakeDeal();
private:
	struct {
		bool operator()(const Order& lhs, const Order& rhs) const {
			return lhs.GetPrice() < rhs.GetPrice();
		}
	} cmp_buy;

	std::priority_queue<Order, std::vector<Order>, decltype(cmp_buy)> orders_buy;

	struct {
		bool operator()(const Order& lhs, const Order& rhs) const {
			return lhs.GetPrice() > rhs.GetPrice();
		}
	} cmp_sell;

	std::priority_queue<Order, std::vector<Order>, decltype(cmp_sell)> orders_sell;
};

#endif
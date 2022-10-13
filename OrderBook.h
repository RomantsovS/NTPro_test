#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <queue>

#include "Order.h"

class OrderBook {
   public:
    void AddSellOrder(int value, double price);
    void AddBuyOrder(int value, double price);

    const Order& GetSellOrder() const;
    const Order& GetBuyOrder() const;

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
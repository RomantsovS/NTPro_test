#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <functional>
#include <optional>
#include <queue>

#include "Deal.h"
#include "Order.h"

class OrderBook {
   public:
    void AddBuyOrder(int value, double price, client_id_type client_id);
    void AddSellOrder(int value, double price, client_id_type client_id);

    std::optional<std::reference_wrapper<const Order>> GetBuyOrder();
    std::optional<std::reference_wrapper<const Order>> GetSellOrder();

    std::optional<Deal> MakeDeal();

   private:
    struct cmp_buy {
        bool operator()(const Order& lhs, const Order& rhs) const {
            return lhs.GetPrice() < rhs.GetPrice();
        }
    };

    std::priority_queue<Order, std::vector<Order>, cmp_buy> orders_buy;

    struct cmp_sell {
        bool operator()(const Order& lhs, const Order& rhs) const {
            return lhs.GetPrice() > rhs.GetPrice();
        }
    };

    std::priority_queue<Order, std::vector<Order>, cmp_sell> orders_sell;
};

#endif
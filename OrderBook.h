#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <functional>
#include <optional>
#include <queue>
#include <memory>
#include <map>
#include <tuple>

#include "Deal.h"
#include "Order.h"

struct OrdersPair {
    std::shared_ptr<Order> buy, sell;
};

class OrderBook {
   public:
    void AddBuyOrder(std::shared_ptr<Order> order);
    void AddSellOrder(std::shared_ptr<Order> order);

    std::optional<std::shared_ptr<Order>> GetBuyOrder();
    std::optional<std::shared_ptr<Order>> GetSellOrder();

    std::optional<OrdersPair> MakeDeal();

   private:
    struct cmp_buy {
        bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) const {
            return std::make_tuple(lhs->GetPrice(), rhs->GetTime()) < std::make_tuple(rhs->GetPrice(), lhs->GetTime());
        }
    };

    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, cmp_buy> orders_buy;

    struct cmp_sell {
        bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) const {
            return std::make_tuple(lhs->GetPrice(), lhs->GetTime()) > std::make_tuple(rhs->GetPrice(), rhs->GetTime());
        }
    };

    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, cmp_sell> orders_sell;
};

#endif
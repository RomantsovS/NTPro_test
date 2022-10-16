#ifndef ORDER_BOOK_H
#define ORDER_BOOK_H

#include <functional>
#include <optional>
#include <queue>
#include <memory>
#include <map>

#include "Deal.h"
#include "Order.h"

class OrderBook {
   public:
    void AddBuyOrder(std::shared_ptr<Order> order);
    void AddSellOrder(std::shared_ptr<Order> order);

    std::optional<std::shared_ptr<Order>> GetBuyOrder();
    std::optional<std::shared_ptr<Order>> GetSellOrder();

    std::optional<Deal> MakeDeal();

   private:
    struct cmp_buy {
        bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) const {
            return lhs->GetPrice() < rhs->GetPrice();
        }
    };

    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, cmp_buy> orders_buy;

    struct cmp_sell {
        bool operator()(const std::shared_ptr<Order>& lhs, const std::shared_ptr<Order>& rhs) const {
            return lhs->GetPrice() > rhs->GetPrice();
        }
    };

    std::priority_queue<std::shared_ptr<Order>, std::vector<std::shared_ptr<Order>>, cmp_sell> orders_sell;
};

#endif
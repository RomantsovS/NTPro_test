#ifndef DEAL_H
#define DEAL_H

#include <chrono>
#include <string>

using client_id_type = std::string;

class Deal {
   public:
    Deal(int amount, double price, client_id_type order_buy_client_id, size_t order_buy_id,
         client_id_type order_sell_client_id, size_t order_sell_id,
         std::chrono::time_point<std::chrono::system_clock> time)
        : amount_(amount),
          price_(price),
          order_buy_client_id_(order_buy_client_id),
          order_sell_client_id_(order_sell_client_id),
          order_buy_id_(order_buy_id),
          order_sell_id_(order_sell_id),
          time_(time) {}

    int GetAmount() const { return amount_; }
    double GetPrice() const { return price_; }
    client_id_type GetClientBuyID() const { return order_buy_client_id_; }
    client_id_type GetClientSellID() const { return order_sell_client_id_; }
    size_t GetOrderBuyID() const { return order_buy_id_; }
    size_t GetOrderSellID() const { return order_sell_id_; }

   private:
    int amount_;
    double price_;
    client_id_type order_buy_client_id_;
    size_t order_buy_id_;
    client_id_type order_sell_client_id_;
    size_t order_sell_id_;
    std::chrono::time_point<std::chrono::system_clock> time_;
};

#endif
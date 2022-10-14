#ifndef DEAL_H
#define DEAL_H

#include <chrono>

using client_id_type = size_t;

class Deal {
   public:
    Deal(int amount, double price, client_id_type order_buy_client_id,
         client_id_type order_sell_client_id,
         std::chrono::time_point<std::chrono::system_clock> time)
        : amount_(amount),
          price_(price),
          order_buy_client_id_(order_buy_client_id),
          order_sell_client_id_(order_sell_client_id),
          time_(time) {}

    int GetAmount() const { return amount_; }
    double GetPrice() const { return price_; }
    client_id_type GetClientBuyID() const { return order_buy_client_id_; }
    client_id_type GetClientSellID() const { return order_sell_client_id_; }

   private:
    int amount_;
    double price_;
    client_id_type order_buy_client_id_;
    client_id_type order_sell_client_id_;
    std::chrono::time_point<std::chrono::system_clock> time_;
};

#endif
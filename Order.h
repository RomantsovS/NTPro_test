#ifndef ORDER_H
#define ORDER_H

#include <chrono>

using client_id_type = size_t;

class Order {
   public:
    enum class Type { buy, sell };

    Order() = default;
    Order(int amount, double price, Type type, client_id_type client_id,
          std::chrono::time_point<std::chrono::system_clock> time)
        : amount_(amount), price_(price), type_(type), client_id_(client_id), time_(time) {}

    double GetPrice() const { return price_; }
    int GetAmount() const { return amount_; }
    void SetAmount(int amount) { amount_ = amount; }
    client_id_type GetClientID() const { return client_id_; }

   private:
    int amount_;
    double price_;
    Type type_;
    client_id_type client_id_;
    std::chrono::time_point<std::chrono::system_clock> time_;
};

#endif
#include "Core.h"

#include "json.hpp"

using nlohmann::json;

Core& GetCore() {
    static Core core;
    return core;
}

client_id_type Core::RegisterNewUser(const std::string& aUserName) {
    client_id_type newUserId = std::to_string(mUsers.size());
    mUsers[newUserId] = User(newUserId, aUserName);

    return newUserId;
}

std::optional<std::reference_wrapper<const User>> Core::GetUser(client_id_type aUserId) {
    if (mUsers.count(aUserId) == 0) return std::nullopt;

    return mUsers.at(aUserId);
}

std::string Core::AddBuyOrder(int amount, double price, client_id_type client_id, std::chrono::time_point<std::chrono::system_clock> time) {
    auto order = std::make_shared<Order>(amount, price, Order::Type::buy, client_id, time, order_id);
    order_book.AddBuyOrder(order);
    client_orders[client_id][order_id++] = order;

    return "added OK\n";
}

std::string Core::AddSellOrder(int amount, double price, client_id_type client_id, std::chrono::time_point<std::chrono::system_clock> time) {
    auto order = std::make_shared<Order>(amount, price, Order::Type::sell, client_id, time, order_id);
    order_book.AddSellOrder(order);
    client_orders[client_id][order_id++] = order;

    return "added OK\n";
}

std::string Core::GetOrders(client_id_type client_id) const {
    json msg = json::array();
    if (client_orders.count(client_id) == 0)
        return msg.dump();

    for (auto& sp : client_orders.at(client_id)) {
        msg.push_back(json::object({ {"type", static_cast<int>(sp.second->GetType())}, {"amount", sp.second->GetAmount()}, {"price", sp.second->GetPrice()},
            {"time", std::chrono::duration_cast<std::chrono::microseconds>(
                   sp.second->GetTime().time_since_epoch()).count()}}));
    }

    return msg.dump();
}

void Core::MakeDeal() {
    auto deal = order_book.MakeDeal();
    if (!deal) return;

    mUsers[deal->GetClientBuyID()].IncreaseBalance(Currencies::USD, deal->GetAmount());
    mUsers[deal->GetClientSellID()].DecreaseBalance(Currencies::USD, deal->GetAmount());

    double val = deal->GetAmount() * deal->GetPrice();

    mUsers[deal->GetClientBuyID()].DecreaseBalance(Currencies::RUB, val);
    mUsers[deal->GetClientSellID()].IncreaseBalance(Currencies::RUB, val);

    client_orders[deal->GetClientBuyID()].erase(deal->GetOrderBuyID());
    client_orders[deal->GetClientSellID()].erase(deal->GetOrderSellID());

    deals.push_back(std::move(deal.value()));
}
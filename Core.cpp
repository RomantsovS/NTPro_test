#include "Core.h"

Core& GetCore() {
    static Core core;
    return core;
}

client_id_type Core::RegisterNewUser(const std::string& aUserName) {
    client_id_type newUserId = mUsers.size();
    mUsers[newUserId] = User(newUserId, aUserName);

    return newUserId;
}

// Запрос имени клиента по ID
const std::optional<std::reference_wrapper<const User>> Core::GetUser(client_id_type aUserId) {
    if (mUsers.count(aUserId) == 0) {
        return std::nullopt;
    }
    else {
        return mUsers[aUserId];
    }
}

void Core::AddBuyOrder(int value, double price, client_id_type client_id) {
    order_book.AddBuyOrder(value, price, client_id);
}

void Core::AddSellOrder(int value, double price, client_id_type client_id) {
    order_book.AddSellOrder(value, price, client_id);
}

void Core::MakeDeal() {
	auto deal = order_book.MakeDeal();
    if (!deal)
        return;
        
    mUsers[deal->GetClientBuyID()].IncreaseBalance(Currencies::USD, deal->GetAmount());
    mUsers[deal->GetClientSellID()].DecreaseBalance(Currencies::USD, deal->GetAmount());

    double val = deal->GetAmount() * deal->GetPrice();

    mUsers[deal->GetClientBuyID()].DecreaseBalance(Currencies::RUB, val);
    mUsers[deal->GetClientSellID()].IncreaseBalance(Currencies::RUB, val);

    deals.push_back(std::move(deal.value()));
}
#ifndef CORE_H
#define CORE_H

#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "OrderBook.h"
#include "User.h"

class Core {
   public:
    // "Регистрирует" нового пользователя и возвращает его ID.
    client_id_type RegisterNewUser(const std::string& aUserName);

    // Запрос клиента по ID
	std::optional<std::reference_wrapper<const User>> GetUser(client_id_type aUserId);

    void AddBuyOrder(int value, double price, client_id_type client_id);
    void AddSellOrder(int value, double price, client_id_type client_id);

    void MakeDeal();

    const std::vector<Deal> GetDeals() const { return deals; }

   private:
    // <UserId, UserName>
    std::map<client_id_type, User> mUsers;
    OrderBook order_book;
    std::vector<Deal> deals;
};

Core& GetCore();

#endif
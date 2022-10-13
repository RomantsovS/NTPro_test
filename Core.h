#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <map>
#include <string>

#include "OrderBook.h"

class Core {
   public:
    // "Регистрирует" нового пользователя и возвращает его ID.
    std::string RegisterNewUser(const std::string& aUserName) {
        size_t newUserId = mUsers.size();
        mUsers[newUserId] = aUserName;

        return std::to_string(newUserId);
    }

    // Запрос имени клиента по ID
    std::string GetUserName(const std::string& aUserId) {
        const auto userIt = mUsers.find(std::stoi(aUserId));
        if (userIt == mUsers.cend()) {
            return "Error! Unknown User";
        } else {
            return userIt->second;
        }
    }

   private:
    // <UserId, UserName>
    std::map<size_t, std::string> mUsers;
    OrderBook order_book;
};

Core& GetCore() {
    static Core core;
    return core;
}

#endif
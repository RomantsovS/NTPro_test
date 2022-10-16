#ifndef CORE_H
#define CORE_H

#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <set>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include "OrderBook.h"
#include "User.h"

class Core {
   public:
       Core(bool run_thread = true);
       ~Core();

       void Stop();
    // "Регистрирует" нового пользователя и возвращает его ID.
    client_id_type RegisterNewUser(const std::string& aUserName);

    // Запрос клиента по ID
	std::optional<std::reference_wrapper<const User>> GetUser(client_id_type aUserId);

    std::string AddBuyOrder(int amount, double price, client_id_type client_id, std::chrono::time_point<std::chrono::system_clock> time);
    std::string AddSellOrder(int amount, double price, client_id_type client_id, std::chrono::time_point<std::chrono::system_clock> time);

    std::string GetOrders(client_id_type client_id) const;

    bool MakeDeal();

    const std::vector<std::shared_ptr<Deal>> GetDeals() const { return deals; }
    std::string GetClientDeals(client_id_type client_id) const;
    std::string GetClientBalance(client_id_type client_id) const;

    void SignalWork();

   private:
       void Do_work();

    // <UserId, UserName>
    std::map<std::string, User> mUsers;
    OrderBook order_book;
    size_t order_id = 0;
    std::map<client_id_type, std::map<size_t, std::shared_ptr<Order>>> client_orders;
    std::vector<std::shared_ptr<Deal>> deals;
    std::map<client_id_type, std::vector<std::shared_ptr<Deal>>> client_deals;
    
    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stopped_;
    bool moreWorkToDo;
    std::mutex moreWorkToDoMutex;
};

Core& GetCore();

#endif
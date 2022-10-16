#include "Core.h"

#include "json.hpp"

using nlohmann::json;

Core& GetCore() {
	static Core core;
	return core;
}

Core::Core(bool run_thread) : stopped_(!run_thread), moreWorkToDo(false) {
	if(run_thread)
		thread_ = std::thread(&Core::Do_work, this);
}

Core::~Core() {
	Stop();
}

void Core::Stop() {
	if (!stopped_) {
		stopped_ = true;
		cv_.notify_one();
		thread_.join();
	}
	else {
		// throw std::runtime_error("already stopped");
	}
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

	SignalWork();

	return "added OK\n";
}

std::string Core::AddSellOrder(int amount, double price, client_id_type client_id, std::chrono::time_point<std::chrono::system_clock> time) {
	auto order = std::make_shared<Order>(amount, price, Order::Type::sell, client_id, time, order_id);
	order_book.AddSellOrder(order);
	client_orders[client_id][order_id++] = order;

	SignalWork();

	return "added OK\n";
}

std::string Core::GetOrders(client_id_type client_id) const {
	json msg = json::array();
	if (client_orders.count(client_id) == 0)
		return msg.dump();

	for (auto& sp : client_orders.at(client_id)) {
		msg.push_back(json::object({ {"type", static_cast<int>(sp.second->GetType())}, {"amount", sp.second->GetAmount()}, {"price", sp.second->GetPrice()},
			{"time", std::chrono::duration_cast<std::chrono::microseconds>(
				   sp.second->GetTime().time_since_epoch()).count()} }));
	}

	return msg.dump();
}

bool Core::MakeDeal() {
	int min_amount;
	std::optional<OrdersPair> orders_pair;

	{
		std::lock_guard lg(mutex_);

		orders_pair = order_book.MakeDeal();
		if (!orders_pair) return false;

		min_amount = std::min(orders_pair->buy->GetAmount(), orders_pair->sell->GetAmount());

		orders_pair->buy->SetAmount(orders_pair->buy->GetAmount() - min_amount);
		orders_pair->sell->SetAmount(orders_pair->sell->GetAmount() - min_amount);

		if (orders_pair->buy->GetAmount() > 0) order_book.AddBuyOrder(orders_pair->buy);
		else client_orders[orders_pair->buy->GetClientID()].erase(orders_pair->buy->GetID());
		if (orders_pair->sell->GetAmount() > 0) order_book.AddSellOrder(orders_pair->sell);
		else client_orders[orders_pair->sell->GetClientID()].erase(orders_pair->sell->GetID());

		mUsers[orders_pair->buy->GetClientID()].IncreaseBalance(Currencies::USD, min_amount);
		mUsers[orders_pair->sell->GetClientID()].DecreaseBalance(Currencies::USD, min_amount);

		double val = min_amount * orders_pair->sell->GetPrice();

		mUsers[orders_pair->buy->GetClientID()].DecreaseBalance(Currencies::RUB, val);
		mUsers[orders_pair->sell->GetClientID()].IncreaseBalance(Currencies::RUB, val);
	}

	deals.push_back(std::make_shared<Deal>(min_amount, orders_pair->sell->GetPrice(), orders_pair->buy->GetClientID(), orders_pair->buy->GetID(),
		orders_pair->sell->GetClientID(), orders_pair->sell->GetID(), std::chrono::system_clock::now()));

	client_deals[orders_pair->buy->GetClientID()].push_back(deals.back());
	client_deals[orders_pair->sell->GetClientID()].push_back(deals.back());

	return true;
}

std::string Core::GetClientDeals(client_id_type client_id) const {
	json msg = json::array();
	if (client_deals.count(client_id) == 0)
		return msg.dump();

	for (auto& deal : client_deals.at(client_id)) {
		msg.push_back(json::object({ {"type", static_cast<int>(deal->GetClientBuyID() == client_id ? Order::Type::buy : Order::Type::sell)},
			{"amount", deal->GetAmount()}, {"price", deal->GetPrice()},
			{"time", std::chrono::duration_cast<std::chrono::microseconds>(deal->GetTime().time_since_epoch()).count()} }));
	}

	return msg.dump();
}

std::string Core::GetClientBalance(client_id_type client_id) const {
	json msg;
	if (mUsers.count(client_id) == 0)
		return msg.dump();

	auto& user = mUsers.at(client_id);
	msg["USD"] = user.GetBalance(Currencies::USD);
	msg["RUB"] = user.GetBalance(Currencies::RUB);

	return msg.dump();
}

void Core::SignalWork() {
	std::lock_guard lg(moreWorkToDoMutex);
	moreWorkToDo = true;
	cv_.notify_one();
}

void Core::Do_work() {
	while (true) {
		std::unique_lock<std::mutex> lock(moreWorkToDoMutex);
		cv_.wait(lock, [this] { return stopped_ || moreWorkToDo; });

		if (stopped_ && !moreWorkToDo)
			return;

		while (MakeDeal()) {}
		moreWorkToDo = false;
		//std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
}
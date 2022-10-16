#ifndef ORDER_H
#define ORDER_H

#include <chrono>
#include <string>
#include <iostream>

using client_id_type = std::string;

class Order {
public:
	enum class Type { buy = 1, sell = 2 };

	Order() = default;
	Order(int amount, double price, Type type, client_id_type client_id,
		std::chrono::time_point<std::chrono::system_clock> time, size_t id)
		: amount_(amount), price_(price), type_(type), client_id_(client_id), time_(time), id_(id) {}
	~Order() { std::cout << "Order dtor\n"; }

		int GetAmount() const { return amount_; }
		void SetAmount(int amount) { amount_ = amount; }

		double GetPrice() const { return price_; }
		Type GetType() const { return type_; }

		client_id_type GetClientID() const { return client_id_; }
		std::chrono::time_point<std::chrono::system_clock> GetTime() const { return time_; }

		size_t GetID() const { return id_; }

	private:
		int amount_;
		double price_;
		Type type_;
		client_id_type client_id_;
		std::chrono::time_point<std::chrono::system_clock> time_;
		size_t id_;
	};

#endif
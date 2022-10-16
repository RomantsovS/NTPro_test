#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <memory>
#include <string_view>
#include <string>
#include <array>

#include "Common.hpp"
#include "Core.h"
#include "json.hpp"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(tcp::socket socket) : socket_(std::move(socket)) {
		//std::cout << "Session ctor\n";
	}
	//~Session() { std::cout << "Session dtor\n"; }

	void start() { do_read(); }

private:
	void do_read() {
		auto self(shared_from_this());
		//std::cout << "Session self->use_count(): " << self.use_count() << std::endl;

		socket_.async_read_some(boost::asio::buffer(data_, max_length), [this, self](boost::system::error_code ec, std::size_t length) {
			if (!ec) {
				std::cout << "receive " << length << "=" << std::string{ data_.data(), length} << std::endl;
				data_[length] = '\0';

				// Парсим json, который пришёл нам в сообщении.
				const auto j = nlohmann::json::parse(data_);
				const auto& reqType = j["ReqType"];

				std::string reply = "Error! Unknown request type";
				if (reqType == Requests::Registration) {
					// Это реквест на регистрацию пользователя.
					// Добавляем нового пользователя и возвращаем его ID.
					reply = GetCore().RegisterNewUser(j["Message"]);
				}
				else if (reqType == Requests::Hello) {
					// Это реквест на приветствие.
					// Находим имя пользователя по ID и приветствуем его по имени.
					const auto user = GetCore().GetUser(j["UserId"]);
					if (user)
						reply = "Hello, " + user->get().GetName() + "!\n";
				}
				else if (reqType == Requests::AddOrder) {
					auto msg = nlohmann::json::parse(std::string(j["Message"]));
					if(msg["type"] == Requests::OrderType::Buy)
						reply = GetCore().AddBuyOrder(msg["amount"], msg["price"], j["UserId"], std::chrono::system_clock::now());
					else
						reply = GetCore().AddSellOrder(msg["amount"], msg["price"], j["UserId"], std::chrono::system_clock::now());
				}
				else if (reqType == Requests::GetOrders) {
					reply = GetCore().GetOrders(j["UserId"]);
				}
				else if (reqType == Requests::GetClientDeals) {
					reply = GetCore().GetClientDeals(j["UserId"]);
				}

				do_write(reply);
			}
		});
	}

	void do_write(std::string_view reply) {
		auto self(shared_from_this());
		//std::cout << "Session self->use_count(): " << self.use_count() << std::endl;

		boost::asio::async_write(
			socket_, boost::asio::buffer(reply, reply.size()),
			[this, self](boost::system::error_code ec, [[maybe_unused]] std::size_t length) {
				if (!ec) {
					do_read();
				}
			});
	}

private:
	tcp::socket socket_;
	enum { max_length = 1024 };
	std::array<char, max_length> data_;
};

#endif
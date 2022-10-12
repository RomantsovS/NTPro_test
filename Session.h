#ifndef SESSION_H
#define SESSION_H

#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "Common.hpp"
#include "Core.h"
#include "json.hpp"

using boost::asio::ip::tcp;

class Session {
   public:
    Session(boost::asio::io_context& io_context) : socket_(io_context) {}

    tcp::socket& socket() { return socket_; }

    void start() {
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            boost::bind(&Session::handle_read, this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    // Обработка полученного сообщения.
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
        if (!error) {
            data_[bytes_transferred] = '\0';

            // Парсим json, который пришёл нам в сообщении.
            auto j = nlohmann::json::parse(data_);
            auto reqType = j["ReqType"];

            std::string reply = "Error! Unknown request type";
            if (reqType == Requests::Registration) {
                // Это реквест на регистрацию пользователя.
                // Добавляем нового пользователя и возвращаем его ID.
                reply = GetCore().RegisterNewUser(j["Message"]);
            } else if (reqType == Requests::Hello) {
                // Это реквест на приветствие.
                // Находим имя пользователя по ID и приветствуем его по имени.
                reply = "Hello, " + GetCore().GetUserName(j["UserId"]) + "!\n";
            }

            boost::asio::async_write(
                socket_, boost::asio::buffer(reply, reply.size()),
                boost::bind(&Session::handle_write, this, boost::asio::placeholders::error));
        } else {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error) {
        if (!error) {
            socket_.async_read_some(
                boost::asio::buffer(data_, max_length),
                boost::bind(&Session::handle_read, this, boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
        } else {
            delete this;
        }
    }

   private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

#endif
#ifndef SESSION_H
#define SESSION_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <memory>
#include <string_view>

#include "Common.hpp"
#include "Core.h"
#include "json.hpp"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
   public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {
        std::cout << "Session ctor\n";
    }
    ~Session() { std::cout << "Session dtor\n"; }

    void start() { do_read(); }

   private:
    void do_read() {
        auto self(shared_from_this());
        std::cout << "Session self->use_count(): " << self.use_count() << std::endl;

        socket_.async_read_some(boost::asio::buffer(data_, max_length), [this, self](
                                                                            boost::system::
                                                                                error_code ec,
                                                                            std::size_t length) {
            if (!ec) {
                std::cout << "receive " << length << "=" << std::string{data_, length} << std::endl;
                data_[length] = '\0';

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

                do_write(reply);
            }
        });
    }

    void do_write(std::string_view reply) {
        auto self(shared_from_this());
        std::cout << "Session self->use_count(): " << self.use_count() << std::endl;
        
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
    char data_[max_length];
};

#endif
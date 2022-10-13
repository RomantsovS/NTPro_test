#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <string>

#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;

class Client {
   public:
    Client(boost::asio::io_context& io_context, const std::string& host, short port)
        : resolver_(io_context), socket_(io_context) {
        tcp::resolver::query query(tcp::v4(), host, std::to_string(port));
        tcp::resolver::iterator iterator = resolver_.resolve(query);

        socket_.connect(*iterator);
    }

    // Отправка сообщения на сервер по шаблону.
    void SendMessage(const std::string& aId, const std::string& aRequestType,
                     const std::string& aMessage) {
        nlohmann::json req;
        req["UserId"] = aId;
        req["ReqType"] = aRequestType;
        req["Message"] = aMessage;

        std::string request = req.dump();
        boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
    }

    // Возвращает строку с ответом сервера на последний запрос.
    std::string ReadMessage() {
        boost::asio::streambuf b;
        boost::asio::read_until(socket_, b, "\0");
        std::istream is(&b);
        std::string line(std::istreambuf_iterator<char>(is), {});
        return line;
    }

    // "Создаём" пользователя, получаем его ID.
    std::string ProcessRegistration(std::istream& is) {
        std::string name;
        std::cout << "Hello! Enter your name: ";
        is >> name;

        // Для регистрации Id не нужен, заполним его нулём
        SendMessage("0", Requests::Registration, name);
        return ReadMessage();
    }

   private:
    tcp::resolver resolver_;
    tcp::socket socket_;
};

#endif
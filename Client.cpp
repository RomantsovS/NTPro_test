#include <iostream>

#include "Client.h"
#include "Common.hpp"
#include "json.hpp"

using boost::asio::ip::tcp;
using nlohmann::json;

Client::Client(boost::asio::io_context& io_context, const std::string& host, short port)
    : resolver_(io_context), socket_(io_context) {
    tcp::resolver::query query(tcp::v4(), host, std::to_string(port));
    tcp::resolver::iterator iterator = resolver_.resolve(query);

    socket_.connect(*iterator);
}

void Client::SendMessageToServer(const std::string& aId, const std::string& aRequestType,
    const std::string& aMessage) {
    nlohmann::json req;
    req["UserId"] = aId;
    req["ReqType"] = aRequestType;
    req["Message"] = aMessage;

    std::string request = req.dump();
    boost::asio::write(socket_, boost::asio::buffer(request, request.size()));
}

std::string Client::ReadMessage() {
    boost::asio::streambuf b;
    boost::asio::read_until(socket_, b, "\0");
    std::istream is(&b);
    std::string line(std::istreambuf_iterator<char>(is), {});
    return line;
}

void Client::SendHello() {
    SendMessageToServer(id_, Requests::Hello, "");
}

void Client::ProcessRegistration(std::istream& is) {
    std::string name;
    std::cout << "Hello! Enter your name: ";
    is >> name;

    SendMessageToServer("", Requests::Registration, name);
    id_ = ReadMessage();
}

void Client::AddOrder() {
    json req, msg;

    int t;
    do {
        std::cout << "Enter order type:\n1) Buy\n2) Sell\n";
        std::cin >> t;
    } while (t <= 0 || t >= static_cast<int>(Requests::OrderType::Max));
    msg["type"] = t;

    std::cout << "Enter amount:\n";
    std::cin >> t;
    msg["amount"] = t;

    std::cout << "Enter price:\n";
    std::cin >> t;
    msg["price"] = t;

    SendMessageToServer(id_, Requests::AddOrder, msg.dump());
    std::cout << ReadMessage() << std::endl;
}

void Client::GetOrders() {
    SendMessageToServer(id_, Requests::GetOrders, "");
    auto orders = json::parse(ReadMessage());

    if (orders.empty()) {
        std::cout << "No orders\n";
        return;
    }

    for (const auto& order : orders) {
        auto i = order["type"].get<int>();

        std::chrono::microseconds dur(order["time"].get<size_t>());
        std::chrono::time_point<std::chrono::system_clock> dt(dur);
        auto tt = std::chrono::system_clock::to_time_t(dt);

        std::cout << "type: " << Requests::OrderTypeString.at(static_cast<Requests::OrderType>(i))
            << " amount: " << order["amount"] << " price: " << order["price"]
            << " time: " << std::put_time(std::localtime(&tt), "%c") << '\n';
    }
}
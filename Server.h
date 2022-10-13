#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <memory>

#include "Common.hpp"
#include "Session.h"

using boost::asio::ip::tcp;

class Server {
   public:
    Server(boost::asio::io_context& io_context)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), common::port)) {
        std::cout << "Server started! Listen " << common::port << " port" << std::endl;
        do_accept();
    }

   private:
    void do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(std::move(socket))->start();
            }

            do_accept();
        });
    }

   private:
    tcp::acceptor acceptor_;
};

#endif
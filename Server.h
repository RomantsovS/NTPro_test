#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "Common.hpp"
#include "Session.h"

using boost::asio::ip::tcp;

class Server {
   public:
    Server(boost::asio::io_context& io_context)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "Server started! Listen " << port << " port" << std::endl;

        Session* new_session = new Session(io_context_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&Server::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(Session* new_session, const boost::system::error_code& error) {
        if (!error) {
            new_session->start();
            new_session = new Session(io_context_);
            acceptor_.async_accept(new_session->socket(),
                                   boost::bind(&Server::handle_accept, this, new_session,
                                               boost::asio::placeholders::error));
        } else {
            delete new_session;
        }
    }

   private:
    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

#endif
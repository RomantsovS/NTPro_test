#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

#include "Server.h"

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        Server s(io_context);

        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
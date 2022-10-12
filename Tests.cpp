#include <gtest/gtest.h>

#include <sstream>
#include <thread>

#include "Server.h"
#include "Client.h"

TEST(TestAll, Test1) {
    std::ostringstream os;
    const std::string expected = R"(0)";

    boost::asio::io_context io_context;

    Server server(io_context);

    std::thread([&io_context] { io_context.run(); }).detach();

    std::thread([&os] {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        boost::asio::io_context io_context_client;

        tcp::resolver resolver(io_context_client);
        tcp::resolver::query query(tcp::v4(), "127.0.0.1", std::to_string(port));
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_context_client);
        s.connect(*iterator);

        Client client;

        std::string my_id = client.ProcessRegistration(s);
        os << my_id;
    }).join();

    // std::this_thread::sleep_for(std::chrono::seconds(3));

    EXPECT_EQ(os.str(), expected);
}
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

        Client client(io_context_client, "127.0.0.1", common::port);

        std::string my_id = client.ProcessRegistration();
        os << my_id;
    }).join();

    EXPECT_EQ(os.str(), expected);
}
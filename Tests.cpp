#include <gtest/gtest.h>

#include <sstream>
#include <thread>

#include "Client.h"
#include "Server.h"

// TEST(TestAll, Test1) {
//     std::ostringstream os;
//     const std::string expected = R"(0)";

//     boost::asio::io_context io_context;

//     Server server(io_context);

//     std::thread([&io_context] { io_context.run(); }).detach();

//     std::thread([&os] {
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//         boost::asio::io_context io_context_client;

//         Client client(io_context_client, "127.0.0.1", common::port);

//         std::string my_id = client.ProcessRegistration();
//         os << my_id;
//     }).join();

//     EXPECT_EQ(os.str(), expected);
// }

TEST(TestOrderBook, TestAddSell) {
    std::ostringstream os;

    OrderBook order_book;

    order_book.AddBuyOrder(10, 62);

    {
        const Order& order_buy = order_book.GetBuyOrder();
        EXPECT_EQ(order_buy.GetPrice(), 62);
    }

    order_book.AddBuyOrder(20, 63);

    {
        const Order& order_buy = order_book.GetBuyOrder();
        EXPECT_EQ(order_buy.GetPrice(), 63);
    }

    order_book.AddSellOrder(50, 61);
    const Order& order_sell = order_book.GetSellOrder();

    EXPECT_EQ(order_sell.GetPrice(), 50);
}
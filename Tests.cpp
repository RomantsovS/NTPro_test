#include <gtest/gtest.h>

#include <sstream>
#include <thread>

#include "Client.h"
#include "Server.h"

TEST(TestOrderBook, TestAddOrders) {
    std::ostringstream os;

    OrderBook order_book;

    order_book.AddBuyOrder(10, 62, 0);

    {
        auto order_buy = order_book.GetBuyOrder();
        EXPECT_EQ(order_buy->get().GetPrice(), 62);
    }

    order_book.AddBuyOrder(20, 63, 1);

    {
        auto order_buy = order_book.GetBuyOrder();
        EXPECT_EQ(order_buy->get().GetPrice(), 63);
    }

    order_book.AddSellOrder(50, 61, 2);
    auto order_sell = order_book.GetSellOrder();

    EXPECT_EQ(order_sell->get().GetPrice(), 61);
}

TEST(TestOrderBook, TestMakeDeal) {
    std::ostringstream os;

    OrderBook order_book;

    order_book.AddBuyOrder(10, 62, 0);
    order_book.AddBuyOrder(20, 63, 1);

    order_book.AddSellOrder(50, 61, 2);
    
    auto deal = order_book.MakeDeal();

    EXPECT_EQ(deal.has_value(), true);

    EXPECT_EQ(order_book.GetBuyOrder().has_value(), true);
    EXPECT_EQ(order_book.GetSellOrder().has_value(), true);

    EXPECT_EQ(order_book.GetBuyOrder()->get().GetPrice(), 62);
    EXPECT_EQ(order_book.GetBuyOrder()->get().GetAmount(), 10);

    EXPECT_EQ(order_book.GetSellOrder()->get().GetPrice(), 61);
    EXPECT_EQ(order_book.GetSellOrder()->get().GetAmount(), 30);
}

TEST(TestCore, TestMakeDeal) {
    std::ostringstream os;

    Core core;

    client_id_type id0 = core.RegisterNewUser("user1");
    client_id_type id1 = core.RegisterNewUser("user2");
    client_id_type id2 = core.RegisterNewUser("user3");

    EXPECT_EQ(id0, 0);
    EXPECT_EQ(id1, 1);
    EXPECT_EQ(id2, 2);

    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::USD), 0.0);
    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::RUB), 0.0);

    core.AddBuyOrder(10, 60, id0);
    core.AddBuyOrder(10, 62, id0);
    core.AddBuyOrder(20, 63, id1);

    core.AddSellOrder(50, 61, id2);
    core.AddSellOrder(50, 62, id2);

    core.MakeDeal();

    EXPECT_EQ(core.GetDeals().size(), 1u);

    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::USD), 0.0);
    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::RUB), 0.0);

    EXPECT_EQ(core.GetUser(id1)->get().GetBalance(Currencies::USD), 20.0);
    EXPECT_EQ(core.GetUser(id1)->get().GetBalance(Currencies::RUB), -1220.0);

    EXPECT_EQ(core.GetUser(id2)->get().GetBalance(Currencies::USD), -20.0);
    EXPECT_EQ(core.GetUser(id2)->get().GetBalance(Currencies::RUB), 1220.0);

    core.MakeDeal();

    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::USD), 10.0);
    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::RUB), -610);

    EXPECT_EQ(core.GetUser(id2)->get().GetBalance(Currencies::USD), -30.0);
    EXPECT_EQ(core.GetUser(id2)->get().GetBalance(Currencies::RUB), 1830.0);

    core.AddBuyOrder(20, 61, id0);

    core.MakeDeal();

    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::USD), 30.0);
    EXPECT_EQ(core.GetUser(id0)->get().GetBalance(Currencies::RUB), -1830.0);

    EXPECT_EQ(core.GetUser(id2)->get().GetBalance(Currencies::USD), -50.0);
    EXPECT_EQ(core.GetUser(id2)->get().GetBalance(Currencies::RUB), 3050);
}

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

        std::istringstream iss(R"(name)");

        std::string my_id = client.ProcessRegistration(iss);
        os << my_id;
    }).join();
    EXPECT_EQ(os.str(), expected);
}
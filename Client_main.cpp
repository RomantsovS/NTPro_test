#include <iostream>

#include "Client.h"
#include "Common.hpp"

int main() {
	try {
		boost::asio::io_context io_context;

		Client client(io_context, "127.0.0.1", common::port);

		// Мы предполагаем, что для идентификации пользователя будет использоваться ID.
		// Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам
		// ID. Этот ID далее используется при отправке запросов.
		client.ProcessRegistration(std::cin);
		std::cout << "registration OK: " << client.GetID() << std::endl;

		while (true) {
			// Тут реализовано "бесконечное" меню.
			std::cout << "Menu:\n"
				"0) Exit\n"
				"1) Hello Request\n"
				"2) Add order\n"
				"3) Get orders\n"
				"4) Get client deals\n"
				"5) Get client balance\n"
				<< std::endl;

			short menu_option_num;
			std::cin >> menu_option_num;
			switch (menu_option_num) {
			case 0: {
				exit(0);
				break;
			}
			case 1: {
				// Для примера того, как может выглядить взаимодействие с сервером
				// реализован один единственный метод - Hello.
				// Этот метод получает от сервера приветствие с именем клиента,
				// отправляя серверу id, полученный при регистрации.
				client.SendHello();
				std::cout << client.ReadMessage();
				break;
			}
			case 2: {
				client.AddOrder();
				break;
			}
			case 3: {
				client.GetOrders();
				break;
			}
			case 4: {
				client.GetClientDeals();
				break;
			}
			case 5: {
				client.GetClientBalance();
				break;
			}
			default: {
				std::cout << "Unknown menu option\n" << std::endl;
			}
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
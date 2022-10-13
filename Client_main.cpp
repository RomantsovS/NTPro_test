#include <iostream>

#include "Client.h"

int main() {
    try {
        boost::asio::io_context io_context;

        Client client(io_context, "127.0.0.1", common::port);

        // Мы предполагаем, что для идентификации пользователя будет использоваться ID.
        // Тут мы "регистрируем" пользователя - отправляем на сервер имя, а сервер возвращает нам
        // ID. Этот ID далее используется при отправке запросов.
        std::string my_id = client.ProcessRegistration();
        std::cout << "registration OK: " << my_id << "\n";
        
        while (true) {
            // Тут реализовано "бесконечное" меню.
            std::cout << "Menu:\n"
                         "1) Hello Request\n"
                         "2) Exit\n"
                      << std::endl;

            short menu_option_num;
            std::cin >> menu_option_num;
            switch (menu_option_num) {
                case 1: {
                    // Для примера того, как может выглядить взаимодействие с сервером
                    // реализован один единственный метод - Hello.
                    // Этот метод получает от сервера приветствие с именем клиента,
                    // отправляя серверу id, полученный при регистрации.
                    client.SendMessage(my_id, Requests::Hello, "");
                    std::cout << client.ReadMessage();
                    break;
                }
                case 2: {
                    exit(0);
                    break;
                }
                default: {
                    std::cout << "Unknown menu option\n" << std::endl;
                }
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
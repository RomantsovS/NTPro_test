#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <string>

class Client {
public:
	Client(boost::asio::io_context& io_context, const std::string& host, short port);

	const std::string& GetID() const { return id_; }

	// Отправка сообщения на сервер по шаблону.
	void SendMessageToServer(const std::string& aId, const std::string& aRequestType,
		const std::string& aMessage);

	// Возвращает строку с ответом сервера на последний запрос.
	std::string ReadMessage();

	void SendHello();

	// "Создаём" пользователя, получаем его ID.
	void ProcessRegistration(std::istream& is);

	void AddOrder();

	void GetOrders();
	void GetClientDeals();
	void GetClientBalance();

private:
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
	std::string id_;
};

#endif
#ifndef CLIENSERVERECN_COMMON_HPP
#define CLIENSERVERECN_COMMON_HPP

#include <string>
#include <unordered_map>

namespace common {

static short port = 5555;

};

namespace Requests {

static std::string Registration = "Reg";
static std::string Hello = "Hel";
static std::string AddOrder = "AddOrder";
static std::string GetOrders = "GetOrders";

enum class OrderType {
	Buy = 1,
	Sell = 2,
	Max = 3
};

static const std::unordered_map<OrderType, std::string> OrderTypeString = {
	{OrderType::Buy, "buy"}, {OrderType::Sell, "sell"}
};

}  // namespace Requests

#endif  // CLIENSERVERECN_COMMON_HPP

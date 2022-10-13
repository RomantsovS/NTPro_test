#ifndef USER_H
#define USER_H

using client_id_type = size_t;

enum class Currencies {
	USD,
	RUB
};

class User {
public:
	User() = default;
	User(client_id_type id, const std::string& name) : id_(id), name_(name) {}

	const std::string& GetName() const { return name_; }

	void IncreaseBalance(Currencies currency, double amount) {
		balance[currency] += amount;
	}
	void DecreaseBalance(Currencies currency, double amount) {
		balance[currency] -= amount;
	}

	double GetBalance(Currencies currency) const {
		if (balance.count(currency) == 0)
			return 0.0;
		return balance.at(currency);
	}
private:
	client_id_type id_;
	std::string name_;
	std::map<Currencies, double> balance;
};

#endif
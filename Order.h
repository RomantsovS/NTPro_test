#ifndef ORDER_H
#define ORDER_H

class Order {
   public:
   enum class Type {
    buy,
    sell
   };

   Order(int value, double price, Type type) : value_(value), price_(price), type_(type) {}

   double GetPrice() const {return price_;}

   private:
   int value_;
   double price_;
   Type type_;
};

#endif
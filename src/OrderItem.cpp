#include "OrderItem.h"

OrderItem::OrderItem() {
    productId = 0;
    productName = "";
    quantity = 0;
    price = 0.0;
    subtotal = 0.0;
}

OrderItem::OrderItem(int pid, const std::string &name, int qty, double p, double sub) {
    productId = pid;
    productName = name;
    quantity = qty;
    price = p;
    subtotal = sub;
}

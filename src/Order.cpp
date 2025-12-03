#include "Order.h"
#include <iostream>
#include <iomanip>

using namespace std;

Order::Order() {
    orderId = 0;
    customerId = 0;
    orderDate = "";
    deliveryDate = "";
    totalAmount = 0.0;
}

Order::Order(int oid, int cid, const std::string &od, const std::string &dd, double total) {
    orderId = oid;
    customerId = cid;
    orderDate = od;
    deliveryDate = dd;
    totalAmount = total;
}

void Order::addItem(const OrderItem &item) {
    items.push_back(item);
}

void Order::displayOrder() const {
    cout << "===========================\n";
    cout << "        ORDER DETAILS\n";
    cout << "===========================\n";

    cout << "Order ID       : " << orderId << "\n";
    cout << "Customer ID    : " << customerId << "\n";
    cout << "Order Date     : " << orderDate << "\n";
    cout << "Delivery Date  : " << deliveryDate << "\n";
    cout << "Total Amount   : " << totalAmount << "\n\n";

    cout << left << setw(6) << "No."
         << setw(35) << "Product Name"
         << setw(8) << "Qty"
         << setw(12) << "Price"
         << setw(12) << "Subtotal"
         << "\n";

    cout << string(75, '-') << "\n";

    int index = 1;
    for (const auto &it : items) {
        cout << left << setw(6) << index++
             << setw(35) << it.getProductName()
             << setw(8) << it.getQuantity()
             << setw(12) << it.getPrice()
             << setw(12) << it.getSubtotal() << "\n";
    }

    cout << string(75, '-') << "\n";
    cout << "Total Amount: " << totalAmount << "\n";
}

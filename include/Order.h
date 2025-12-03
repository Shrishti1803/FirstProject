#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "OrderItem.h"

class Order {
private:
    int orderId;
    int customerId;
    std::string orderDate;
    std::string deliveryDate;
    double totalAmount;

    std::vector<OrderItem> items;   // list of products in this order

public:
    Order();
    Order(int oid, int cid, const std::string &od, const std::string &dd, double total);

    // Getters
    int getOrderId() const { return orderId; }
    int getCustomerId() const { return customerId; }
    std::string getOrderDate() const { return orderDate; }
    std::string getDeliveryDate() const { return deliveryDate; }
    double getTotalAmount() const { return totalAmount; }
    const std::vector<OrderItem>& getItems() const { return items; }

    // Setters
    void setOrderId(int id) { orderId = id; }
    void setCustomerId(int id) { customerId = id; }
    void setOrderDate(const std::string &d) { orderDate = d; }
    void setDeliveryDate(const std::string &d) { deliveryDate = d; }
    void setTotalAmount(double amt) { totalAmount = amt; }

    // Add items
    void addItem(const OrderItem &item);

    // Utility
    void displayOrder() const;
};

#endif

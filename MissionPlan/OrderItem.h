#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <string>

class OrderItem {
private:
    int productId;
    std::string productName;
    int quantity;
    double price;
    double subtotal;

public:
    OrderItem();
    OrderItem(int pid, const std::string &name, int qty, double p, double sub);

    // Getters
    int getProductId() const { return productId; }
    std::string getProductName() const { return productName; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    double getSubtotal() const { return subtotal; }

    // Setters
    void setProductId(int id) { productId = id; }
    void setProductName(const std::string &n) { productName = n; }
    void setQuantity(int q) { quantity = q; }
    void setPrice(double p) { price = p; }
    void setSubtotal(double s) { subtotal = s; }
};

#endif

#include "Product.h"

Product::Product() {}

Product::Product(string name, string cat, float price, int qty, string comp, Date exp) {
    Product_name = name;
    Category = cat;
    Price = price;
    Stock_Qtn = qty;
    Company_name = comp;
    ExpiryDate = exp;
}

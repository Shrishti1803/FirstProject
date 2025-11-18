#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include "Date.h"
using namespace std;

class Product {

public:
    int Product_id;
    string Product_name;
    string Category;
    float Price;
    int Stock_Qtn;
    string Company_name;
    Date ExpiryDate;

    Product();
    Product(string name, string cat, float price, int qty, string comp, Date exp);
};

#endif

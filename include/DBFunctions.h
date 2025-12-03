#ifndef DBFUNCTIONS_H
#define DBFUNCTIONS_H

#include <iostream>
#include <string>
#include <vector>

// MySQL Connector/C++ headers
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Customer.h"
#include "Order.h"
#include "OrderItem.h"
#include "Address.h"


using namespace std;

// -------------------------
// DB Connection
// -------------------------
sql::Connection* createConnection();

// -------------------------
// Display functions (read-only)
// -------------------------
void displayAllProducts(sql::Connection *con);
void displayByCategory(sql::Connection* con, string cat);
void displayBySubcategory(sql::Connection* con, string cat, string subcat);

// -------------------------
// Dynamic loaders (vector-based)
// -------------------------
vector<string> loadCategories(sql::Connection* con);
vector<string> loadSubcategories(sql::Connection* con, const string &category);

//-----------------
// Customer details 
//-----------------
void inputCustomerDetails(sql::Connection* con, const std::string& emailFromLogin);
void insertCustomerDetails(sql::Connection* con, const Customer& c);

void displayProductDetailsById(int productId);
void displaySupplierForProduct(int productId);

int getLastInsertedCustomerID(sql::Connection* con);

int getLastInsertedCustomerID(sql::Connection* con);
int getCustomerIdByEmail(sql::Connection* con, const std::string& email);
int getCustomerIdByEmail(sql::Connection* con, const std::string& email);

// Creates an order from the cart
bool createOrderFromCart(sql::Connection* con, int customerId, int &orderId);

// Load all orders of a customer (most recent first)
std::vector<Order> loadOrdersForCustomer(sql::Connection* con, int customerId);

// Load all items in a particular order
std::vector<OrderItem> loadOrderItems(sql::Connection* con, int orderId);

// Fetch a Customer by ID
bool getCustomerById(sql::Connection* con, int customerId, Customer &outCustomer);

// Update customer details (Name, Contact_Num, Address) by ID
bool updateCustomerDetails(sql::Connection* con, const Customer &c);

// Update password for a login email
bool updateLoginPassword(sql::Connection* con, const std::string &email, const std::string &newPassword);

// create order from cart; sets created orderId
bool createOrderFromCart(sql::Connection* con, int customerId, int addressId, int &orderId);

// load orders (most recent first)
std::vector<Order> loadOrdersForCustomer(sql::Connection* con, int customerId);

// load order items
std::vector<OrderItem> loadOrderItems(sql::Connection* con, int orderId);

// customer helpers
bool getCustomerById(sql::Connection* con, int customerId, Customer &outCustomer);
bool updateCustomerDetails(sql::Connection* con, const Customer &c);
bool updateLoginPassword(sql::Connection* con, const std::string &email, const std::string &newPassword);

// ---- PRODUCT SEARCH ----
vector<int> searchProductsByName(sql::Connection* con, const string &name);
vector<int> searchProductsByCompany(sql::Connection* con, const string &company);
vector<int> searchProductsByCategory(sql::Connection* con, const string &category);
vector<int> searchProductsByPriceRange(sql::Connection* con, double minPrice, double maxPrice);

vector<int> sortProductsByPriceAsc(sql::Connection*, const string&, const string&);
vector<int> sortProductsByPriceDesc(sql::Connection*, const string&, const string&);
vector<int> sortProductsByName(sql::Connection*, const string&, const string&);
vector<int> sortProductsByStock(sql::Connection*, const string&, const string&);

vector<int> filterProductsByCompany(sql::Connection*, const string&, const string&, const string&);
vector<int> filterProductsByPriceRange(sql::Connection*, const string&, const string&, float, float);
void displayCompactProductRow(sql::Connection* con, int productId, int index);
bool addAddress(sql::Connection* con, int customerId, const string &line, const string &city, const string &state, const string &pincode);
vector<pair<int,string>> loadAddresses(sql::Connection* con, int customerId);
bool deleteAddress(sql::Connection* con, int addressId);
vector<Address> loadFullAddresses(sql::Connection* con, int customerId);

#endif // DBFUNCTIONS_H

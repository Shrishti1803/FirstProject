// Functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <cppconn/connection.h>  // for sql::Connection*

// Basic UI helpers
void clearScreen();
void pressEnterToContinue();
int getIntInput(const std::string &prompt);
void printFancyHeader(const std::string &title);
std::string color(const std::string &code, const std::string &text);

// Updated prototypes with customerId parameter
void productDetailsScreen(int productId, int customerId, sql::Connection* con);
void handleProductsFlow(sql::Connection *con,
                        const std::string &category,
                        const std::string &subcategory,
                        int customerId);
void handleSubcategoryFlow(sql::Connection *con,
                           const std::string &category,
                           int customerId);
void runCategoryMenu(sql::Connection *con, int customerId);
void runSearchMenu(sql::Connection* con, int customerId);

// My Profile / Orders UI (B3)
void showMyProfile(sql::Connection* con, int customerId, const std::string &loggedInEmail);
void showMyOrders(sql::Connection* con, int customerId);
void showOrderDetails(sql::Connection* con, int orderId, int orderNumber);
void editProfileMenu(sql::Connection* con, int customerId, const std::string &loggedInEmail);

// My Profile + Orders UI
void showMyProfile(sql::Connection* con, int customerId, const std::string &loggedInEmail);
void editProfileMenu(sql::Connection* con, int customerId, const std::string &loggedInEmail);
void showMyOrders(sql::Connection* con, int customerId);
void showOrderDetails(sql::Connection* con, int orderId, int orderNumber);


#endif

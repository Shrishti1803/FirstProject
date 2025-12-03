// Functions.h
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <cppconn/connection.h>  // for sql::Connection*
#include "Address.h"

// Basic UI helpers
void clearScreen();
void pressEnterToContinue();
int getIntInput(const std::string &prompt);
void printFancyHeader(const std::string &title);
std::string color(const std::string &code, const std::string &text);

// Product & Category flows
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

// My Profile + Orders
void showMyProfile(sql::Connection* con, int customerId, const std::string &loggedInEmail);
void editProfileMenu(sql::Connection* con, int customerId, const std::string &loggedInEmail);
void showMyOrders(sql::Connection* con, int customerId);
void showOrderDetails(sql::Connection* con, int orderId, int orderNumber);

// Addresses (NEW)
int selectDeliveryAddress(sql::Connection* con, int customerId);
void manageAddresses(sql::Connection* con, int customerId);

#endif

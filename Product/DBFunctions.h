#ifndef DBFUNCTIONS_H
#define DBFUNCTIONS_H

#include <bits/stdc++.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

#include "Product.h"
#include "Date.h"

using namespace std;
using namespace sql;

// Function declarations
sql::Connection* createConnection();
Product inputProduct();
void insertProduct(sql::Connection* con, Product p);
void displayAllProducts(sql::Connection* con);

#endif
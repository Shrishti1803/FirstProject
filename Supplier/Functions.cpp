#include "Functions.h"
#include <iomanip>
// 1) CONNECT TO DATABASE
sql::Connection* createConnection() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "*****");

    con->setSchema("PROJECT");
    return con;
}


// 4) SELECT AND DISPLAY PRODUCTS
void displayAllProducts(sql::Connection *con) {
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM Supplier");

    // Print Header
    cout << left
         << setw(15) << "Supplier_ID"
         << setw(20) << "Supplier_Name"
         << setw(20) << "Contact_Num"
         << setw(30) << "Email"
         << setw(25) << "Address"
         << endl;

    cout << string(104, '-') << endl; // separator line

    // Print Rows
    while (res->next()) {
        cout << left
             << setw(15) << res->getInt("SID")
             << setw(20) << res->getString("Sname")
             << setw(20) << res->getString("COntact_Num")
             << setw(30) << res->getString("Email")
             << setw(25) << res->getString("Address")
             << endl;
    }

    delete res;
    delete stmt;
}

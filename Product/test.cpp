#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <iostream>
using namespace std;

int main() {
    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        // FIRST initialize driver
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "shrishti@2006");
        con->setSchema("PROJECT1");

        // THEN connect
        con = driver->connect("tcp://127.0.0.1:3306", "root", "shrishti@2006");

        cout << "Connected to MySQL!" << endl;

        delete con;
    }
    catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }
}

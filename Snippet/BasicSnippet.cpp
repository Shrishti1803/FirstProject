#include<bits/stdc++.h>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;

class Date{
    
    public:
        int day;
        int month;
        int year;

        Date() {}

        Date(int d, int m, int y) {
            day = d;
            month = m;
            year = y;
        }
        void setdate(int d, int m, int y){
            this-> day = d;
            this-> month = m;
            this-> year = y;
        }

        void getdate(){
            cout << day << "-" << month << "-" << year << endl;
        }

};
class Product{

    public:
        int Product_id;
        string Product_name;
        string Category;
        float Price;
        int Stock_Qtn;
        string Company_name;
        Date ExpiryDate;    

        Product() {}

        Product(string name, string cat, float price, int qty, string comp, Date exp) {
            Product_name = name;
            Category = cat;
            Price = price;
            Stock_Qtn = qty;
            Company_name = comp;
            ExpiryDate = exp;
        }


};

int main(){


    try {
        // ---------------------------------------------------------
        // 1️⃣ Declare MySQL driver and connection pointers
        // ---------------------------------------------------------
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        // ---------------------------------------------------------
        // 2️⃣ Initialize the MySQL driver
        //    (This creates the driver object needed to connect)
        // ---------------------------------------------------------
        driver = sql::mysql::get_mysql_driver_instance();

        // ---------------------------------------------------------
        // 3️⃣ Connect to the MySQL server
        //    - 127.0.0.1  = local machine
        //    - 3306       = MySQL default port
        //    - root       = username
        //    - password   = your MySQL password
        // ---------------------------------------------------------
        con = driver->connect("tcp://127.0.0.1:3306", "root", "*****");

        cout << "Connected to MySQL!" << endl;

        // ---------------------------------------------------------
        // 4️⃣ Select the database to work with
        //    All queries after this will run inside 'PROJECT1'
        // ---------------------------------------------------------
        con->setSchema("PROJECT");

        // ---------------------------------------------------------
        // 5️⃣ TAKING THE USER INPUT FOR ALL THE DETAILS 
        //    This product will be inserted into MySQL
        // ---------------------------------------------------------

        Product p;
        Date exp;

        // Product Name
        cout << "Enter Product Name: ";
        cin.ignore();//cin.ignore() → clears leftover junk so that
                     //getline() → can properly read a full line.
        getline(cin, p.Product_name);

        // Category
        cout << "Enter Category: ";
        getline(cin, p.Category);

        // Price
        cout << "Enter Price: ";
        cin >> p.Price;

        // Stock Qty
        cout << "Enter Stock Quantity: ";
        cin >> p.Stock_Qtn;

        cin.ignore(); // flush newline before taking string input

        // Company Name
        cout << "Enter Company Name: ";
        getline(cin, p.Company_name);

        // Expiry Date

        string dateInput;
        cout << "Enter Expiry Date (DD-MM-YYYY): ";
        cin.ignore();
        getline(cin, dateInput);

        sscanf(dateInput.c_str(), "%d-%d-%d", &exp.day, &exp.month, &exp.year);

        p.ExpiryDate = exp;









        // Convert C++ date → MySQL DATE format (YYYY-MM-DD)
        string sqlDate = to_string(p.ExpiryDate.year) + "-" +
                         to_string(p.ExpiryDate.month) + "-" +
                         to_string(p.ExpiryDate.day);

        // ---------------------------------------------------------
        // 6️⃣ Prepare an SQL INSERT statement
        //    Using prepared statements is safer and prevents SQL injection
        // ---------------------------------------------------------
        sql::PreparedStatement *pstmt;
        pstmt = con->prepareStatement(
        "INSERT INTO PRODUCT(Product_name, Category, Price, Stock_Qtn, Company_name, ExpiryDate) "
        "VALUES(?, ?, ?, ?, ?, ?)"
        );


        // ---------------------------------------------------------
        // 7️⃣ Bind Product object's values into the SQL INSERT query
        // ---------------------------------------------------------
        pstmt->setString(1, p.Product_name);
        pstmt->setString(2, p.Category);
        pstmt->setDouble(3, p.Price);
        pstmt->setInt(4, p.Stock_Qtn);
        pstmt->setString(5, p.Company_name);
        pstmt->setString(6, sqlDate);  // DATE goes here

        // ---------------------------------------------------------
        // 8️⃣ Execute the INSERT command
        //    This sends the data into the MySQL PRODUCT table
        // ---------------------------------------------------------
        pstmt->execute();

        cout << "Product Inserted Successfully!" << endl;

        delete pstmt;   // free memory

        // ---------------------------------------------------------
        // 9️⃣ Run a SELECT query to fetch all products from the table
        // ---------------------------------------------------------
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM PRODUCT");

        // ---------------------------------------------------------
        // 🔟 Print all rows returned by the SELECT query
        // ---------------------------------------------------------
        while (res->next()) {
            cout << res->getInt("Product_ID") << " | "
                 << res->getString("Product_name") << " | "
                 << res->getString("Category") << " | "
                 << res->getDouble("Price") << " | "
                 << res->getInt("Stock_Qtn") << " | "
                 << res->getString("Company_name") << " | "
                 << res->getString("ExpiryDate")
                 << endl;
        }

        // ---------------------------------------------------------
        // 1️⃣1️⃣ Clean up memory
        // ---------------------------------------------------------
        delete res;
        delete stmt;
        delete con;
    }

    // -------------------------------------------------------------
    // Catch and display any MySQL errors (wrong query, wrong table, etc.)
    // -------------------------------------------------------------
    catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
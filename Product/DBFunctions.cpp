#include "DBFunctions.h"
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


// 2) TAKE USER INPUT
Product inputProduct() {
    Product p;
    Date exp;

    cout << "Enter Product Name: ";
    cin.ignore();
    getline(cin, p.Product_name);

    cout << "Enter Category: ";
    getline(cin, p.Category);

    cout << "Enter Price: ";
    cin >> p.Price;

    cout << "Enter Stock Quantity: ";
    cin >> p.Stock_Qtn;

    cin.ignore();

    cout << "Enter Company Name: ";
    getline(cin, p.Company_name);

    string dateInput;
    cout << "Enter Expiry Date (DD-MM-YYYY): ";
    getline(cin, dateInput);

    sscanf(dateInput.c_str(), "%d-%d-%d", &exp.day, &exp.month, &exp.year);

    p.ExpiryDate = exp;

    return p;
}


// 3) INSERT PRODUCT INTO DB
void insertProduct(sql::Connection *con, Product p) {
    sql::PreparedStatement *pstmt;

    string sqlDate = p.ExpiryDate.toSQL();

    pstmt = con->prepareStatement(
        "INSERT INTO PRODUCT(Product_name, Category, Price, Stock_Qtn, Company_name, ExpiryDate) "
        "VALUES(?, ?, ?, ?, ?, ?)"
    );

    pstmt->setString(1, p.Product_name);
    pstmt->setString(2, p.Category);
    pstmt->setDouble(3, p.Price);
    pstmt->setInt(4, p.Stock_Qtn);
    pstmt->setString(5, p.Company_name);
    pstmt->setString(6, sqlDate);

    pstmt->execute();

    cout << "Product Inserted Successfully!" << endl;

    delete pstmt;
}


// 4) SELECT AND DISPLAY PRODUCTS
void displayAllProducts(sql::Connection *con) {
    sql::Statement *stmt = con->createStatement();
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM PRODUCT");

    // Print Header
    cout << left
         << setw(12) << "Product_ID"
         << setw(20) << "Product_Name"
         << setw(15) << "Category"
         << setw(10) << "Price"
         << setw(12) << "Stock_Qtn"
         << setw(20) << "Company_Name"
         << setw(15) << "ExpiryDate"
         << endl;

    cout << string(104, '-') << endl; // separator line

    // Print Rows
    while (res->next()) {
        cout << left
             << setw(12) << res->getInt("Product_ID")
             << setw(20) << res->getString("Product_name")
             << setw(15) << res->getString("Category")
             << setw(10) << res->getDouble("Price")
             << setw(12) << res->getInt("Stock_Qtn")
             << setw(20) << res->getString("Company_name")
             << setw(15) << res->getString("ExpiryDate")
             << endl;
    }

    delete res;
    delete stmt;
}

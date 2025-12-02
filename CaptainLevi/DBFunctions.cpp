// DBFunctions.cpp
// Clean DB functions: connection, display (read-only) and dynamic loaders

#include "DBFunctions.h"
#include "Customer.h"
#include <iomanip>
#include <limits>


// -----------------------------
// 1) ESTABLISHING THE DATABASE CONNECTION
// -----------------------------
sql::Connection* createConnection() {
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        // update username/password as necessary
        con = driver->connect("tcp://127.0.0.1:3306", "root", "shrishti@2006"); //give the mysql password 
        con->setSchema("PROJECT1"); //make sure all the required tables are in this database
    } catch (sql::SQLException &e) {
        cout << "Unable to connect to DB: " << e.what() << endl;
        throw;
    }

    return con;
}

// ---------------------------------------
// 2) DISPLAY BY CATEGORY OF THE PRODUCTS
// ---------------------------------------
void displayByCategory(sql::Connection* con, string cat) {
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        pstmt = con->prepareStatement(
            "SELECT * FROM PRODUCT WHERE Category = ?" 
        );
        pstmt->setString(1, cat);
        res = pstmt->executeQuery();

        cout << "\n--- Products in Category: " << cat << " ---\n\n";

        cout << left
             << setw(10) << "Product_ID"
             << setw(28) << "Product_Name"
             << setw(18) << "Category"
             << setw(18) << "Subcategory"
             << setw(10) << "Price"
             << setw(10) << "Stock"
             << setw(20) << "Company"
             << setw(12) << "Expiry"
             << endl;

        cout << string(126, '-') << endl;

        while (res->next()) {
            cout << left
                 << setw(10) << res->getInt("Product_ID")
                 << setw(28) << res->getString("Product_name")
                 << setw(18) << res->getString("Category")
                 << setw(18) << res->getString("Subcategory")
                 << setw(10) << res->getDouble("Price")
                 << setw(10) << res->getInt("Stock_Qtn")
                 << setw(20) << res->getString("Company_name")
                 << setw(12) << res->getString("ExpiryDate")
                 << endl;
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in displayByCategory: " << e.what() << endl;
    }

    if (res) delete res;
    if (pstmt) delete pstmt;
}

// ------------------------------------
// 3) DISPLAY BY CATEGORY + SUBCATEGORY
// ------------------------------------
void displayBySubcategory(sql::Connection* con, string cat, string subcat) {
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
          pstmt = con->prepareStatement(
                "SELECT Product_ID, Product_Name, Price "
                "FROM PRODUCT "
                "WHERE Category = ? AND Subcategory = ? "
                "ORDER BY Product_ID"
);

        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        res = pstmt->executeQuery();

        cout << "\n--- Products in " << cat << " → " << subcat << " ---\n\n";

        cout << left
             << setw(6)  << "ID"
             << setw(35) << "Name"
             << setw(10) << "Price"
             << '\n';

        cout << string(6 + 35 + 10, '-') << '\n';


        while (res->next()) {
        cout << left
            << setw(6)  << res->getInt("Product_ID")
            << setw(35) << res->getString("Product_Name")
            << setw(10) << res->getDouble("Price")
            << '\n';
}

    } catch (sql::SQLException &e) {
        cout << "SQL Error in displayBySubcategory: " << e.what() << endl;
    }

    if (res) delete res;
    if (pstmt) delete pstmt;
}

// -----------------------------
// 4) DISPLAY ALL PRODUCTS
// -----------------------------
void displayAllProducts(sql::Connection *con) {
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;

    try {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT * FROM PRODUCT");

        cout << left
             << setw(10) << "Product_ID"
             << setw(28) << "Product_Name"
             << setw(18) << "Category"
             << setw(18) << "Subcategory"
             << setw(10) << "Price"
             << setw(10) << "Stock_Qtn"
             << setw(20) << "Company_Name"
             << setw(12) << "ExpiryDate"
             << endl;

        cout << string(126, '-') << endl;

        while (res->next()) {
            cout << left
                 << setw(10) << res->getInt("Product_ID")
                 << setw(28) << res->getString("Product_name")
                 << setw(18) << res->getString("Category")
                 << setw(18) << res->getString("Subcategory")
                 << setw(10) << res->getDouble("Price")
                 << setw(10) << res->getInt("Stock_Qtn")
                 << setw(20) << res->getString("Company_name")
                 << setw(12) << res->getString("ExpiryDate")
                 << endl;
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in displayAllProducts: " << e.what() << endl;
    }

    if (res) delete res;
    if (stmt) delete stmt;
}

// -------------------------------------------
// 5) LOAD DISTINCT CATEGORIES (vector-based)
// -------------------------------------------
vector<string> loadCategories(sql::Connection* con) {
    vector<string> categories;
    sql::Statement *stmt = nullptr;
    sql::ResultSet *res = nullptr;
    try {
        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT DISTINCT Category FROM PRODUCT ORDER BY Category ASC");

        while (res->next()) {
            string cat = res->getString("Category");
            if (!cat.empty())
                categories.push_back(cat);
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in loadCategories: " << e.what() << endl;
    }
    if (res) delete res;
    if (stmt) delete stmt;
    return categories;
}

// ---------------------------------------------
// 6) LOAD DISTINCT SUBCATEGORIES (vector-based)
// ---------------------------------------------
vector<string> loadSubcategories(sql::Connection* con, const string &category) {
    vector<string> subcats;
    sql::PreparedStatement *pstmt = nullptr;
    sql::ResultSet *res = nullptr;
    try {
        pstmt = con->prepareStatement(
            "SELECT DISTINCT Subcategory FROM PRODUCT WHERE Category = ? ORDER BY Subcategory ASC"
        );
        pstmt->setString(1, category);
        res = pstmt->executeQuery();
        while (res->next()) {
            string s = res->getString("Subcategory");
            if (!s.empty())
                subcats.push_back(s);
        }
    } catch (sql::SQLException &e) {
        cout << "SQL Error in loadSubcategories: " << e.what() << endl;
    }
    if (res) delete res;
    if (pstmt) delete pstmt;
    return subcats;
}

// ---------------------------------------------------
// 7) Take Customer details & save to CUSTOMER_DETAILS
// ---------------------------------------------------
void inputCustomerDetails(sql::Connection* con, const std::string& emailFromLogin) {
    if (!con) {
        cout << "❌ No active DB connection.\n";
        return;
    }
    Customer c;
    int id;
    std::string name, contact, address;

    cout << "\n=====================================\n";
    cout << "         SAVE CUSTOMER DETAILS       \n";
    cout << "=====================================\n\n";

    cout << "Enter Full Name: ";
    std::getline(cin, name);

    cout << "Enter Contact Number: ";
    std::getline(cin, contact);

    // We do NOT ask for email again.
    // We use the same email that was used during login.
    cout << "Using Email from login: " << emailFromLogin << "\n";

    cout << "Enter Address: ";
    std::getline(cin, address);

    c.setName(name);
    c.setContactnumber(contact);
    c.setEmail(emailFromLogin);
    c.setAddress(address);

    // Now insert this customer details into the DB
    insertCustomerDetails(con, c);
}

void insertCustomerDetails(sql::Connection* con, const Customer &c) {
    try {
            sql::PreparedStatement* pstmt = con->prepareStatement(
                "INSERT INTO CUSTOMER_DETAILS (Name, Contact_Num, Email, Address) VALUES (?, ?, ?, ?)"
            );

            pstmt->setString(1, c.getName());
            pstmt->setString(2, c.getContactnumber());
            pstmt->setString(3, c.getEmail());
            pstmt->setString(4, c.getAddress());
            pstmt->executeUpdate();

        delete pstmt;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in insertCustomerDetails: " << e.what() << endl;
    }
}



// (we will NOT print Product_ID or SID to the user)
void displayProductDetailsById(int productId) {
    sql::Connection* con = nullptr;
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        con = createConnection();

        // We use Product_ID internally, but we don't print it
        pstmt = con->prepareStatement(
            "SELECT Product_Name, Category, Subcategory, " 
            "       Price, Stock_Qtn, Company_name, ExpiryDate "
            "FROM PRODUCT "
            "WHERE Product_ID = ?"
        );
        pstmt->setInt(1, productId);   // we pass the ID from C++ (not from user typing)

        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "\n========== PRODUCT DETAILS ==========\n";
            cout << "Product Name   : " << res->getString("Product_Name") << '\n';
            cout << "Category       : " << res->getString("Category") << '\n';
            cout << "Subcategory    : " << res->getString("Subcategory") << '\n';
            cout << "Price          : " << res->getDouble("Price") << '\n';
            cout << "Stock Quantity : " << res->getInt("Stock_Qtn") << '\n';
            cout << "Brand/Company  : " << res->getString("Company_name") << '\n';
            cout << "Expiry Date    : " << res->getString("ExpiryDate") << '\n';
        } else {
            cout << "\nNo product found for this selection.\n";
        }

        delete res;
        delete pstmt;
        con->close();
        delete con;
    }
    catch (sql::SQLException& e) {
        cerr << "\n[ERROR] Failed to fetch product details: " << e.what() << endl;
        if (res) delete res;
        if (pstmt) delete pstmt;
        if (con) { con->close(); delete con; }
    }
}

// Show supplier details for a given product
// (we join PRODUCT -> Supplier using SID, but we only display Sname, Contact_Num, Email, Address)
void displaySupplierForProduct(int productId) {
    sql::Connection* con = nullptr;
    sql::PreparedStatement* pstmt = nullptr;
    sql::ResultSet* res = nullptr;

    try {
        con = createConnection();

        pstmt = con->prepareStatement(
            "SELECT s.Sname, s.Contact_Num, s.Email, s.Address "
            "FROM PRODUCT p "
            "JOIN Supplier s ON p.SID = s.SID "
            "WHERE p.Product_ID = ?"
        );
        pstmt->setInt(1, productId);

        res = pstmt->executeQuery();

        if (res->next()) {
            cout << "\n====== SUPPLIER DETAILS ======\n";
            cout << "Supplier Name   : " << res->getString("Sname") << '\n';
            cout << "Contact Number  : " << res->getString("Contact_Num") << '\n';
            cout << "Email           : " << res->getString("Email") << '\n';
            cout << "Address         : " << res->getString("Address") << '\n';
        } else {
            cout << "\nNo supplier found for this product.\n";
        }

        delete res;
        delete pstmt;
        con->close();
        delete con;
    }
    catch (sql::SQLException& e) {
        cerr << "\n[ERROR] Failed to fetch supplier details: " << e.what() << endl;
        if (res) delete res;
        if (pstmt) delete pstmt;
        if (con) { con->close(); delete con; }
    }
}

//To auto detect the last entered customer id 
int getLastInsertedCustomerID(sql::Connection* con) {
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT LAST_INSERT_ID()");
        sql::ResultSet* res = pstmt->executeQuery();

        int id = -1;
        if (res->next()) {
            id = res->getInt(1);  // FIRST column
        }

        delete res;
        delete pstmt;

        return id;
    } catch (sql::SQLException &e) {
        std::cout << "Error fetching last inserted Customer ID: " << e.what() << std::endl;
        return -1;
    }
}

int getCustomerIdByEmail(sql::Connection* con, const std::string& email) {
    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("SELECT ID FROM CUSTOMER_DETAILS WHERE Email = ?");
        pstmt->setString(1, email);

        sql::ResultSet* res = pstmt->executeQuery();

        int id = -1;
        if (res->next()) {
            id = res->getInt("ID");
        }

        delete res;
        delete pstmt;

        return id;
    }
    catch (sql::SQLException &e) {
        std::cout << "SQL Error in getCustomerIdByEmail: " << e.what() << std::endl;
        return -1;
    }
}


// ---------------- createOrderFromCart ----------------
bool createOrderFromCart(sql::Connection* con, int customerId, int &createdOrderId) {
    if (!con) return false;
    try {
        // 1) load cart items
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT c.Product_ID, c.Quantity, p.Product_Name, p.Price "
            "FROM Cart c JOIN PRODUCT p ON c.Product_ID = p.Product_ID "
            "WHERE c.Customer_ID = ?"
        );
        pstmt->setInt(1, customerId);
        sql::ResultSet* res = pstmt->executeQuery();

        std::vector<int> productIds;
        std::vector<int> qtys;
        std::vector<double> prices;
        std::vector<std::string> names;
        double total = 0.0;

        while (res->next()) {
            int pid = res->getInt("Product_ID");
            int qty = res->getInt("Quantity");
            std::string pname = res->getString("Product_Name");
            double price = res->getDouble("Price");
            double subtotal = price * qty;

            productIds.push_back(pid);
            qtys.push_back(qty);
            prices.push_back(price);
            names.push_back(pname);
            total += subtotal;
        }
        delete res;
        delete pstmt;

        if (productIds.empty()) return false;

        // 2) insert Orders row (OrderDate = CURDATE(), DeliveryDate = CURDATE() + 4 days)
        pstmt = con->prepareStatement(
            "INSERT INTO Orders (CustomerID, OrderDate, DeliveryDate, TotalAmount) "
            "VALUES (?, CURDATE(), DATE_ADD(CURDATE(), INTERVAL 4 DAY), ?)"
        );
        pstmt->setInt(1, customerId);
        pstmt->setDouble(2, total);
        pstmt->executeUpdate();
        delete pstmt;

        // 3) grab LAST_INSERT_ID()
        pstmt = con->prepareStatement("SELECT LAST_INSERT_ID() AS id");
        res = pstmt->executeQuery();
        int orderId = -1;
        if (res->next()) orderId = res->getInt("id");
        delete res;
        delete pstmt;

        if (orderId == -1) return false;
        createdOrderId = orderId;

        // 4) insert OrderItems
        for (size_t i = 0; i < productIds.size(); ++i) {
            sql::PreparedStatement* p2 = con->prepareStatement(
                "INSERT INTO OrderItems (OrderID, ProductID, Quantity, PriceAtPurchase, Subtotal) "
                "VALUES (?, ?, ?, ?, ?)"
            );
            p2->setInt(1, orderId);
            p2->setInt(2, productIds[i]);
            p2->setInt(3, qtys[i]);
            p2->setDouble(4, prices[i]);
            p2->setDouble(5, prices[i] * qtys[i]);
            p2->executeUpdate();
            delete p2;
        }

        // 5) reduce stock
        sql::Statement* stmt = con->createStatement();
        stmt->execute(
            "UPDATE PRODUCT p "
            "JOIN Cart c ON p.Product_ID = c.Product_ID "
            "SET p.Stock_Qtn = p.Stock_Qtn - c.Quantity "
            "WHERE c.Customer_ID = " + std::to_string(customerId)
        );

        // 6) clear cart
        stmt->execute("DELETE FROM Cart WHERE Customer_ID = " + std::to_string(customerId));
        delete stmt;

        return true;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in createOrderFromCart: " << e.what() << endl;
        return false;
    }
}

// ---------------- loadOrdersForCustomer ----------------
std::vector<Order> loadOrdersForCustomer(sql::Connection* con, int customerId) {
    std::vector<Order> orders;
    if (!con) return orders;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT OrderID, OrderDate, DeliveryDate, TotalAmount "
            "FROM Orders WHERE CustomerID = ? ORDER BY OrderDate DESC, OrderID DESC"
        );
        pstmt->setInt(1, customerId);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            orders.push_back(
                Order(
                    res->getInt("OrderID"),
                    customerId,
                    res->getString("OrderDate"),
                    res->getString("DeliveryDate"),
                    res->getDouble("TotalAmount")
                )
            );
        }
        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in loadOrdersForCustomer: " << e.what() << endl;
    }
    return orders;
}

// ---------------- loadOrderItems ----------------
std::vector<OrderItem> loadOrderItems(sql::Connection* con, int orderId) {
    std::vector<OrderItem> items;
    if (!con) return items;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT oi.ProductID, p.Product_Name, oi.Quantity, oi.PriceAtPurchase, oi.Subtotal "
            "FROM OrderItems oi JOIN PRODUCT p ON oi.ProductID = p.Product_ID "
            "WHERE oi.OrderID = ?"
        );
        pstmt->setInt(1, orderId);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            items.push_back(
                OrderItem(
                    res->getInt("ProductID"),
                    res->getString("Product_Name"),
                    res->getInt("Quantity"),
                    res->getDouble("PriceAtPurchase"),
                    res->getDouble("Subtotal")
                )
            );
        }
        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in loadOrderItems: " << e.what() << endl;
    }
    return items;
}

// ---------------- getCustomerById ----------------
bool getCustomerById(sql::Connection* con, int customerId, Customer &outCustomer) {
    if (!con) return false;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT ID, Name, Contact_Num, Email, Address FROM CUSTOMER_DETAILS WHERE ID = ?"
        );
        pstmt->setInt(1, customerId);
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next()) {
            outCustomer.setId(res->getInt("ID"));
            outCustomer.setName(res->getString("Name"));
            outCustomer.setContactnumber(res->getString("Contact_Num"));
            outCustomer.setEmail(res->getString("Email"));
            outCustomer.setAddress(res->getString("Address"));
            delete res;
            delete pstmt;
            return true;
        }
        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in getCustomerById: " << e.what() << endl;
    }
    return false;
}

// ---------------- updateCustomerDetails ----------------
bool updateCustomerDetails(sql::Connection* con, const Customer &c) {
    if (!con) return false;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE CUSTOMER_DETAILS SET Name = ?, Contact_Num = ?, Address = ? WHERE ID = ?"
        );
        pstmt->setString(1, c.getName());
        pstmt->setString(2, c.getContactnumber());
        pstmt->setString(3, c.getAddress());
        pstmt->setInt(4, c.getId());
        int rows = pstmt->executeUpdate();
        delete pstmt;
        return rows > 0;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in updateCustomerDetails: " << e.what() << endl;
        return false;
    }
}

// ---------------- updateLoginPassword ----------------
bool updateLoginPassword(sql::Connection* con, const std::string &email, const std::string &newPassword) {
    if (!con) return false;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "UPDATE `LOGIN` SET Password = ? WHERE Email = ?"
        );
        pstmt->setString(1, newPassword);
        pstmt->setString(2, email);
        int rows = pstmt->executeUpdate();
        delete pstmt;
        return rows > 0;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in updateLoginPassword: " << e.what() << endl;
        return false;
    }
}

vector<int> searchProductsByName(sql::Connection* con, const string &name) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Product_Name LIKE ?"
        );
        pstmt->setString(1, "%" + name + "%");
        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            ids.push_back(res->getInt("Product_ID"));
        }

        delete res; delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in searchProductsByName: " << e.what() << endl;
    }
    return ids;
}

vector<int> searchProductsByCompany(sql::Connection* con, const string &company) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Company_name LIKE ?"
        );
        pstmt->setString(1, "%" + company + "%");
        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            ids.push_back(res->getInt("Product_ID"));
        }

        delete res; delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in searchProductsByCompany: " << e.what() << endl;
    }
    return ids;
}

vector<int> searchProductsByCategory(sql::Connection* con, const string &category) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category LIKE ?"
        );
        pstmt->setString(1, "%" + category + "%");
        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            ids.push_back(res->getInt("Product_ID"));
        }

        delete res; delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in searchProductsByCategory: " << e.what() << endl;
    }
    return ids;
}

vector<int> searchProductsByPriceRange(sql::Connection* con, double minPrice, double maxPrice) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Price BETWEEN ? AND ?"
        );
        pstmt->setDouble(1, minPrice);
        pstmt->setDouble(2, maxPrice);
        sql::ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            ids.push_back(res->getInt("Product_ID"));
        }

        delete res; delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error in searchProductsByPriceRange: " << e.what() << endl;
    }
    return ids;
}

vector<int> sortProductsByPriceAsc(sql::Connection* con, const string &cat, const string &subcat) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category=? AND Subcategory=? ORDER BY Price ASC"
        );
        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        sql::ResultSet* rs = pstmt->executeQuery();

        while (rs->next())
            ids.push_back(rs->getInt(1));

        delete rs;
        delete pstmt;
    } catch (...) {}
    return ids;
}

vector<int> sortProductsByPriceDesc(sql::Connection* con, const string &cat, const string &subcat) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category=? AND Subcategory=? ORDER BY Price DESC"
        );
        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        sql::ResultSet* rs = pstmt->executeQuery();

        while (rs->next())
            ids.push_back(rs->getInt(1));

        delete rs;
        delete pstmt;
    } catch (...) {}
    return ids;
}

vector<int> sortProductsByName(sql::Connection* con, const string &cat, const string &subcat) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category=? AND Subcategory=? ORDER BY Product_Name ASC"
        );
        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        sql::ResultSet* rs = pstmt->executeQuery();

        while (rs->next())
            ids.push_back(rs->getInt(1));

        delete rs;
        delete pstmt;
    } catch (...) {}
    return ids;
}

vector<int> sortProductsByStock(sql::Connection* con, const string &cat, const string &subcat) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category=? AND Subcategory=? ORDER BY Stock_Qtn DESC"
        );
        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        sql::ResultSet* rs = pstmt->executeQuery();

        while (rs->next())
            ids.push_back(rs->getInt(1));

        delete rs;
        delete pstmt;
    } catch (...) {}
    return ids;
}

vector<int> filterProductsByCompany(sql::Connection* con, const string &cat, const string &subcat, const string &company) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category=? AND Subcategory=? AND Company_name=?"
        );
        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        pstmt->setString(3, company);
        sql::ResultSet* rs = pstmt->executeQuery();

        while (rs->next())
            ids.push_back(rs->getInt(1));

        delete rs;
        delete pstmt;
    } catch (...) {}
    return ids;
}

vector<int> filterProductsByPriceRange(sql::Connection* con, const string &cat, const string &subcat, float low, float high) {
    vector<int> ids;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID FROM PRODUCT WHERE Category=? AND Subcategory=? AND Price BETWEEN ? AND ?"
        );
        pstmt->setString(1, cat);
        pstmt->setString(2, subcat);
        pstmt->setDouble(3, low);
        pstmt->setDouble(4, high);
        sql::ResultSet* rs = pstmt->executeQuery();

        while (rs->next())
            ids.push_back(rs->getInt(1));

        delete rs;
        delete pstmt;
    } catch (...) {}
    return ids;
}

// Functions.cpp
#include "Functions.h"
#include "DBFunctions.h"
#include "CartFunctions.h"
#include "login.h"
#include "WishlistFunctions.h"
#include "Address.h"

#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <sstream>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

// -------------------- ANSI COLOR HELPERS --------------------
static const string ANSI_RESET = "\033[0m";
static const string ANSI_BOLD = "\033[1m";
static const string ANSI_RED = "\033[31m";
static const string ANSI_GREEN = "\033[32m";
static const string ANSI_YELLOW = "\033[33m";
static const string ANSI_BLUE = "\033[34m";
static const string ANSI_MAGENTA = "\033[35m";
static const string ANSI_CYAN = "\033[36m";

std::string color(const std::string &code, const std::string &text) {
    return code + text + ANSI_RESET;
}

// -------------------- SCREEN HELPERS --------------------
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void pressEnterToContinue() {
    cout << color(ANSI_CYAN, "\nPress ENTER to continue...");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInput(const string &prompt) {
    int value;
    while (true) {
        cout << color(ANSI_YELLOW, prompt);
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << color(ANSI_RED, "Invalid input. Please enter a number.\n");
    }
}

void printFancyHeader(const string &title) {
    cout << color(ANSI_MAGENTA, "═══════════════════════════════════════════════════════════════════════════════\n");
    int pad = (79 - (int)title.size()) / 2;
    if (pad < 0) pad = 0;
    cout << string(pad, ' ') << color(ANSI_BLUE, "🛒  " + title) << "\n";
    cout << color(ANSI_MAGENTA, "═══════════════════════════════════════════════════════════════════════════════\n\n");
}

// -------------------- PRODUCT LIST UTILS --------------------
vector<int> showNumberedProducts(sql::Connection* con, const string &category, const string &subcategory) {
    vector<int> productIds;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(
            "SELECT Product_ID, Product_Name, Price, Stock_Qtn "
            "FROM PRODUCT WHERE Category = ? AND Subcategory = ? ORDER BY Product_ID"
        );
        pstmt->setString(1, category);
        pstmt->setString(2, subcategory);

        sql::ResultSet* res = pstmt->executeQuery();

        cout << left << setw(6) << "No."
             << setw(8) << "PID"
             << setw(40) << "Product Name"
             << setw(10) << "Price"
             << setw(8) << "Stock" << "\n";
        cout << string(70, '-') << "\n";

        int idx = 1;
        while (res->next()) {
            cout << left << setw(6) << idx
                 << setw(8) << res->getInt("Product_ID")
                 << setw(40) << res->getString("Product_Name")
                 << setw(10) << res->getDouble("Price")
                 << setw(8) << res->getInt("Stock_Qtn")
                 << "\n";

            productIds.push_back(res->getInt("Product_ID"));
            idx++;
        }

        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cerr << color(ANSI_RED, string("SQL Error: ") + e.what()) << endl;
    }
    return productIds;
}

// -------------------- PRODUCT DETAILS SCREEN --------------------
void productDetailsScreen(int productId, int customerId, sql::Connection* con) {
    clearScreen();
    printFancyHeader("PRODUCT DETAILS");

    displayProductDetailsById(productId);

    cout << "\n" << color(ANSI_CYAN, "Actions:") << "\n";
    cout << color(ANSI_GREEN, "1. View Supplier Details") << "\n";
    cout << color(ANSI_GREEN, "2. Add this product to Cart") << "\n";
    cout << color(ANSI_GREEN, "3. Add to Wishlist") << "\n";    // <-- NEW option
    cout << color(ANSI_GREEN, "0. Back") << "\n";

    int choice = getIntInput("Enter choice: ");

    if (choice == 1) {
        clearScreen();
        printFancyHeader("SUPPLIER DETAILS");
        displaySupplierForProduct(productId);
        pressEnterToContinue();
    }
    else if (choice == 2) {
        int qty = getIntInput("\nEnter quantity: ");
        bool ok = addToCart(con, customerId, productId, qty);
        if (ok) cout << color(ANSI_GREEN, "\n✅ Product added to cart.\n");
        else cout << color(ANSI_RED, "\n❌ Failed to add to cart.\n");
        pressEnterToContinue();
    }
    else if (choice == 3) {                                // <-- NEW handler
        bool ok = addToWishlist(con, customerId, productId);
        if (ok) cout << color(ANSI_GREEN, "\n✔ Added to wishlist.\n");
        else cout << color(ANSI_YELLOW, "\n⚠ Item may already be in wishlist or failed.\n");
        pressEnterToContinue();
    }
    else {
        // back
    }

}

// -------------------- SUBCATEGORY FLOW --------------------
void handleProductsFlow(sql::Connection *con, const string &category, const string &subcategory, int customerId) {
    bool back = false;

    while (!back) {
        clearScreen();
        printFancyHeader("PRODUCTS — " + category + " → " + subcategory);

        // Load default list
        vector<int> productIds = showNumberedProducts(con, category, subcategory);

        if (productIds.empty()) {
            cout << color(ANSI_YELLOW, "No products in this subcategory.\n");
            pressEnterToContinue();
            return;
        }

        cout << "\n" << color(ANSI_CYAN, "Options:") << "\n";
        cout << "1) View Product Details\n";
        cout << "2) Sort Products\n";
        cout << "3) Filter Products\n";
        cout << "0) Back\n";

        int ch = getIntInput("Enter choice: ");

        // -------------------- BACK --------------------
        if (ch == 0) {
            back = true;
        }

        // -------------------- VIEW PRODUCT DETAILS --------------------
        else if (ch == 1) {
            int num = getIntInput("Enter item number: ");

            if (num >= 1 && num <= (int)productIds.size()) {
                int selectedId = productIds[num - 1];
                productDetailsScreen(selectedId, customerId, con);
            } else {
                cout << color(ANSI_RED, "Invalid item number.\n");
                pressEnterToContinue();
            }
        }

        // -------------------- SORT PRODUCTS --------------------
        else if (ch == 2) {
            clearScreen();
            printFancyHeader("SORT PRODUCTS");

            cout << "1) Price Low → High\n";
            cout << "2) Price High → Low\n";
            cout << "3) Name A → Z\n";
            cout << "4) Stock High → Low\n";
            cout << "0) Back\n";

            int s = getIntInput("Choose sorting option: ");
            if (s == 0) continue;

            if (s == 1)
                productIds = sortProductsByPriceAsc(con, category, subcategory);
            else if (s == 2)
                productIds = sortProductsByPriceDesc(con, category, subcategory);
            else if (s == 3)
                productIds = sortProductsByName(con, category, subcategory);
            else if (s == 4)
                productIds = sortProductsByStock(con, category, subcategory);
            else {
                cout << color(ANSI_RED, "Invalid choice.\n");
                pressEnterToContinue();
                continue;
            }

            // ---------- SHOW SORTED RESULTS ----------
            clearScreen();
            printFancyHeader("SORTED PRODUCTS");

            cout << left
                 << setw(6)  << "No."
                 << setw(10) << "PID"
                 << setw(30) << "Name"
                 << setw(10) << "Stock"
                 << setw(10) << "Price"
                 << setw(15) << "Expiry"
                 << setw(20) << "Supplier" << "\n";

            cout << string(110, '-') << "\n";

            for (int i = 0; i < productIds.size(); i++)
                displayCompactProductRow(con, productIds[i], i + 1);

            pressEnterToContinue();
            continue;
        }

        // -------------------- FILTER PRODUCTS --------------------
        else if (ch == 3) {
            clearScreen();
            printFancyHeader("FILTER PRODUCTS");

            cout << "1) By Company Name\n";
            cout << "2) By Price Range\n";
            cout << "0) Back\n";

            int f = getIntInput("Choose filter option: ");
            if (f == 0) continue;

            if (f == 1) {
                cout << "Enter company name: ";
                string comp;
                getline(cin, comp);
                if (comp.empty()) getline(cin, comp);

                productIds = filterProductsByCompany(con, category, subcategory, comp);
            }
            else if (f == 2) {
                float low = getIntInput("Enter minimum price: ");
                float high = getIntInput("Enter maximum price: ");
                productIds = filterProductsByPriceRange(con, category, subcategory, low, high);
            }
            else {
                cout << color(ANSI_RED, "Invalid choice.\n");
                pressEnterToContinue();
                continue;
            }

            // ---------- SHOW FILTERED RESULTS ----------
            clearScreen();
            printFancyHeader("FILTERED PRODUCTS");

            if (productIds.empty()) {
                cout << color(ANSI_YELLOW, "No products match this filter.\n");
                pressEnterToContinue();
                continue;
            }

            cout << left
                 << setw(6)  << "No."
                 << setw(10) << "PID"
                 << setw(30) << "Name"
                 << setw(10) << "Stock"
                 << setw(10) << "Price"
                 << setw(15) << "Expiry"
                 << setw(20) << "Supplier" << "\n";

            cout << string(110, '-') << "\n";

            for (int i = 0; i < productIds.size(); i++)
                displayCompactProductRow(con, productIds[i], i + 1);

            pressEnterToContinue();
            continue;
        }

        // -------------------- INVALID --------------------
        else {
            cout << color(ANSI_RED, "Invalid choice.\n");
            pressEnterToContinue();
        }
    }
}


// -------------------- SUBCATEGORY MENU --------------------
void handleSubcategoryFlow(sql::Connection *con, const string &category, int customerId) {
    bool back = false;
    while (!back) {
        clearScreen();
        printFancyHeader("SUBCATEGORIES — " + category);

        vector<string> subcats = loadSubcategories(con, category);

        if (subcats.empty()) {
            cout << "No subcategories.\n1) View all products\n0) Back\n";
            int ch = getIntInput("Enter: ");
            if (ch == 1) {
                clearScreen();
                displayByCategory(con, category);
                pressEnterToContinue();
            } else back = true;
            continue;
        }

        for (int i = 0; i < (int)subcats.size(); i++)
            cout << (i+1) << ") " << subcats[i] << "\n";
        cout << "0) Back\n";

        int ch = getIntInput("Enter: ");
        if (ch == 0) back = true;
        else if (ch >= 1 && ch <= (int)subcats.size())
            handleProductsFlow(con, category, subcats[ch-1], customerId);
    }
}

// -------------------- CATEGORY MENU --------------------
void runCategoryMenu(sql::Connection *con, int customerId) {
    bool exitProgram = false;

    while (!exitProgram) {
        clearScreen();
        printFancyHeader("CATEGORY MENU");

        // OPTION 1 – My Profile
        cout << color(ANSI_CYAN, "1) My Profile\n");

        // OPTION 2 – Search Products
        cout << color(ANSI_CYAN, "2) Search Products\n\n");

        // Load categories
        vector<string> categories = loadCategories(con);

        int offset = 2; // 1 = My Profile, 2 = Search

        cout << color(ANSI_CYAN, "Available Categories:\n\n");
        for (size_t i = 0; i < categories.size(); ++i) {
            cout << "  " << color(ANSI_YELLOW, to_string((int)i + offset + 1))
                 << ") " << categories[i] << "\n";
        }

        int viewAllIndex = (int)categories.size() + offset + 1;
        int viewCartIndex = viewAllIndex + 1;

        cout << "\n  " << color(ANSI_YELLOW, to_string(viewAllIndex)) << ") View ALL Products\n";
        cout << "  " << color(ANSI_YELLOW, to_string(viewCartIndex)) << ") View Cart\n";
        cout << "  0) Exit\n\n";
        cout << "---------------------------------------\n";

        int choice = getIntInput("👉 Enter your choice: ");

        if (choice == 0) {
            exitProgram = true;
        }
        else if (choice == 1) {
            Customer c;
            string emailFromDB = "";
            if (getCustomerById(con, customerId, c)) emailFromDB = c.getEmail();
            showMyProfile(con, customerId, emailFromDB);
        }
        else if (choice == 2) {
            runSearchMenu(con, customerId);  // ← CALL SEARCH HERE
        }
        else if (choice == viewAllIndex) {
            clearScreen();
            printFancyHeader("ALL PRODUCTS");
            displayAllProducts(con);
            pressEnterToContinue();
        }
        else if (choice == viewCartIndex) {
            runCartMenu(con, customerId);
        }
        else {
            int catIndex = choice - offset - 1;
            if (catIndex >= 0 && catIndex < (int)categories.size()) {
                string selectedCategory = categories[catIndex];
                handleSubcategoryFlow(con, selectedCategory, customerId);
            } else {
                cout << color(ANSI_RED, "Invalid choice. Try again.\n");
                pressEnterToContinue();
            }
        }
    }
}


// -------------------- MY PROFILE --------------------
void showMyProfile(sql::Connection* con, int customerId, const std::string &loggedInEmail) {
    while (true) {
        clearScreen();
        printFancyHeader("MY PROFILE");

        cout << "1) View Personal Details\n";
        cout << "2) My Orders\n";
        cout << "3) Edit Profile\n";
        cout << "4) Manage Addresses\n";
        cout << "0) Back\n";

        int ch = getIntInput("Enter: ");
        if (ch == 0) return;

        if (ch == 1) {
            Customer c;
            bool ok = getCustomerById(con, customerId, c);

            clearScreen();
            printFancyHeader("PERSONAL DETAILS");

            if (ok) {
                cout << "Name        : " << c.getName() << "\n";
                cout << "Email       : " << c.getEmail() << "\n";
                cout << "Contact No. : " << c.getContactnumber() << "\n";
                cout << "Address     : " << c.getAddress() << "\n\n";
            } else {
                cout << "No details found.\n";
            }

            pressEnterToContinue();
        }

        else if (ch == 2) {
            showMyOrders(con, customerId);
        }

        else if (ch == 3) {
            editProfileMenu(con, customerId, loggedInEmail);
        }
        else if (ch == 4) {
            manageAddresses(con, customerId);
        }

    }
}

// -------------------- EDIT PROFILE --------------------
void editProfileMenu(sql::Connection* con, int customerId, const std::string &loggedInEmail) {
    while (true) {
        clearScreen();
        printFancyHeader("EDIT PROFILE");

        cout << "1) Edit Name\n";
        cout << "2) Edit Contact Number\n";
        cout << "3) Edit Address\n";
        cout << "4) Change Password\n";
        cout << "0) Back\n";

        int ch = getIntInput("Enter: ");
        if (ch == 0) return;

        Customer c;
        if (!getCustomerById(con, customerId, c)) {
            cout << "Record not found.\n";
            pressEnterToContinue();
            return;
        }

        if (ch == 1) {
            cout << "Current Name: " << c.getName() << "\n";
            cout << "Enter new name: ";
            string s;
            getline(cin, s);
            if (s.empty()) getline(cin, s);
            c.setName(s);
            updateCustomerDetails(con, c);
            cout << "Updated.\n";
            pressEnterToContinue();
        }

        else if (ch == 2) {
            cout << "Current Contact: " << c.getContactnumber() << "\n";
            cout << "Enter new contact: ";
            string s;
            getline(cin, s);
            if (s.empty()) getline(cin, s);
            c.setContactnumber(s);
            updateCustomerDetails(con, c);
            cout << "Updated.\n";
            pressEnterToContinue();
        }

        else if (ch == 3) {
            cout << "Current Address: " << c.getAddress() << "\n";
            cout << "Enter new address: ";
            string s;
            getline(cin, s);
            if (s.empty()) getline(cin, s);
            c.setAddress(s);
            updateCustomerDetails(con, c);
            cout << "Updated.\n";
            pressEnterToContinue();
        }

        else if (ch == 4) {
            cout << "Enter current password: ";
            string oldPwd;
            getline(cin, oldPwd);
            if (oldPwd.empty()) getline(cin, oldPwd);

            if (!authenticateLogin(con, loggedInEmail, oldPwd)) {
                cout << color(ANSI_RED, "Wrong password!\n");
                pressEnterToContinue();
                continue;
            }

            cout << "Enter new password: ";
            string newPwd;
            getline(cin, newPwd);
            if (newPwd.empty()) getline(cin, newPwd);

            if (newPwd.size() < 6) {
                cout << "Too short.\n";
                pressEnterToContinue();
                continue;
            }

            updateLoginPassword(con, loggedInEmail, newPwd);
            cout << "Password updated.\n";
            pressEnterToContinue();
        }
    }
}

// -------------------- MY ORDERS LIST --------------------
void showMyOrders(sql::Connection* con, int customerId) {
    clearScreen();
    printFancyHeader("MY ORDERS");

    vector<Order> orders = loadOrdersForCustomer(con, customerId);

    if (orders.empty()) {
        cout << "You have no orders yet.\n";
        pressEnterToContinue();
        return;
    }

    for (int i = 0; i < orders.size(); i++)
        cout << (i+1) << ") Order " << (i+1) 
             << " — " << orders[i].getOrderDate() << "\n";

    cout << "0) Back\n";

    int choice = getIntInput("Enter order number: ");
    if (choice == 0) return;
    if (choice < 0 || choice > orders.size()) {
        cout << "Invalid.\n";
        pressEnterToContinue();
        return;
    }

    int orderId = orders[choice-1].getOrderId();
    showOrderDetails(con, orderId, choice);
}

// -------------------- ORDER DETAILS --------------------
void showOrderDetails(sql::Connection* con, int orderId, int orderNumber) {
    clearScreen();
    printFancyHeader("ORDER DETAILS");

    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("SELECT OrderID, OrderDate, DeliveryDate, TotalAmount FROM Orders WHERE OrderID=?");
        pstmt->setInt(1, orderId);

        sql::ResultSet* res = pstmt->executeQuery();

        if (!res->next()) {
            cout << "Order not found.\n";
            delete res; delete pstmt;
            pressEnterToContinue();
            return;
        }

        cout << "Order Number  : " << orderNumber << "\n";
        cout << "Order ID      : " << res->getInt("OrderID") << "\n";
        cout << "Order Date    : " << res->getString("OrderDate") << "\n";
        cout << "Delivery Date : " << res->getString("DeliveryDate") << "\n";
        cout << "Total Amount  : " << res->getDouble("TotalAmount") << "\n\n";

        int oid = res->getInt("OrderID");
        delete res;
        delete pstmt;

        vector<OrderItem> items = loadOrderItems(con, oid);

        cout << left << setw(6) << "No."
             << setw(35) << "Product"
             << setw(8) << "Qty"
             << setw(12) << "Price"
             << setw(12) << "Subtotal" << "\n";

        cout << string(75, '-') << "\n";

        int totalItems = 0;

        for (int i = 0; i < items.size(); i++) {
            cout << left << setw(6) << (i+1)
                 << setw(35) << items[i].getProductName()
                 << setw(8) << items[i].getQuantity()
                 << setw(12) << items[i].getPrice()
                 << setw(12) << items[i].getSubtotal()
                 << "\n";

            totalItems += items[i].getQuantity();
        }

        cout << string(75, '-') << "\n";
        cout << "Total Items : " << totalItems << "\n";

    } catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << "\n";
    }

    pressEnterToContinue();
}

void runSearchMenu(sql::Connection* con, int customerId) {
    while (true) {
        clearScreen();
        printFancyHeader("SEARCH PRODUCTS");

        cout << "1) Search by Name\n";
        cout << "2) Search by Company/Brand\n";
        cout << "3) Search by Category\n";
        cout << "4) Search by Price Range\n";
        cout << "0) Back\n\n";

        int ch = getIntInput("Enter choice: ");
        if (ch == 0) return;

        vector<int> results;

        if (ch == 1) {
            cout << "Enter product name: ";
            string name; getline(cin, name);
            results = searchProductsByName(con, name);
        }
        else if (ch == 2) {
            cout << "Enter company/brand: ";
            string brand; getline(cin, brand);
            results = searchProductsByCompany(con, brand);
        }
        else if (ch == 3) {
            cout << "Enter category: ";
            string cat; getline(cin, cat);
            results = searchProductsByCategory(con, cat);
        }
        else if (ch == 4) {
            cout << "Enter minimum price: ";
            double minP; cin >> minP; cin.ignore();
            cout << "Enter maximum price: ";
            double maxP; cin >> maxP; cin.ignore();
            results = searchProductsByPriceRange(con, minP, maxP);
        }
        else {
            cout << "Invalid choice.\n";
            pressEnterToContinue();
            continue;
        }

        clearScreen();
        printFancyHeader("SEARCH RESULTS");

        if (results.empty()) {
            cout << "No products match your search.\n";
            pressEnterToContinue();
            continue;
        }

        cout << left
            << setw(6)  << "No."
            << setw(10) << "PID"
            << setw(30) << "Name"
            << setw(10) << "Stock"
            << setw(10) << "Price"
            << setw(15) << "Expiry"
            << setw(20) << "Supplier"
            << "\n";

        cout << string(105, '-') << "\n";

        for (int i = 0; i < results.size(); ++i) {
            displayCompactProductRow(con, results[i], i + 1);
        }


        cout << "\nEnter number to view product, or 0 to go back: ";
        int pick = getIntInput("");

        if (pick == 0) continue;
        if (pick < 1 || pick > results.size()) {
            cout << "Invalid selection.\n";
            pressEnterToContinue();
            continue;
        }

        productDetailsScreen(results[pick - 1], customerId, con);
    }
}

void manageAddresses(sql::Connection* con, int customerId) {
    while (true) {
        clearScreen();
        printFancyHeader("MANAGE ADDRESSES");

        cout << "1) Add New Address\n";
        cout << "2) View Saved Addresses\n";
        cout << "3) Delete Address\n";
        cout << "0) Back\n";

        int ch = getIntInput("Enter option: ");
        if (ch == 0) return;

        // ----------------- ADD NEW ADDRESS -----------------
        if (ch == 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // FIX INPUT

            string line, city, state, pin;

            cout << "Enter Address Line: ";
            getline(cin, line);

            cout << "City: ";
            getline(cin, city);

            cout << "State: ";
            getline(cin, state);

            cout << "Pincode: ";
            getline(cin, pin);

            bool ok = addAddress(con, customerId, line, city, state, pin);

            if (ok) cout << color(ANSI_GREEN, "\n✔ Address added.\n");
            else    cout << color(ANSI_RED, "\n❌ Failed to add address.\n");

            pressEnterToContinue();
        }

        // ----------------- VIEW SAVED ADDRESSES -----------------
        else if (ch == 2) {
            vector<pair<int,string>> addrs = loadAddresses(con, customerId);

            clearScreen();
            printFancyHeader("SAVED ADDRESSES");

            if (addrs.empty()) {
                cout << color(ANSI_YELLOW, "No saved addresses.\n");
            } else {
                for (int i = 0; i < addrs.size(); i++) {
                    cout << (i+1) << ") " 
                         << addrs[i].second
                         << " (ID: " << addrs[i].first << ")\n";
                }
            }
            pressEnterToContinue();
        }

        // ----------------- DELETE ADDRESS -----------------
        else if (ch == 3) {
            int id = getIntInput("Enter Address ID to delete (0 to cancel): ");
            if (id == 0) continue;

            bool ok = deleteAddress(con, id);

            if (ok) cout << color(ANSI_GREEN, "✔ Address deleted.\n");
            else    cout << color(ANSI_RED, "❌ Failed. Check ID.\n");

            pressEnterToContinue();
        }

        else {
            cout << color(ANSI_RED, "Invalid choice.\n");
            pressEnterToContinue();
        }
    }
}

int selectDeliveryAddress(sql::Connection* con, int customerId) {
    clearScreen();
    printFancyHeader("SELECT DELIVERY ADDRESS");

    vector<Address> addresses = loadFullAddresses(con, customerId);

    if (addresses.empty()) {
        cout << color(ANSI_RED, "❌ No saved addresses.\n");
        cout << "Add a new address in My Profile → Manage Addresses.\n";
        pressEnterToContinue();
        return -1;
    }

    cout << "Choose an address:\n\n";

    for (int i = 0; i < addresses.size(); i++) {
        cout << (i+1) << ") "
             << addresses[i].getLine() << ", "
             << addresses[i].getCity() << ", "
             << addresses[i].getState() << " "
             << addresses[i].getPostalCode() << "\n";

        if (addresses[i].isDefault())
            cout << "   " << color(ANSI_GREEN, "(Default)") << "\n";

        cout << "\n";
    }

    cout << "0) Cancel\n\n";

    int choice = getIntInput("Enter choice: ");

    if (choice == 0)
        return -1;

    if (choice < 1 || choice > addresses.size())
        return -1;

    return addresses[choice - 1].getId();
}

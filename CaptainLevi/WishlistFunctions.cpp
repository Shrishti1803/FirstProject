#include "WishlistFunctions.h"
#include <iostream>
#include <iomanip>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

// ADD TO WISHLIST
bool addToWishlist(sql::Connection* con, int customerId, int productId) {
    try {
        sql::PreparedStatement* check =
            con->prepareStatement("SELECT * FROM WISHLIST WHERE CustomerID = ? AND ProductID = ?");
        check->setInt(1, customerId);
        check->setInt(2, productId);

        sql::ResultSet* res = check->executeQuery();
        if (res->next()) {
            cout << "⚠ Already in wishlist.\n";
            delete res; delete check;
            return false;
        }
        delete res; delete check;

        sql::PreparedStatement* pstmt =
            con->prepareStatement("INSERT INTO WISHLIST(CustomerID, ProductID) VALUES(?, ?)");
        pstmt->setInt(1, customerId);
        pstmt->setInt(2, productId);

        pstmt->executeUpdate();
        delete pstmt;

        cout << "✔ Added to wishlist.\n";
        return true;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in addToWishlist: " << e.what() << endl;
        return false;
    }
}

// REMOVE
bool removeFromWishlist(sql::Connection* con, int customerId, int productId) {
    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement("DELETE FROM WISHLIST WHERE CustomerID = ? AND ProductID = ?");
        pstmt->setInt(1, customerId);
        pstmt->setInt(2, productId);

        int rows = pstmt->executeUpdate();
        delete pstmt;

        if (rows > 0) {
            cout << "✔ Removed from wishlist.\n";
            return true;
        }

        cout << "⚠ Not in wishlist.\n";
        return false;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in removeFromWishlist: " << e.what() << endl;
        return false;
    }
}

// VIEW WISHLIST
vector<Wishlist> viewWishlist(sql::Connection* con, int customerId) {
    vector<Wishlist> items;

    try {
        sql::PreparedStatement* pstmt =
            con->prepareStatement(
                "SELECT W.WishlistID, W.ProductID, P.Product_Name, P.Price "
                "FROM WISHLIST W JOIN PRODUCT P ON W.ProductID = P.Product_ID "
                "WHERE W.CustomerID = ?"
            );
        pstmt->setInt(1, customerId);

        sql::ResultSet* res = pstmt->executeQuery();

        cout << "\n========== YOUR WISHLIST ==========\n";
        cout << left << setw(6) << "No."
             << setw(10) << "PID"
             << setw(35) << "Product Name"
             << setw(10) << "Price"
             << endl;
        cout << string(70, '-') << endl;

        int index = 1;
        while (res->next()) {
            int wid = res->getInt("WishlistID");
            int pid = res->getInt("ProductID");
            string name = res->getString("Product_Name");
            double price = res->getDouble("Price");

            cout << left << setw(6) << index
                 << setw(10) << pid
                 << setw(35) << name
                 << setw(10) << price
                 << endl;

            items.push_back(Wishlist(wid, customerId, pid));
            index++;
        }

        delete res;
        delete pstmt;
    }
    catch (sql::SQLException &e) {
        cerr << "SQL Error in viewWishlist: " << e.what() << endl;
    }

    return items;
}

// WISHLIST MENU
void runWishlistMenu(sql::Connection* con, int customerId) {
    while (true) {
        cout << "\n========== WISHLIST MENU ==========\n";
        cout << "1) View Wishlist\n";
        cout << "2) Remove Item\n";
        cout << "0) Back\n";
        cout << "Enter choice: ";

        int ch;
        cin >> ch;

        if (ch == 0) return;

        if (ch == 1) {
            viewWishlist(con, customerId);
        }
        else if (ch == 2) {
            int pid;
            cout << "Enter Product ID to remove: ";
            cin >> pid;
            removeFromWishlist(con, customerId, pid);
        }
        else {
            cout << "❌ Invalid choice.\n";
        }
    }
}

#include <iostream>
using namespace std;

// Include headers
#include "Supplier.h"
#include "Functions.h"

// Include cpp implementations
#include "Supplier.cpp"
#include "Functions.cpp"

// This menu will just display all the Products list available on the store 

int main() {

    try {
        sql::Connection *con = createConnection();

        displayAllProducts(con);
    
        delete con;
    }
    catch (sql::SQLException &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
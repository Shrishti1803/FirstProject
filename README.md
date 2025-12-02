📦 PROJECT1 – Console-Based E-Commerce Application

A complete mini-ecommerce system built using C++, MySQL Connector/C++, and modular OOP design.

🚀 Overview

PROJECT1 is a fully interactive console-based e-commerce system that allows customers to browse products, add items to cart, place orders, manage profiles, and track wishlist items.

The project simulates a real-world shopping experience, complete with:

Categories & Subcategories

Product details + supplier details

Cart system

Checkout process with stock update

Order history with delivery date

Search, sorting, filtering

Wishlist

Profile editing

Login system (new + existing user flow)

🏗 Project Structure
PROJECT1/
│
├── CaptainLevi/            ← All .cpp implementation files
│   ├── FinalMain.cpp
│   ├── DBFunctions.cpp
│   ├── Functions.cpp
│   ├── CartFunctions.cpp
│   ├── Product.cpp
│   ├── Customer.cpp
│   ├── Supplier.cpp
│   ├── Order.cpp
│   ├── OrderItem.cpp
│   ├── login.cpp
│   └── Date.cpp
│
├── MissionPlan/            ← All headers
│   ├── DBFunctions.h
│   ├── Functions.h
│   ├── CartFunctions.h
│   ├── Product.h
│   ├── Customer.h
│   ├── Supplier.h
│   ├── Order.h
│   ├── OrderItem.h
│   ├── login.h
│   └── Date.h
│
└── README.md

🧪 Database Structure (MySQL)
Database Name: PROJECT1
Tables Used
1. PRODUCT
Product_ID (PK)
Product_Name
Category
Subcategory
Price
Stock_Qtn
Company_name
ExpiryDate
SID (FK → Supplier.SID)

2. CUSTOMER_DETAILS
ID (PK)
Name
Contact_Num
Email
Address

3. LOGIN
Email (PK)
Password

4. Supplier
SID (PK)
Sname
Contact_Num
Email
Address

5. Cart
CartID (PK)
Customer_ID (FK)
Product_ID (FK)
Quantity

6. Orders
OrderID (PK)
Customer_ID (FK)
OrderDate
DeliveryDate
TotalAmount

7. OrderItems
ItemID (PK)
OrderID (FK)
Product_ID (FK)
Product_Name
Price
Quantity
Subtotal

8. Wishlist
WishlistID (PK)
Customer_ID (FK)
Product_ID (FK)

🔐 Login Flow
New User

User chooses New User

Creates login (email + password)

Enters customer details

Customer ID auto-generated

Redirects to main shopping menu

Existing User

Enters email & password

Credentials verified

Customer ID loaded

Redirects to main menu

🛍 Customer Features
🔸 1. Category & Subcategory Browsing

Dynamic loading of categories

For each category → subcategories

For each subcategory → products shown with numbering

Cleaner UI with colored formatting

🔸 2. Product Details

Detailed view

Stock

Price

Expiry

Brand

Supplier info (shown on request)

🔸 3. Cart System

Add to Cart

Update quantity

Remove item

View cart

Checkout

After checkout:

Stock reduced automatically

Order created

Cart cleared

🔸 4. Orders & Order History

Every checkout creates:

An entry in Orders table

Corresponding items in OrderItems table

Order date

Delivery date = OrderDate + 4 days

My Orders:

Shows list of past orders

User selects which order to view

Detailed view contains:

Items

Total amount

Delivery date

Product names, quantities, prices

🔸 5. Wishlist

Add any product to wishlist

Remove from wishlist

View wishlist anytime

🔸 6. Search System

You can search by:

Product name

Category

Company/Brand

Price range

Displays matching products as numbered list.

🔸 7. Sorting

Sorting options inside product list:

Price: Low → High

Price: High → Low

Name: A → Z

Stock: High → Low

🔸 8. Filtering

Filters available:

Filter by Company/Brand

Filter by Price Range

🔸 9. Profile Management

Inside "My Profile":

View personal details

Edit name

Edit address

Edit contact number

Change password (with current password verification)

View all orders

🖥 How to Compile & Run
Compile
g++ -o final CaptainLevi/*.cpp -I./MissionPlan -I/usr/include/mysql-cppconn-8/ -lmysqlcppconn

Run
./final

🎯 Future Planned Features

(Not implemented yet)

Admin Dashboard

Product management (add/update/delete)

Sales report / analytics

Advanced return/cancellation policy

Desktop application (GUI)

📝 Conclusion

PROJECT1 is a complete modular backend for a console-based e-commerce system.
It includes almost every core feature of real-world online shopping platforms — implemented using C++, MySQL, and clean modular architecture.


👤 Author

Shrishti —C++ with MySQL
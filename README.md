# 🛒 PROJECT1 — C++ Console E-Commerce Application  
A fully functional, production-style **E-Commerce Console Application** written in **C++** with **MySQL Database Integration**.  
This project includes a complete customer system and is now evolving into a full admin panel.

---

## 🚀 Project Overview
PROJECT1 is a modular e-commerce backend built entirely in **C++**, integrating with **MySQL** for persistent storage.  
The application includes:

- Customer authentication  
- Product browsing (category → subcategory)
- Full-text search  
- Sorting & filtering  
- Wishlist  
- Cart system  
- Checkout & orders  
- Address management  
- Supplier linking  
- Professional UI (terminal-based)

The project follows a **clean folder structure** with `src/` for source files and `include/` for headers.

---

## 🧩 Features (Customer Side)

### 🔐 **1. Authentication System**
- New customer registration  
- Secure login  
- Password verification  
- Profile editing (name, contact, address, password)

---

### 🛍 **2. Product Browsing**
- Category → Subcategory navigation  
- Displays product lists in clean, formatted table view  
- Shows:
  - Product ID  
  - Product Name  
  - Price  
  - Stock  
  - Expiry Date  
  - Supplier Name  

---

### 🔎 **3. Search System**
Search by:
- Product Name  
- Brand/Company  
- Category  
- Price Range  

All results displayed with complete product details.

---

### ↕ **4. Sorting**
Sort products by:
- Price (Low → High)  
- Price (High → Low)  
- Name (A → Z)  
- Stock (High → Low)

---

### 🎯 **5. Filters**
Filter products by:
- Company/Brand  
- Price Range  

---

### 💖 **6. Wishlist System**
- Add products to wishlist  
- Remove items  
- View wishlist with full details

---

### 🛒 **7. Cart System**
- Add to cart  
- Update quantity  
- Remove items  
- Auto-subtotal calculation  
- View cart summary  
- Stock verification  
- Cart clearing on checkout  

---

### 🚚 **8. Address Management**
- Add new address  
- View saved addresses  
- Delete address  
- Choose delivery address during checkout  
- Supports multiple addresses per customer  

---

### 📦 **9. Orders System**
- Place orders  
- Auto delivery date generation  
- View past orders  
- View order details + item breakdown  
- Order total calculation  

---

## 📁 Folder Structure

PROJECT1/
│
├── src/ # All .cpp implementation files
│ ├── FinalMain.cpp
│ ├── Functions.cpp
│ ├── DBFunctions.cpp
│ ├── Product.cpp
│ ├── Customer.cpp
│ ├── WishlistFunctions.cpp
│ ├── CartFunctions.cpp
│ ├── Address.cpp
│ ├── Order.cpp
│ └── (other modules)
│
├── include/ # All .h header files
│ ├── Functions.h
│ ├── DBFunctions.h
│ ├── Product.h
│ ├── Customer.h
│ ├── WishlistFunctions.h
│ ├── CartFunctions.h
│ ├── Address.h
│ ├── Order.h
│ └── login.h
|
|── final 
│
└── README.md


---

## 🛠 Technology Stack

### **Languages**
- C++17 / C++20  
- SQL (MySQL)

### **Database**
- MySQL 8.x  
- Tables include:
  - PRODUCT  
  - CUSTOMER_DETAILS  
  - Supplier  
  - Wishlist  
  - Cart  
  - Orders  
  - OrderItems  
  - Addresses  
  - LOGIN  

### **Libraries**
- MySQL Connector/C++ (`mysql-cppconn-8`)  
- Standard C++ STL  

---

## 🔧 How to Build & Run

### **1. Install MySQL Connector/C++**
Ubuntu:
```bash
sudo apt install libmysqlcppconn-dev


2. Compile the project

From project root:

g++ -o final src/*.cpp -I./include -I/usr/include/mysql-cppconn-8/ -lmysqlcppconn

3. Run
./final



📌 Database Requirements
Before running, ensure you have the required tables:
CUSTOMER_DETAILS
PRODUCT
Supplier
Wishlist
Cart
Address
Orders
OrderItems
LOGIN

Your schema includes:
Product Categories & Subcategories
400 seeded products
20 suppliers
Login table for customer auth


🧱 Architecture Overview
Customer Flow
Start → Login/Register → Category Menu → Subcategory → Product Details  
     → Add to Cart/Wishlist → Cart → Checkout → Orders → Logout

Backend Modules
DBFunctions.cpp: All SQL operations
Functions.cpp: UI logic, menus, display tables
Order.cpp: Order & order-item handling
Address.cpp: Multi-address system
WishlistFunctions.cpp: Wishlist logic
CartFunctions.cpp: Cart operations


📘 Future Scope (Planned)

🔐 Admin Panel
Admin login
Product management (CRUD)
Supplier management
Category/subcategory management
View all orders
Update order status
Customer management

🌐 Web Version
Backend using Drogon (C++ Web Framework)
Frontend using React.js
REST APIs
JSON responses


✨ Why This Project Stands Out
3500+ lines of organized, modular C++ code
Professional folder structure
Real-world e-commerce logic
SQL-backed persistence
Clean UI with ANSI formatting
Scalable architecture
Perfect for major college projects & resumes


👩‍💻 Author
** Shrishti **
BCA Student & Developer
Passionate about C++, Databases, and real-world software engineering.


⭐ Support
If you like this project, feel free to ⭐ star the repository on GitHub!
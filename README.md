# Supermarket Management System — C++

A command-line supermarket management system written in C++17 with SQLite storage. It supports role-based logins, inventory management, sales processing and text report generation.

## Features
- Role-based access: Admin, SalesPerson, InventoryPerson, Cashier
- Inventory: add, remove, edit, list and find products by barcode
- Sales: process sales and view sales history
- Reports: generate sales and inventory reports as `.txt` files
- Admin user management

## Requirements
- A C++17 compiler (g++ or clang++)
- SQLite3 development library
  - Debian/Ubuntu: `sudo apt install libsqlite3-dev`
  - macOS: `brew install sqlite`

## Build and Run
```bash
make
./supermarket
```

Or compile manually:
```bash
g++ -std=c++17 main.cpp database.cpp user.cpp inventory.cpp sales.cpp reports.cpp dashboard.cpp -o supermarket -lsqlite3
```

### On Replit
1. Create a Repl and choose **C++**
2. Upload all the files
3. In the Shell tab, run `make` then `./supermarket`

## Default Login
| Username | Password | Role  |
|----------|----------|-------|
| admin    | admin123 | Admin |

Use the Admin account to create other users (SalesPerson, Cashier, etc.).

> **Note:** this is a learning project. Passwords are stored in plain text and the default admin credentials are public, so don't use it as-is in a real store.

## File Structure
```
main.cpp         → entry point
database.h/cpp   → SQLite connection (singleton)
product.h        → Product data struct
user.h/cpp       → User base class + Admin/SalesPerson/InventoryPerson/Cashier
inventory.h/cpp  → Add/Remove/Edit/List/FindByBarcode
sales.h/cpp      → Process sales, view history
reports.h/cpp    → Generate .txt reports
dashboard.h/cpp  → Login + routes menus by role
Makefile         → build with: make
```

## Data
The database (`supermarket.db`) and the generated reports are created in the folder you run the program from. They are git-ignored.

#include "inventory.h"
#include <iostream>
#include <iomanip>
#include <sqlite3.h>

Inventory::Inventory() {
    db = Database::getInstance();
}

// ─── Add Item ─────────────────────────────────────────────────────────────────
void Inventory::addItem() {
    std::string item, brand, barcode;
    double price;
    int quantity;

    std::cout << "\n--- Add New Item ---\n";
    std::cout << "Item name  : "; std::cin.ignore(); std::getline(std::cin, item);
    std::cout << "Brand      : "; std::getline(std::cin, brand);
    std::cout << "Price      : "; std::cin >> price;
    std::cout << "Quantity   : "; std::cin >> quantity;
    std::cout << "Barcode    : "; std::cin >> barcode;

    std::string sql = "INSERT INTO products (item, brand, price, quantity, barcode) "
                      "VALUES ('" + item + "','" + brand + "'," +
                      std::to_string(price) + "," + std::to_string(quantity) +
                      ",'" + barcode + "');";

    if (db->execute(sql))
        std::cout << "✔ Item added successfully.\n";
    else
        std::cout << "✘ Failed to add item (barcode may already exist).\n";
}

// ─── Remove Item ──────────────────────────────────────────────────────────────
void Inventory::removeItem() {
    int id;
    std::cout << "\n--- Remove Item ---\n";
    listAllItems();
    std::cout << "Enter product ID to remove: "; std::cin >> id;

    std::string sql = "DELETE FROM products WHERE id=" + std::to_string(id) + ";";
    if (db->execute(sql))
        std::cout << "✔ Item removed.\n";
    else
        std::cout << "✘ Could not remove item.\n";
}

// ─── Edit Quantity ────────────────────────────────────────────────────────────
void Inventory::editQuantity() {
    int id, qty;
    std::cout << "\n--- Edit Quantity ---\n";
    listAllItems();
    std::cout << "Enter product ID : "; std::cin >> id;
    std::cout << "New quantity     : "; std::cin >> qty;

    std::string sql = "UPDATE products SET quantity=" + std::to_string(qty) +
                      " WHERE id=" + std::to_string(id) + ";";
    if (db->execute(sql))
        std::cout << "✔ Quantity updated.\n";
    else
        std::cout << "✘ Update failed.\n";
}

// ─── List All Items ───────────────────────────────────────────────────────────
void Inventory::listAllItems() {
    std::cout << "\n" << std::left
              << std::setw(5)  << "ID"
              << std::setw(20) << "Item"
              << std::setw(15) << "Brand"
              << std::setw(10) << "Price"
              << std::setw(10) << "Qty"
              << std::setw(15) << "Barcode" << "\n";
    std::cout << std::string(75, '-') << "\n";

    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, item, brand, price, quantity, barcode FROM products;";
    sqlite3_prepare_v2(db->getDB(), sql.c_str(), -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << std::left
                  << std::setw(5)  << sqlite3_column_int(stmt, 0)
                  << std::setw(20) << (char*)sqlite3_column_text(stmt, 1)
                  << std::setw(15) << (char*)sqlite3_column_text(stmt, 2)
                  << std::setw(10) << sqlite3_column_double(stmt, 3)
                  << std::setw(10) << sqlite3_column_int(stmt, 4)
                  << std::setw(15) << (char*)sqlite3_column_text(stmt, 5)
                  << "\n";
    }
    sqlite3_finalize(stmt);
}

// ─── Find by Barcode ─────────────────────────────────────────────────────────
void Inventory::findByBarcode() {
    std::string barcode;
    std::cout << "\nEnter barcode: "; std::cin >> barcode;

    Product p = getProductByBarcode(barcode);
    if (p.id == -1) {
        std::cout << "✘ Product not found.\n";
        return;
    }
    std::cout << "\nFound: [" << p.id << "] " << p.item
              << " (" << p.brand << ") — $" << p.price
              << " | Qty: " << p.quantity << "\n";
}

// ─── Helpers ──────────────────────────────────────────────────────────────────
std::vector<Product> Inventory::getAllProducts() {
    std::vector<Product> list;
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db->getDB(),
        "SELECT id, item, brand, price, quantity, barcode FROM products;",
        -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Product p;
        p.id       = sqlite3_column_int(stmt, 0);
        p.item     = (char*)sqlite3_column_text(stmt, 1);
        p.brand    = (char*)sqlite3_column_text(stmt, 2);
        p.price    = sqlite3_column_double(stmt, 3);
        p.quantity = sqlite3_column_int(stmt, 4);
        p.barcode  = (char*)sqlite3_column_text(stmt, 5);
        list.push_back(p);
    }
    sqlite3_finalize(stmt);
    return list;
}

Product Inventory::getProductById(int id) {
    Product p; p.id = -1;
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id,item,brand,price,quantity,barcode FROM products WHERE id="
                      + std::to_string(id) + ";";
    sqlite3_prepare_v2(db->getDB(), sql.c_str(), -1, &stmt, 0);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        p.id       = sqlite3_column_int(stmt, 0);
        p.item     = (char*)sqlite3_column_text(stmt, 1);
        p.brand    = (char*)sqlite3_column_text(stmt, 2);
        p.price    = sqlite3_column_double(stmt, 3);
        p.quantity = sqlite3_column_int(stmt, 4);
        p.barcode  = (char*)sqlite3_column_text(stmt, 5);
    }
    sqlite3_finalize(stmt);
    return p;
}

Product Inventory::getProductByBarcode(const std::string& barcode) {
    Product p; p.id = -1;
    sqlite3_stmt* stmt;
    std::string sql = "SELECT id,item,brand,price,quantity,barcode FROM products WHERE barcode='"
                      + barcode + "';";
    sqlite3_prepare_v2(db->getDB(), sql.c_str(), -1, &stmt, 0);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        p.id       = sqlite3_column_int(stmt, 0);
        p.item     = (char*)sqlite3_column_text(stmt, 1);
        p.brand    = (char*)sqlite3_column_text(stmt, 2);
        p.price    = sqlite3_column_double(stmt, 3);
        p.quantity = sqlite3_column_int(stmt, 4);
        p.barcode  = (char*)sqlite3_column_text(stmt, 5);
    }
    sqlite3_finalize(stmt);
    return p;
}

bool Inventory::updateStock(int productId, int newQty) {
    std::string sql = "UPDATE products SET quantity=" + std::to_string(newQty)
                    + " WHERE id=" + std::to_string(productId) + ";";
    return db->execute(sql);
}

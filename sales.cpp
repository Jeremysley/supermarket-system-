#include "sales.h"
#include <iostream>
#include <iomanip>
#include <sqlite3.h>

Sales::Sales(const std::string& username)
    : currentUser(username) {
    db = Database::getInstance();
}

// ─── Process Sale ─────────────────────────────────────────────────────────────
void Sales::processSale() {
    double total = 0;
    char   more  = 'y';

    std::cout << "\n========== NEW SALE ==========\n";
    inventory.listAllItems();

    while (more == 'y' || more == 'Y') {
        int id, qty;
        std::cout << "\nEnter Product ID : "; std::cin >> id;
        std::cout << "Quantity         : "; std::cin >> qty;

        Product p = inventory.getProductById(id);

        if (p.id == -1) {
            std::cout << "✘ Product not found.\n";
        } else if (p.quantity < qty) {
            std::cout << "✘ Insufficient stock! Available: " << p.quantity << "\n";
        } else {
            double lineTotal = p.price * qty;
            total += lineTotal;

            // Record sale in DB
            std::string sql =
                "INSERT INTO sales (product_id, product_name, quantity_sold, unit_price, total_amount, sold_by) "
                "VALUES (" + std::to_string(p.id) + ",'" + p.item + "'," +
                std::to_string(qty) + "," + std::to_string(p.price) + "," +
                std::to_string(lineTotal) + ",'" + currentUser + "');";
            db->execute(sql);

            // Deduct stock
            inventory.updateStock(p.id, p.quantity - qty);

            std::cout << "✔ Added: " << p.item << " x" << qty
                      << " = $" << std::fixed << std::setprecision(2) << lineTotal << "\n";
        }

        std::cout << "Add another item? (y/n): "; std::cin >> more;
    }

    std::cout << "\n==============================\n";
    std::cout << "  TOTAL BILL : $" << std::fixed << std::setprecision(2) << total << "\n";
    std::cout << "==============================\n";
}

// ─── View Sales History ───────────────────────────────────────────────────────
void Sales::viewSalesHistory() {
    std::cout << "\n" << std::left
              << std::setw(5)  << "ID"
              << std::setw(20) << "Product"
              << std::setw(8)  << "Qty"
              << std::setw(10) << "Unit $"
              << std::setw(12) << "Total $"
              << std::setw(15) << "Sold By"
              << std::setw(20) << "Date" << "\n";
    std::cout << std::string(90, '-') << "\n";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db->getDB(),
        "SELECT id, product_name, quantity_sold, unit_price, total_amount, sold_by, date "
        "FROM sales ORDER BY date DESC;",
        -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << std::left
                  << std::setw(5)  << sqlite3_column_int(stmt, 0)
                  << std::setw(20) << (char*)sqlite3_column_text(stmt, 1)
                  << std::setw(8)  << sqlite3_column_int(stmt, 2)
                  << std::setw(10) << sqlite3_column_double(stmt, 3)
                  << std::setw(12) << sqlite3_column_double(stmt, 4)
                  << std::setw(15) << (char*)sqlite3_column_text(stmt, 5)
                  << std::setw(20) << (char*)sqlite3_column_text(stmt, 6)
                  << "\n";
    }
    sqlite3_finalize(stmt);
}

// ─── Get Total Sales (for Reports) ───────────────────────────────────────────
double Sales::getTotalSales() {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db->getDB(),
        "SELECT SUM(total_amount) FROM sales;", -1, &stmt, 0);
    double total = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        total = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return total;
}

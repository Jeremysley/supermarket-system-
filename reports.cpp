#include "reports.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <sqlite3.h>

Reports::Reports() {
    db = Database::getInstance();
}

void Reports::writeToFile(const std::string& content, const std::string& filename) {
    std::ofstream file(filename);
    file << content;
    file.close();
    std::cout << "✔ Report saved to " << filename << "\n";
}

// ─── Sales Report ─────────────────────────────────────────────────────────────
void Reports::generateSalesReport() {
    std::ostringstream ss;
    ss << "========================================\n";
    ss << "         SALES REPORT\n";
    ss << "========================================\n\n";
    ss << std::left
       << std::setw(20) << "Product"
       << std::setw(8)  << "Qty"
       << std::setw(12) << "Total ($)"
       << std::setw(15) << "Sold By"
       << std::setw(20) << "Date" << "\n";
    ss << std::string(75, '-') << "\n";

    sqlite3_stmt* stmt;
    double grandTotal = 0;
    sqlite3_prepare_v2(db->getDB(),
        "SELECT product_name, quantity_sold, total_amount, sold_by, date FROM sales ORDER BY date DESC;",
        -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        double amount = sqlite3_column_double(stmt, 2);
        grandTotal += amount;
        ss << std::left
           << std::setw(20) << (char*)sqlite3_column_text(stmt, 0)
           << std::setw(8)  << sqlite3_column_int(stmt, 1)
           << std::setw(12) << std::fixed << std::setprecision(2) << amount
           << std::setw(15) << (char*)sqlite3_column_text(stmt, 3)
           << std::setw(20) << (char*)sqlite3_column_text(stmt, 4)
           << "\n";
    }
    sqlite3_finalize(stmt);

    ss << std::string(75, '-') << "\n";
    ss << "GRAND TOTAL: $" << std::fixed << std::setprecision(2) << grandTotal << "\n";

    std::cout << ss.str();
    writeToFile(ss.str(), "sales_report.txt");
}

// ─── Inventory Report ─────────────────────────────────────────────────────────
void Reports::generateInventoryReport() {
    std::ostringstream ss;
    ss << "========================================\n";
    ss << "       INVENTORY REPORT\n";
    ss << "========================================\n\n";
    ss << std::left
       << std::setw(5)  << "ID"
       << std::setw(20) << "Item"
       << std::setw(15) << "Brand"
       << std::setw(10) << "Price"
       << std::setw(10) << "Qty"
       << std::setw(15) << "Barcode" << "\n";
    ss << std::string(75, '-') << "\n";

    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db->getDB(),
        "SELECT id,item,brand,price,quantity,barcode FROM products ORDER BY item;",
        -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int qty = sqlite3_column_int(stmt, 4);
        ss << std::left
           << std::setw(5)  << sqlite3_column_int(stmt, 0)
           << std::setw(20) << (char*)sqlite3_column_text(stmt, 1)
           << std::setw(15) << (char*)sqlite3_column_text(stmt, 2)
           << std::setw(10) << sqlite3_column_double(stmt, 3)
           << std::setw(10) << qty
           << std::setw(15) << (char*)sqlite3_column_text(stmt, 5);
        if (qty < 5) ss << "  ⚠ LOW STOCK";
        ss << "\n";
    }
    sqlite3_finalize(stmt);

    std::cout << ss.str();
    writeToFile(ss.str(), "inventory_report.txt");
}

// ─── Full Report ──────────────────────────────────────────────────────────────
void Reports::generateFullReport() {
    generateInventoryReport();
    generateSalesReport();
    std::cout << "✔ Full report generated.\n";
}

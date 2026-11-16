#include "user.h"

void Admin::showMenu() {
    std::cout << "\n========== ADMIN MENU ==========\n";
    std::cout << "  1. Manage Inventory\n";
    std::cout << "  2. Manage Users\n";
    std::cout << "  3. View Sales\n";
    std::cout << "  4. Generate Reports\n";
    std::cout << "  5. Logout\n";
    std::cout << "================================\n";
}

void SalesPerson::showMenu() {
    std::cout << "\n======= SALES PERSON MENU =======\n";
    std::cout << "  1. Process a Sale\n";
    std::cout << "  2. View Sales History\n";
    std::cout << "  3. View Products\n";
    std::cout << "  4. Logout\n";
    std::cout << "=================================\n";
}

void InventoryPerson::showMenu() {
    std::cout << "\n===== INVENTORY PERSON MENU =====\n";
    std::cout << "  1. Add Item\n";
    std::cout << "  2. Remove Item\n";
    std::cout << "  3. Edit Quantity\n";
    std::cout << "  4. List All Items\n";
    std::cout << "  5. Find by Barcode\n";
    std::cout << "  6. Logout\n";
    std::cout << "=================================\n";
}

void Cashier::showMenu() {
    std::cout << "\n========= CASHIER MENU ==========\n";
    std::cout << "  1. Process Checkout\n";
    std::cout << "  2. View Products\n";
    std::cout << "  3. Logout\n";
    std::cout << "==================================\n";
}

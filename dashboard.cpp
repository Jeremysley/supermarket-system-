#include "dashboard.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sqlite3.h>

Dashboard::Dashboard() {
    db = Database::getInstance();
}

// ─── Login ────────────────────────────────────────────────────────────────────
std::shared_ptr<User> Dashboard::login() {
    std::string username, password;
    std::cout << "\n╔══════════════════════════════════╗\n";
    std::cout << "║    SUPERMARKET MANAGEMENT SYS   ║\n";
    std::cout << "╚══════════════════════════════════╝\n";
    std::cout << "Username : "; std::cin >> username;
    std::cout << "Password : "; std::cin >> password;

    sqlite3_stmt* stmt;
    std::string sql = "SELECT id, name, username, role FROM users "
                      "WHERE username='" + username + "' AND password='" + password + "';";
    sqlite3_prepare_v2(db->getDB(), sql.c_str(), -1, &stmt, 0);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int         id   = sqlite3_column_int(stmt, 0);
        std::string name = (char*)sqlite3_column_text(stmt, 1);
        std::string user = (char*)sqlite3_column_text(stmt, 2);
        std::string role = (char*)sqlite3_column_text(stmt, 3);
        sqlite3_finalize(stmt);

        std::cout << "\n✔ Welcome, " << name << "! (" << role << ")\n";

        if (role == "Admin")           return std::make_shared<Admin>(id, name, user);
        if (role == "SalesPerson")     return std::make_shared<SalesPerson>(id, name, user);
        if (role == "InventoryPerson") return std::make_shared<InventoryPerson>(id, name, user);
        if (role == "Cashier")         return std::make_shared<Cashier>(id, name, user);
    }
    sqlite3_finalize(stmt);
    std::cout << "✘ Invalid credentials.\n";
    return nullptr;
}

// ─── Main Run Loop ────────────────────────────────────────────────────────────
void Dashboard::run() {
    while (true) {
        currentUser = login();
        if (!currentUser) continue;

        std::string role = currentUser->getRole();
        if      (role == "Admin")           runAdmin();
        else if (role == "InventoryPerson") runInventoryPerson();
        else if (role == "SalesPerson")     runSalesPerson();
        else if (role == "Cashier")         runCashier();

        char again;
        std::cout << "\nReturn to login? (y/n): "; std::cin >> again;
        if (again != 'y' && again != 'Y') break;
    }
    std::cout << "\nGoodbye!\n";
}

// ─── Admin Flow ───────────────────────────────────────────────────────────────
void Dashboard::runAdmin() {
    Inventory inv;
    Sales     sales(currentUser->getUsername());
    Reports   reports;
    int choice;

    do {
        currentUser->showMenu();
        std::cout << "Choice: "; std::cin >> choice;
        switch (choice) {
            case 1: {
                int sub;
                std::cout << "\n1.Add  2.Remove  3.Edit Qty  4.List  5.Find Barcode\nChoice: ";
                std::cin >> sub;
                if      (sub == 1) inv.addItem();
                else if (sub == 2) inv.removeItem();
                else if (sub == 3) inv.editQuantity();
                else if (sub == 4) inv.listAllItems();
                else if (sub == 5) inv.findByBarcode();
                break;
            }
            case 2: manageUsers(); break;
            case 3: sales.viewSalesHistory(); break;
            case 4: {
                int sub;
                std::cout << "\n1.Sales Report  2.Inventory Report  3.Full Report\nChoice: ";
                std::cin >> sub;
                if      (sub == 1) reports.generateSalesReport();
                else if (sub == 2) reports.generateInventoryReport();
                else if (sub == 3) reports.generateFullReport();
                break;
            }
            case 5: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

// ─── Inventory Person Flow ────────────────────────────────────────────────────
void Dashboard::runInventoryPerson() {
    Inventory inv;
    int choice;
    do {
        currentUser->showMenu();
        std::cout << "Choice: "; std::cin >> choice;
        switch (choice) {
            case 1: inv.addItem();       break;
            case 2: inv.removeItem();    break;
            case 3: inv.editQuantity();  break;
            case 4: inv.listAllItems();  break;
            case 5: inv.findByBarcode(); break;
            case 6: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

// ─── Sales Person Flow ────────────────────────────────────────────────────────
void Dashboard::runSalesPerson() {
    Sales     sales(currentUser->getUsername());
    Inventory inv;
    int choice;
    do {
        currentUser->showMenu();
        std::cout << "Choice: "; std::cin >> choice;
        switch (choice) {
            case 1: sales.processSale();       break;
            case 2: sales.viewSalesHistory();  break;
            case 3: inv.listAllItems();        break;
            case 4: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 4);
}

// ─── Cashier Flow ─────────────────────────────────────────────────────────────
void Dashboard::runCashier() {
    Sales     sales(currentUser->getUsername());
    Inventory inv;
    int choice;
    do {
        currentUser->showMenu();
        std::cout << "Choice: "; std::cin >> choice;
        switch (choice) {
            case 1: sales.processSale();  break;
            case 2: inv.listAllItems();   break;
            case 3: std::cout << "Logging out...\n"; break;
            default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 3);
}

// ─── Manage Users (Admin only) ────────────────────────────────────────────────
void Dashboard::manageUsers() {
    std::cout << "\n--- Manage Users ---\n";
    std::cout << "1. Add User\n2. List Users\n3. Back\nChoice: ";
    int choice; std::cin >> choice;

    if (choice == 1) {
        std::string name, username, password, role;
        std::cout << "Name     : "; std::cin.ignore(); std::getline(std::cin, name);
        std::cout << "Username : "; std::cin >> username;
        std::cout << "Password : "; std::cin >> password;
        std::cout << "Role (Admin/SalesPerson/InventoryPerson/Cashier): ";
        std::cin >> role;

        std::string sql = "INSERT INTO users (name, username, password, role) VALUES ('"
                        + name + "','" + username + "','" + password + "','" + role + "');";
        if (db->execute(sql))
            std::cout << "✔ User added.\n";
        else
            std::cout << "✘ Failed (username may already exist).\n";

    } else if (choice == 2) {
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db->getDB(),
            "SELECT id, name, username, role FROM users;", -1, &stmt, 0);
        std::cout << "\n" << std::left
                  << std::setw(5)  << "ID"
                  << std::setw(20) << "Name"
                  << std::setw(15) << "Username"
                  << std::setw(15) << "Role" << "\n";
        std::cout << std::string(55, '-') << "\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << std::left
                      << std::setw(5)  << sqlite3_column_int(stmt, 0)
                      << std::setw(20) << (char*)sqlite3_column_text(stmt, 1)
                      << std::setw(15) << (char*)sqlite3_column_text(stmt, 2)
                      << std::setw(15) << (char*)sqlite3_column_text(stmt, 3) << "\n";
        }
        sqlite3_finalize(stmt);
    }
}

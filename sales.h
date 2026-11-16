#pragma once
#include "database.h"
#include "inventory.h"
#include <string>

class Sales {
private:
    Database*   db;
    Inventory   inventory;
    std::string currentUser;   // username of who is logged in

public:
    Sales(const std::string& username);

    void processSale();        // full checkout loop
    void viewSalesHistory();   // print past transactions
    double getTotalSales();    // used by Reports
};

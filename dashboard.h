#pragma once
#include "user.h"
#include "inventory.h"
#include "sales.h"
#include "reports.h"
#include "database.h"
#include <memory>
#include <string>

class Dashboard {
private:
    Database*              db;
    std::shared_ptr<User>  currentUser;

    // Login returns a User* based on role from DB
    std::shared_ptr<User> login();

    // Role-specific menu handlers
    void runAdmin();
    void runInventoryPerson();
    void runSalesPerson();
    void runCashier();

    // Admin sub-function: manage users
    void manageUsers();

public:
    Dashboard();
    void run();   // main entry point — called from main()
};

#include "database.h"
#include "dashboard.h"

int main() {
    // 1. Initialize database & create tables
    Database* db = Database::getInstance();
    db->initTables();

    // 2. Hand control to Dashboard (handles login + all role menus)
    Dashboard dashboard;
    dashboard.run();

    return 0;
}

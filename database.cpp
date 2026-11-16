#include "database.h"

Database* Database::instance = nullptr;

Database::Database(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
}

Database* Database::getInstance() {
    if (!instance)
        instance = new Database("supermarket.db");
    return instance;
}

sqlite3* Database::getDB() { return db; }

bool Database::execute(const std::string& sql) {
    char* errMsg;
    if (sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

void Database::initTables() {
    // Products table
    execute(R"(
        CREATE TABLE IF NOT EXISTS products (
            id        INTEGER PRIMARY KEY AUTOINCREMENT,
            item      TEXT    NOT NULL,
            brand     TEXT    NOT NULL,
            price     REAL    NOT NULL,
            quantity  INTEGER NOT NULL DEFAULT 0,
            barcode   TEXT    UNIQUE
        );
    )");

    // Users table
    execute(R"(
        CREATE TABLE IF NOT EXISTS users (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            name     TEXT NOT NULL,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            role     TEXT NOT NULL
        );
    )");

    // Sales table
    execute(R"(
        CREATE TABLE IF NOT EXISTS sales (
            id            INTEGER PRIMARY KEY AUTOINCREMENT,
            product_id    INTEGER,
            product_name  TEXT,
            quantity_sold INTEGER,
            unit_price    REAL,
            total_amount  REAL,
            sold_by       TEXT,
            date          TEXT DEFAULT (datetime('now','localtime')),
            FOREIGN KEY(product_id) REFERENCES products(id)
        );
    )");

    // Default admin account (only inserted if no users exist)
    execute(R"(
        INSERT OR IGNORE INTO users (name, username, password, role)
        VALUES ('Administrator', 'admin', 'admin123', 'Admin');
    )");
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

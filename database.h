#pragma once
#include <sqlite3.h>
#include <string>
#include <iostream>

class Database {
private:
    sqlite3* db;
    static Database* instance;
    Database(const std::string& dbName);
public:
    static Database* getInstance();
    sqlite3* getDB();
    bool execute(const std::string& sql);
    void initTables();
    ~Database();
};

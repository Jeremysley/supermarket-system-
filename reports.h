#pragma once
#include "database.h"
#include <string>

class Reports {
private:
    Database* db;
    void writeToFile(const std::string& content, const std::string& filename);

public:
    Reports();
    void generateSalesReport();
    void generateInventoryReport();
    void generateFullReport();
};

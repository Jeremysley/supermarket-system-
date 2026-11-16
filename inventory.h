#pragma once
#include "product.h"
#include "database.h"
#include <vector>
#include <string>

class Inventory {
private:
    Database* db;

public:
    Inventory();

    // Menu-driven operations (prompt user for input)
    void addItem();
    void removeItem();
    void editQuantity();
    void listAllItems();
    void findByBarcode();

    // Programmatic helpers used by Sales/Dashboard
    std::vector<Product> getAllProducts();
    Product              getProductById(int id);
    Product              getProductByBarcode(const std::string& barcode);
    bool                 updateStock(int productId, int newQty);
};

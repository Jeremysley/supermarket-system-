#pragma once
#include <string>

// Lightweight struct that maps directly to the products table
struct Product {
    int         id;
    std::string item;
    std::string brand;
    double      price;
    int         quantity;
    std::string barcode;
};

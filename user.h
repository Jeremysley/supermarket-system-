#pragma once
#include <string>
#include <iostream>

// ─── Base User ────────────────────────────────────────────────────────────────
class User {
protected:
    int         id;
    std::string name;
    std::string username;
    std::string role;

public:
    User(int id, const std::string& name,
         const std::string& username, const std::string& role)
        : id(id), name(name), username(username), role(role) {}

    virtual void showMenu() = 0;   // each role defines its own menu

    std::string getRole()     const { return role; }
    std::string getName()     const { return name; }
    std::string getUsername() const { return username; }
    int         getId()       const { return id; }

    virtual ~User() {}
};

// ─── Derived Roles ────────────────────────────────────────────────────────────
class Admin : public User {
public:
    Admin(int id, const std::string& name, const std::string& username)
        : User(id, name, username, "Admin") {}
    void showMenu() override;
};

class SalesPerson : public User {
public:
    SalesPerson(int id, const std::string& name, const std::string& username)
        : User(id, name, username, "SalesPerson") {}
    void showMenu() override;
};

class InventoryPerson : public User {
public:
    InventoryPerson(int id, const std::string& name, const std::string& username)
        : User(id, name, username, "InventoryPerson") {}
    void showMenu() override;
};

class Cashier : public User {
public:
    Cashier(int id, const std::string& name, const std::string& username)
        : User(id, name, username, "Cashier") {}
    void showMenu() override;
};

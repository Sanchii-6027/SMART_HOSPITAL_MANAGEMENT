#ifndef LOGIN_H
#define LOGIN_H

#include <iostream>
#include <string>
#include "account.h"

using namespace std;

// Node for storing accounts using a linked list
struct AccountNode
{
    Account data;
    AccountNode* next;

    AccountNode(const Account& account);
};

class Login
{
private:
    AccountNode* head;
    bool accountsLoaded;

    // Internal linked-list operations
    void appendAccount(const Account& account);
    void clearAccounts();

    // File handling
    bool loadAccounts();
    bool saveAccounts() const;

public:
    // Constructor and destructor
    Login();
    ~Login();

    // Initial admin creation (only when no accounts exist)
    bool createInitialAdmin();

    // Create a new staff account
    bool createAccount(const Account& newAccount);

    // Authenticate staff according to selected role
    bool authenticate(
        const string& selectedRole,
        Account& loggedInAccount
    ) const;

    // Check whether username already exists
    bool usernameExists(const string& username) const;

    // Display all staff accounts
    void displayAccounts() const;

    // Reload accounts from file
    bool refreshAccounts();
};

#endif
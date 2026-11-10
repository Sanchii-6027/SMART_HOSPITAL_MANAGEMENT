#include "account.h"

// Default constructor
Account::Account()
{
    id = "";
    name = "";
    username = "";
    password = "";
    role = "";
    department = "";
}

// Parameterized constructor
Account::Account(string accountID,
                 string accountName,
                 string accountUsername,
                 string accountPassword,
                 string accountRole,
                 string accountDepartment)
{
    id = accountID;
    name = accountName;
    username = accountUsername;
    password = accountPassword;
    role = accountRole;
    department = accountDepartment;
}

// Display account details
void Account::displayAccount() const
{
    cout << "\nAccount ID: " << id;
    cout << "\nName: " << name;
    cout << "\nUsername: " << username;
    cout << "\nRole: " << role;
    cout << "\nDepartment: " << department << '\n';
}
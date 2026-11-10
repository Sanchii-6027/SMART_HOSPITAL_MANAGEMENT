#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string>

using namespace std;

struct Account
{
    string id;
    string name;
    string username;
    string password;
    string role;
    string department;

    Account();

    Account(
        string accountID,
        string accountName,
        string accountUsername,
        string accountPassword,
        string accountRole,
        string accountDepartment
    );

    void displayAccount() const;
};

#endif
#include "login.h"
#include <fstream>
#include <sstream>

// AccountNode constructor
AccountNode::AccountNode(const Account& account)
{
    data = account;
    next = NULL;
}

// Login constructor
Login::Login()
{
    head = NULL;
    accountsLoaded = false;

    loadAccounts();
}

// Login destructor
Login::~Login()
{
    clearAccounts();
}

// Clear linked list
void Login::clearAccounts()
{
    AccountNode* current = head;

    while (current != NULL)
    {
        AccountNode* temp = current;
        current = current->next;
        delete temp;
    }

    head = NULL;
}

// Add account to linked list
void Login::appendAccount(const Account& account)
{
    AccountNode* newNode = new AccountNode(account);

    if (head == NULL)
    {
        head = newNode;
        return;
    }

    AccountNode* current = head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = newNode;
}

// Load accounts from file
bool Login::loadAccounts()
{
    clearAccounts();

    ifstream file("DATA/accounts.txt");

    // File may not exist on the first run
    if (!file.is_open())
    {
        accountsLoaded = true;
        return true;
    }

    string line;

    while (getline(file, line))
    {
        string id, name, username;
        string password, role, department;

        stringstream ss(line);

        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, username, '|') &&
            getline(ss, password, '|') &&
            getline(ss, role, '|') &&
            getline(ss, department))
        {
            if (!id.empty() && !username.empty() && !role.empty())
            {
                Account account(
                    id, name, username,
                    password, role, department
                );

                appendAccount(account);
            }
        }
    }

    file.close();
    accountsLoaded = true;

    return true;
}

// Save all accounts to file
bool Login::saveAccounts() const
{
    ofstream file("DATA/accounts.txt", ios::out | ios::trunc);

    if (!file.is_open())
    {
        cout << "\nError: Unable to open accounts.txt\n";
        return false;
    }

    AccountNode* current = head;

    while (current != NULL)
    {
        const Account& account = current->data;

        file << account.id << "|"
             << account.name << "|"
             << account.username << "|"
             << account.password << "|"
             << account.role << "|"
             << account.department << "\n";

        current = current->next;
    }

    file.close();

    return !file.fail();
}

// Check whether username already exists
bool Login::usernameExists(const string& username) const
{
    AccountNode* current = head;

    while (current != NULL)
    {
        if (current->data.username == username)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

// Create the first Admin account
bool Login::createInitialAdmin()
{
    if (head != NULL)
    {
        cout << "\nAn account already exists. "
             << "Initial Admin cannot be created again.\n";

        return false;
    }

    string name, username, password;

    cout << "\n========== INITIAL ADMIN SETUP ==========\n";

    cout << "Enter Admin Name: ";
    getline(cin, name);

    cout << "Create Admin Username: ";
    getline(cin, username);

    cout << "Create Admin Password: ";
    getline(cin, password);

    if (name.empty() || username.empty() || password.empty())
    {
        cout << "\nAll fields are required.\n";
        return false;
    }

    Account admin(
        "ADMIN001",
        name,
        username,
        password,
        "Admin",
        "Administration"
    );

    appendAccount(admin);

    if (!saveAccounts())
    {
        clearAccounts();
        cout << "\nAdmin account could not be saved.\n";
        return false;
    }

    cout << "\nInitial Admin account created successfully!\n";

    return true;
}

// Create a new staff account
bool Login::createAccount(const Account& newAccount)
{
    if (newAccount.id.empty() ||
        newAccount.name.empty() ||
        newAccount.username.empty() ||
        newAccount.password.empty() ||
        newAccount.role.empty() ||
        newAccount.department.empty())
    {
        cout << "\nError: All account fields are required.\n";
        return false;
    }

    // Prevent file-format corruption
    string fields[6] = {
        newAccount.id,
        newAccount.name,
        newAccount.username,
        newAccount.password,
        newAccount.role,
        newAccount.department
    };

    for (int i = 0; i < 6; i++)
    {
        if (fields[i].find('|') != string::npos ||
            fields[i].find('\n') != string::npos ||
            fields[i].find('\r') != string::npos)
        {
            cout << "\nError: Account fields cannot contain "
                 << "the | character or line breaks.\n";

            return false;
        }
    }

    if (usernameExists(newAccount.username))
    {
        cout << "\nError: Username already exists.\n";
        return false;
    }

    AccountNode* current = head;

    while (current != NULL)
    {
        if (current->data.id == newAccount.id)
        {
            cout << "\nError: Account ID already exists.\n";
            return false;
        }

        current = current->next;
    }

    appendAccount(newAccount);

    if (!saveAccounts())
    {
        // Remove the newly added account if saving fails
        AccountNode* previous = NULL;
        current = head;

        while (current->next != NULL)
        {
            previous = current;
            current = current->next;
        }

        if (previous == NULL)
        {
            head = NULL;
        }
        else
        {
            previous->next = NULL;
        }

        delete current;

        cout << "\nAccount could not be saved.\n";
        return false;
    }

    cout << "\nStaff account created successfully!\n";

    return true;
}

// Authenticate staff
bool Login::authenticate(
    const string& selectedRole,
    Account& loggedInAccount) const
{
    string username, password;

    cout << "\n========== STAFF LOGIN ==========\n";

    cout << "Selected Role: " << selectedRole << '\n';

    cout << "Enter Username: ";
    getline(cin, username);

    cout << "Enter Password: ";
    getline(cin, password);

    AccountNode* current = head;

    while (current != NULL)
    {
        if (current->data.username == username &&
            current->data.password == password &&
            current->data.role == selectedRole)
        {
            loggedInAccount = current->data;

            cout << "\nLogin successful!\n";
            cout << "Welcome, " << loggedInAccount.name << "!\n";

            return true;
        }

        current = current->next;
    }

    cout << "\nInvalid username, password or selected role.\n";

    return false;
}

// Display all accounts
void Login::displayAccounts() const
{
    if (head == NULL)
    {
        cout << "\nNo staff accounts found.\n";
        return;
    }

    cout << "\n========== STAFF ACCOUNTS ==========\n";

    AccountNode* current = head;

    while (current != NULL)
    {
        current->data.displayAccount();
        cout << "--------------------------------\n";

        current = current->next;
    }
}

// Reload accounts from file
bool Login::refreshAccounts()
{
    return loadAccounts();
}
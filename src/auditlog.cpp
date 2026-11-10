
#include "../include/auditlog.h"

#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

// Constructor of AuditLog
AuditLog::AuditLog()
{
    logID = "";
    userID = "";
    role = "";
    action = "";
    dateTime = "";
    next = NULL;
}

// Constructor of AuditLogManager
AuditLogManager::AuditLogManager()
{
    head = NULL;
    tail = NULL;

    logCount = 0;
    nextLogNumber = 1;

    loadLogs();
}

// Destructor
AuditLogManager::~AuditLogManager()
{
    saveLogs();
    clearLogs();
}

// Insert node at the end
void AuditLogManager::appendLog(AuditLog* log)
{
    if (log == NULL)
        return;

    log->next = NULL;

    if (head == NULL)
    {
        head = log;
        tail = log;
    }
    else
    {
        tail->next = log;
        tail = log;
    }

    logCount++;
}

// Delete all nodes
void AuditLogManager::clearLogs()
{
    AuditLog* current = head;

    while (current != NULL)
    {
        AuditLog* temp = current;
        current = current->next;
        delete temp;
    }

    head = NULL;
    tail = NULL;
    logCount = 0;
}

// Generate unique Log ID
string AuditLogManager::generateLogID()
{
    string id = "LOG00000";
    string number = to_string(nextLogNumber);

    id.replace(3, 5 - number.length(), string(5 - number.length(), '0'));
    id.replace(8 - number.length(), number.length(), number);

    nextLogNumber++;

    return id;
}

// Load logs from file
bool AuditLogManager::loadLogs()
{
    ifstream file("DATA/audit_logs.txt");

    if (!file.is_open())
        return false;

    clearLogs();

    string line;
    int maximumNumber = 0;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        string fields[5];
        stringstream ss(line);

        bool valid = true;

        for (int i = 0; i < 4; i++)
        {
            if (!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }

        if (!valid || !getline(ss, fields[4]))
            continue;

        AuditLog* log = new AuditLog();

        log->logID = fields[0];
        log->userID = fields[1];
        log->role = fields[2];
        log->action = fields[3];
        log->dateTime = fields[4];

        appendLog(log);

        if (log->logID.length() > 3 &&
            log->logID.substr(0, 3) == "LOG")
        {
            int number = atoi(log->logID.substr(3).c_str());

            if (number > maximumNumber)
                maximumNumber = number;
        }
    }

    file.close();

    nextLogNumber = maximumNumber + 1;

    return true;
}

// Save logs to file
bool AuditLogManager::saveLogs() const
{
    ofstream file("DATA/audit_logs.txt");

    if (!file.is_open())
        return false;

    AuditLog* current = head;

    while (current != NULL)
    {
        file << current->logID << "|"
             << current->userID << "|"
             << current->role << "|"
             << current->action << "|"
             << current->dateTime << "\n";

        current = current->next;
    }

    file.close();

    return true;
}

// Add a new activity log
bool AuditLogManager::addLog(
    const string& userID,
    const string& role,
    const string& action
)
{
    if (userID.empty() || role.empty() || action.empty())
    {
        cout << "\nInvalid audit log details.\n";
        return false;
    }

    AuditLog* log = new AuditLog();

    log->logID = generateLogID();
    log->userID = userID;
    log->role = role;
    log->action = action;

    time_t now = time(NULL);
    tm* currentTime = localtime(&now);

    char buffer[30];

    if (currentTime != NULL)
    {
        strftime(
            buffer,
            sizeof(buffer),
            "%Y-%m-%d %H:%M:%S",
            currentTime
        );

        log->dateTime = buffer;
    }
    else
    {
        log->dateTime = "Unknown";
    }

    appendLog(log);

    if (!saveLogs())
    {
        cout << "\nWarning: Audit log could not be saved.\n";
        return false;
    }

    cout << "\nAudit log added successfully.\n";

    return true;
}

// Search log using Log ID
AuditLog* AuditLogManager::searchLog(const string& logID) const
{
    AuditLog* current = head;

    while (current != NULL)
    {
        if (current->logID == logID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Display all logs
void AuditLogManager::displayAllLogs() const
{
    if (head == NULL)
    {
        cout << "\nNo audit logs available.\n";
        return;
    }

    cout << "\n========== ALL AUDIT LOGS ==========\n";

    AuditLog* current = head;

    while (current != NULL)
    {
        cout << "\nLog ID: " << current->logID;
        cout << "\nUser ID: " << current->userID;
        cout << "\nRole: " << current->role;
        cout << "\nAction: " << current->action;
        cout << "\nDate & Time: " << current->dateTime;
        cout << "\n-----------------------------------\n";

        current = current->next;
    }
}

// Display logs of a particular user
void AuditLogManager::displayUserLogs(const string& userID) const
{
    AuditLog* current = head;
    bool found = false;

    while (current != NULL)
    {
        if (current->userID == userID)
        {
            cout << "\nLog ID: " << current->logID;
            cout << "\nRole: " << current->role;
            cout << "\nAction: " << current->action;
            cout << "\nDate & Time: " << current->dateTime;
            cout << "\n-----------------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo logs found for this user.\n";
}

// Display logs according to role
void AuditLogManager::displayRoleLogs(const string& role) const
{
    AuditLog* current = head;
    bool found = false;

    while (current != NULL)
    {
        if (current->role == role)
        {
            cout << "\nLog ID: " << current->logID;
            cout << "\nUser ID: " << current->userID;
            cout << "\nAction: " << current->action;
            cout << "\nDate & Time: " << current->dateTime;
            cout << "\n-----------------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo logs found for this role.\n";
}

// Get total number of logs
int AuditLogManager::getLogCount() const
{
    return logCount;
}

// Check whether list is empty
bool AuditLogManager::isEmpty() const
{
    return head == NULL;
}

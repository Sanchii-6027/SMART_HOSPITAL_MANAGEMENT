
#ifndef AUDIT_LOG_H
#define AUDIT_LOG_H

#include <iostream>
#include <string>

using namespace std;

struct AuditLog
{
    string logID;
    string userID;
    string role;
    string action;
    string dateTime;

    AuditLog* next;

    AuditLog();
};

class AuditLogManager
{
private:
    AuditLog* head;
    AuditLog* tail;

    int logCount;
    int nextLogNumber;

    void appendLog(AuditLog* log);
    void clearLogs();

    string generateLogID();

    bool loadLogs();
    bool saveLogs() const;

public:
    AuditLogManager();

    ~AuditLogManager();

    AuditLogManager(const AuditLogManager& other) = delete;
    AuditLogManager& operator=(const AuditLogManager& other) = delete;

    // Add a new activity to the audit log
    bool addLog(
        const string& userID,
        const string& role,
        const string& action
    );

    // Search log using its ID
    AuditLog* searchLog(const string& logID) const;

    // Display all activity logs
    void displayAllLogs() const;

    // Display logs of a particular user
    void displayUserLogs(const string& userID) const;

    // Display logs according to role
    void displayRoleLogs(const string& role) const;

    // Display total number of logs
    int getLogCount() const;

    // Check whether the log list is empty
    bool isEmpty() const;
};

#endif

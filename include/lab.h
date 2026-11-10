
#ifndef LAB_H
#define LAB_H

#include <iostream>
#include <string>

using namespace std;

struct LabRecord
{
    string orderID;
    string patientID;
    string doctorID;

    string testName;
    string orderDate;

    string sampleStatus;

    string result;
    string referenceRange;

    string abnormalFlag;

    string reportDate;
    string labStaffID;

    LabRecord* next;

    LabRecord();
};

class LabManager
{
private:
    LabRecord* head;
    LabRecord* tail;

    int recordCount;
    int nextOrderNumber;

    void appendRecord(LabRecord* record);
    void clearRecords();

    string generateOrderID();

    bool loadLabRecords();
    bool saveLabRecords() const;

public:
    LabManager();
    ~LabManager();

    LabManager(const LabManager& other) = delete;
    LabManager& operator=(const LabManager& other) = delete;

    bool orderTest(const LabRecord& record);

    LabRecord* searchByOrderID(const string& orderID) const;

    void displayLabRecord(const string& orderID) const;

    void displayAllRecords() const;

    void displayPendingSamples() const;

    void displayPatientReports(const string& patientID) const;

    bool updateSampleStatus(
        const string& orderID,
        const string& newStatus
    );

    bool enterTestResult(
        const string& orderID,
        const string& result,
        const string& referenceRange,
        const string& reportDate,
        const string& labStaffID,
        const string& abnormalFlag
    );

    int getRecordCount() const;

    bool isEmpty() const;
};

#endif

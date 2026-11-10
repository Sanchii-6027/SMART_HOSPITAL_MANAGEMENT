
#include "../include/lab.h"

#include <fstream>
#include <sstream>

using namespace std;

// ---------------- LAB RECORD CONSTRUCTOR ----------------

LabRecord::LabRecord()
{
    orderID = "";
    patientID = "";
    doctorID = "";
    testName = "";
    orderDate = "";
    sampleStatus = "Pending";
    result = "";
    referenceRange = "";
    abnormalFlag = "Not Available";
    reportDate = "";
    labStaffID = "";
    next = NULL;
}

// ---------------- APPEND RECORD ----------------

void LabManager::appendRecord(LabRecord* record)
{
    record->next = NULL;

    if (head == NULL)
    {
        head = record;
        tail = record;
    }
    else
    {
        tail->next = record;
        tail = record;
    }

    recordCount++;
}

// ---------------- CLEAR RECORDS ----------------

void LabManager::clearRecords()
{
    LabRecord* current = head;

    while (current != NULL)
    {
        LabRecord* temp = current;
        current = current->next;
        delete temp;
    }

    head = NULL;
    tail = NULL;
    recordCount = 0;
}

// ---------------- GENERATE ORDER ID ----------------

string LabManager::generateOrderID()
{
    string number = to_string(nextOrderNumber);

    while (number.length() < 5)
    {
        number = "0" + number;
    }

    nextOrderNumber++;

    return "LAB" + number;
}

// ---------------- CONSTRUCTOR ----------------

LabManager::LabManager()
{
    head = NULL;
    tail = NULL;

    recordCount = 0;
    nextOrderNumber = 1;

    loadLabRecords();
}

// ---------------- DESTRUCTOR ----------------

LabManager::~LabManager()
{
    saveLabRecords();
    clearRecords();
}

// ---------------- ORDER TEST ----------------

bool LabManager::orderTest(const LabRecord& record)
{
    if (record.patientID == "" ||
        record.doctorID == "" ||
        record.testName == "")
    {
        cout << "Patient ID, Doctor ID and Test Name are required.\n";
        return false;
    }

    LabRecord* newRecord = new LabRecord;

    newRecord->orderID = generateOrderID();
    newRecord->patientID = record.patientID;
    newRecord->doctorID = record.doctorID;
    newRecord->testName = record.testName;
    newRecord->orderDate = record.orderDate;

    newRecord->sampleStatus = "Pending";
    newRecord->result = "";
    newRecord->referenceRange = "";
    newRecord->abnormalFlag = "Not Available";
    newRecord->reportDate = "";
    newRecord->labStaffID = "";

    appendRecord(newRecord);

    cout << "\nLab test ordered successfully.\n";
    cout << "Order ID: " << newRecord->orderID << endl;

    saveLabRecords();

    return true;
}

// ---------------- SEARCH BY ORDER ID ----------------

LabRecord* LabManager::searchByOrderID(const string& orderID) const
{
    LabRecord* current = head;

    while (current != NULL)
    {
        if (current->orderID == orderID)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

// ---------------- DISPLAY ONE LAB RECORD ----------------

void LabManager::displayLabRecord(const string& orderID) const
{
    LabRecord* record = searchByOrderID(orderID);

    if (record == NULL)
    {
        cout << "\nLab record not found.\n";
        return;
    }

    cout << "\n---------- LAB RECORD ----------\n";

    cout << "Order ID: " << record->orderID << endl;
    cout << "Patient ID: " << record->patientID << endl;
    cout << "Doctor ID: " << record->doctorID << endl;
    cout << "Test Name: " << record->testName << endl;
    cout << "Order Date: " << record->orderDate << endl;
    cout << "Sample Status: " << record->sampleStatus << endl;
    cout << "Result: " << record->result << endl;
    cout << "Reference Range: " << record->referenceRange << endl;
    cout << "Abnormal Flag: " << record->abnormalFlag << endl;
    cout << "Report Date: " << record->reportDate << endl;
    cout << "Lab Staff ID: " << record->labStaffID << endl;
}

// ---------------- DISPLAY ALL RECORDS ----------------

void LabManager::displayAllRecords() const
{
    if (isEmpty())
    {
        cout << "\nNo laboratory records available.\n";
        return;
    }

    LabRecord* current = head;

    cout << "\n---------- ALL LAB RECORDS ----------\n";

    while (current != NULL)
    {
        cout << "\nOrder ID: " << current->orderID;
        cout << "\nPatient ID: " << current->patientID;
        cout << "\nTest: " << current->testName;
        cout << "\nSample Status: " << current->sampleStatus;
        cout << "\nResult: " << current->result;
        cout << "\n-----------------------------\n";

        current = current->next;
    }
}

// ---------------- DISPLAY PENDING SAMPLES ----------------

void LabManager::displayPendingSamples() const
{
    LabRecord* current = head;
    bool found = false;

    cout << "\n---------- PENDING SAMPLES ----------\n";

    while (current != NULL)
    {
        if (current->sampleStatus == "Pending")
        {
            cout << "\nOrder ID: " << current->orderID;
            cout << "\nPatient ID: " << current->patientID;
            cout << "\nTest: " << current->testName << endl;

            found = true;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No pending samples.\n";
    }
}

// ---------------- DISPLAY PATIENT REPORTS ----------------

void LabManager::displayPatientReports(const string& patientID) const
{
    LabRecord* current = head;
    bool found = false;

    cout << "\n---------- PATIENT LAB REPORTS ----------\n";

    while (current != NULL)
    {
        if (current->patientID == patientID &&
            current->result != "")
        {
            displayLabRecord(current->orderID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No lab reports found for this patient.\n";
    }
}

// ---------------- UPDATE SAMPLE STATUS ----------------

bool LabManager::updateSampleStatus(
    const string& orderID,
    const string& newStatus
)
{
    LabRecord* record = searchByOrderID(orderID);

    if (record == NULL)
    {
        cout << "Lab order not found.\n";
        return false;
    }

    if (newStatus != "Pending" &&
        newStatus != "Collected" &&
        newStatus != "Rejected")
    {
        cout << "Invalid sample status.\n";
        return false;
    }

    if (record->sampleStatus == "Completed")
    {
        cout << "This test is already completed.\n";
        return false;
    }

    record->sampleStatus = newStatus;

    saveLabRecords();

    cout << "Sample status updated successfully.\n";

    return true;
}

// ---------------- ENTER TEST RESULT ----------------

bool LabManager::enterTestResult(
    const string& orderID,
    const string& result,
    const string& referenceRange,
    const string& reportDate,
    const string& labStaffID,
    const string& abnormalFlag
)
{
    LabRecord* record = searchByOrderID(orderID);

    if (record == NULL)
    {
        cout << "Lab order not found.\n";
        return false;
    }

    if (record->sampleStatus != "Collected")
    {
        cout << "Sample must be collected before entering results.\n";
        return false;
    }

    if (result == "" ||
        referenceRange == "" ||
        reportDate == "" ||
        labStaffID == "")
    {
        cout << "All result details are required.\n";
        return false;
    }

    if (abnormalFlag != "Normal" &&
        abnormalFlag != "Abnormal")
    {
        cout << "Abnormal flag must be Normal or Abnormal.\n";
        return false;
    }

    record->result = result;
    record->referenceRange = referenceRange;
    record->reportDate = reportDate;
    record->labStaffID = labStaffID;
    record->abnormalFlag = abnormalFlag;
    record->sampleStatus = "Completed";

    saveLabRecords();

    cout << "Laboratory result entered successfully.\n";

    if (abnormalFlag == "Abnormal")
    {
        cout << "WARNING: This result has been flagged as abnormal.\n";
    }

    return true;
}

// ---------------- GET RECORD COUNT ----------------

int LabManager::getRecordCount() const
{
    return recordCount;
}

// ---------------- CHECK EMPTY ----------------

bool LabManager::isEmpty() const
{
    return head == NULL;
}

// ---------------- LOAD LAB RECORDS ----------------

bool LabManager::loadLabRecords()
{
    ifstream file("DATA/lab_records.txt");

    if (!file.is_open())
    {
        return true;
    }

    string line;

    while (getline(file, line))
    {
        if (line == "")
        {
            continue;
        }

        string fields[12];
        stringstream ss(line);

        bool valid = true;

        for (int i = 0; i < 11; i++)
        {
            if (!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }

        if (!valid || !getline(ss, fields[11]))
        {
            continue;
        }

        LabRecord* record = new LabRecord;

        record->orderID = fields[0];
        record->patientID = fields[1];
        record->doctorID = fields[2];
        record->testName = fields[3];
        record->orderDate = fields[4];
        record->sampleStatus = fields[5];
        record->result = fields[6];
        record->referenceRange = fields[7];
        record->abnormalFlag = fields[8];
        record->reportDate = fields[9];
        record->labStaffID = fields[10];

        stringstream numberStream(fields[11]);
        int arrivalNumber = 0;

        if (!(numberStream >> arrivalNumber) ||
            record->orderID == "" ||
            record->patientID == "" ||
            record->testName == "" ||
            searchByOrderID(record->orderID) != NULL)
        {
            delete record;
            continue;
        }

        appendRecord(record);

        string numberPart = record->orderID.substr(3);
        stringstream idStream(numberPart);

        int idNumber = 0;

        if (idStream >> idNumber)
        {
            if (idNumber >= nextOrderNumber)
            {
                nextOrderNumber = idNumber + 1;
            }
        }
    }

    file.close();

    return true;
}

// ---------------- SAVE LAB RECORDS ----------------

bool LabManager::saveLabRecords() const
{
    ofstream file("DATA/lab_records.txt");

    if (!file.is_open())
    {
        cout << "Unable to save laboratory records.\n";
        return false;
    }

    LabRecord* current = head;

    while (current != NULL)
    {
        file << current->orderID << "|"
             << current->patientID << "|"
             << current->doctorID << "|"
             << current->testName << "|"
             << current->orderDate << "|"
             << current->sampleStatus << "|"
             << current->result << "|"
             << current->referenceRange << "|"
             << current->abnormalFlag << "|"
             << current->reportDate << "|"
             << current->labStaffID << "|"
             << 0 << "\n";

        current = current->next;
    }

    file.close();

    return true;
}

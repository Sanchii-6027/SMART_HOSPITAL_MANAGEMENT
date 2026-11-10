
#include "../include/emergency.h"

#include <fstream>
#include <sstream>

using namespace std;

// ---------------- EMERGENCY CASE ----------------

EmergencyCase::EmergencyCase()
{
    emergencyID = "";
    patientID = "";
    reason = "";
    triageDoctorID = "";
    assessmentDateTime = "";
    severity = "";
    specialistDepartment = "";
    specialistDoctorID = "";
    bedStatus = "Not Assigned";
    status = "Waiting";
    arrivalOrder = 0;
}

// ---------------- CONSTRUCTOR ----------------

EmergencyManager::EmergencyManager()
{
    capacity = 10;
    emergencyCount = 0;
    nextEmergencyNumber = 1;
    nextArrivalOrder = 1;

    heap = new EmergencyCase[capacity];

    loadEmergencies();
}

// ---------------- DESTRUCTOR ----------------

EmergencyManager::~EmergencyManager()
{
    saveEmergencies();
    delete[] heap;
}

// ---------------- RESIZE HEAP ----------------

void EmergencyManager::resizeHeap()
{
    capacity *= 2;

    EmergencyCase* newHeap = new EmergencyCase[capacity];

    for (int i = 0; i < emergencyCount; i++)
    {
        newHeap[i] = heap[i];
    }

    delete[] heap;
    heap = newHeap;
}

// ---------------- SWAP CASES ----------------

void EmergencyManager::swapCases(int first, int second)
{
    EmergencyCase temp = heap[first];
    heap[first] = heap[second];
    heap[second] = temp;
}

// ---------------- PRIORITY ----------------

int EmergencyManager::getPriority(const string& severity) const
{
    if (severity == "Critical")
        return 4;

    if (severity == "Severe")
        return 3;

    if (severity == "Moderate")
        return 2;

    if (severity == "Low")
        return 1;

    return 0;
}

// ---------------- PRIORITY COMPARISON ----------------

bool EmergencyManager::hasHigherPriority(
    const EmergencyCase& first,
    const EmergencyCase& second
) const
{
    int firstPriority = getPriority(first.severity);
    int secondPriority = getPriority(second.severity);

    if (firstPriority > secondPriority)
        return true;

    if (firstPriority == secondPriority &&
        first.arrivalOrder < second.arrivalOrder)
        return true;

    return false;
}

// ---------------- HEAPIFY UP ----------------

void EmergencyManager::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (hasHigherPriority(heap[index], heap[parent]))
        {
            swapCases(index, parent);
            index = parent;
        }
        else
        {
            break;
        }
    }
}

// ---------------- HEAPIFY DOWN ----------------

void EmergencyManager::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int highest = index;

        if (left < emergencyCount &&
            hasHigherPriority(heap[left], heap[highest]))
        {
            highest = left;
        }

        if (right < emergencyCount &&
            hasHigherPriority(heap[right], heap[highest]))
        {
            highest = right;
        }

        if (highest != index)
        {
            swapCases(index, highest);
            index = highest;
        }
        else
        {
            break;
        }
    }
}

// ---------------- GENERATE EMERGENCY ID ----------------

string EmergencyManager::generateEmergencyID()
{
    string id = "EMG";

    string number = to_string(nextEmergencyNumber);

    while (number.length() < 5)
    {
        number = "0" + number;
    }

    nextEmergencyNumber++;

    return id + number;
}

// ---------------- ADD EMERGENCY ----------------

bool EmergencyManager::addEmergency(const EmergencyCase& emergency)
{
    if (emergency.patientID == "" ||
        emergency.reason == "")
    {
        cout << "Patient ID and emergency reason are required.\n";
        return false;
    }

    if (getPriority(emergency.severity) == 0)
    {
        cout << "Invalid severity level.\n";
        return false;
    }

    if (emergencyCount == capacity)
    {
        resizeHeap();
    }

    EmergencyCase newCase = emergency;

    if (newCase.emergencyID == "")
    {
        newCase.emergencyID = generateEmergencyID();
    }

    else
    {
        if (searchEmergency(newCase.emergencyID) != NULL)
        {
            cout << "Emergency ID already exists.\n";
            return false;
        }

        string numberPart = newCase.emergencyID.substr(3);

        stringstream ss(numberPart);
        int number = 0;

        if (ss >> number)
        {
            if (number >= nextEmergencyNumber)
            {
                nextEmergencyNumber = number + 1;
            }
        }
    }

    newCase.arrivalOrder = nextArrivalOrder++;

    if (newCase.status == "")
    {
        newCase.status = "Waiting";
    }

    if (newCase.bedStatus == "")
    {
        newCase.bedStatus = "Not Assigned";
    }

    heap[emergencyCount] = newCase;

    heapifyUp(emergencyCount);

    emergencyCount++;

    saveEmergencies();

    cout << "\nEmergency registered successfully.\n";
    cout << "Emergency ID: " << newCase.emergencyID << endl;

    return true;
}

// ---------------- GET HIGHEST PRIORITY ----------------

EmergencyCase* EmergencyManager::getHighestPriorityEmergency()
{
    if (isEmpty())
    {
        return NULL;
    }

    return &heap[0];
}

// ---------------- SEARCH EMERGENCY ----------------

EmergencyCase* EmergencyManager::searchEmergency(
    const string& emergencyID
) const
{
    for (int i = 0; i < emergencyCount; i++)
    {
        if (heap[i].emergencyID == emergencyID)
        {
            return &heap[i];
        }
    }

    return NULL;
}

// ---------------- REMOVE EMERGENCY ----------------

bool EmergencyManager::removeEmergency(const string& emergencyID)
{
    int index = -1;

    for (int i = 0; i < emergencyCount; i++)
    {
        if (heap[i].emergencyID == emergencyID)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        cout << "Emergency record not found.\n";
        return false;
    }

    emergencyCount--;

    if (index != emergencyCount)
    {
        heap[index] = heap[emergencyCount];

        if (index > 0 &&
            hasHigherPriority(heap[index], heap[(index - 1) / 2]))
        {
            heapifyUp(index);
        }
        else
        {
            heapifyDown(index);
        }
    }

    saveEmergencies();

    cout << "Emergency record removed successfully.\n";

    return true;
}

// ---------------- DISPLAY ONE EMERGENCY ----------------

void EmergencyManager::displayEmergency(const string& emergencyID) const
{
    EmergencyCase* emergency = searchEmergency(emergencyID);

    if (emergency == NULL)
    {
        cout << "Emergency record not found.\n";
        return;
    }

    cout << "\n---------- EMERGENCY DETAILS ----------\n";

    cout << "Emergency ID: " << emergency->emergencyID << endl;
    cout << "Patient ID: " << emergency->patientID << endl;
    cout << "Reason: " << emergency->reason << endl;
    cout << "Triage Doctor ID: " << emergency->triageDoctorID << endl;
    cout << "Assessment Date/Time: " << emergency->assessmentDateTime << endl;
    cout << "Severity: " << emergency->severity << endl;
    cout << "Specialist Department: " << emergency->specialistDepartment << endl;
    cout << "Specialist Doctor ID: " << emergency->specialistDoctorID << endl;
    cout << "Bed Status: " << emergency->bedStatus << endl;
    cout << "Status: " << emergency->status << endl;
}

// ---------------- DISPLAY ALL EMERGENCIES ----------------

void EmergencyManager::displayAllEmergencies() const
{
    if (isEmpty())
    {
        cout << "\nNo emergency records available.\n";
        return;
    }

    cout << "\n---------- ALL EMERGENCY RECORDS ----------\n";

    for (int i = 0; i < emergencyCount; i++)
    {
        cout << "\nEmergency ID: " << heap[i].emergencyID;
        cout << "\nPatient ID: " << heap[i].patientID;
        cout << "\nSeverity: " << heap[i].severity;
        cout << "\nStatus: " << heap[i].status << endl;
    }
}

// ---------------- DISPLAY PRIORITY QUEUE ----------------

void EmergencyManager::displayEmergencyQueue() const
{
    if (isEmpty())
    {
        cout << "\nEmergency queue is empty.\n";
        return;
    }

    cout << "\n---------- EMERGENCY PRIORITY QUEUE ----------\n";

    // Display a sorted copy so the actual heap remains unchanged.

    EmergencyCase* temp = new EmergencyCase[emergencyCount];

    for (int i = 0; i < emergencyCount; i++)
    {
        temp[i] = heap[i];
    }

    for (int i = 0; i < emergencyCount; i++)
    {
        int highest = i;

        for (int j = i + 1; j < emergencyCount; j++)
        {
            if (hasHigherPriority(temp[j], temp[highest]))
            {
                highest = j;
            }
        }

        EmergencyCase swapTemp = temp[i];
        temp[i] = temp[highest];
        temp[highest] = swapTemp;

        cout << "\nEmergency ID: " << temp[i].emergencyID;
        cout << "\nPatient ID: " << temp[i].patientID;
        cout << "\nSeverity: " << temp[i].severity;
        cout << "\n-----------------------------\n";
    }

    delete[] temp;
}

// ---------------- UPDATE EMERGENCY STATUS ----------------

bool EmergencyManager::updateEmergencyStatus(
    const string& emergencyID,
    const string& newStatus
)
{
    EmergencyCase* emergency = searchEmergency(emergencyID);

    if (emergency == NULL || newStatus == "")
    {
        cout << "Invalid emergency record or status.\n";
        return false;
    }

    emergency->status = newStatus;

    saveEmergencies();

    cout << "Emergency status updated successfully.\n";

    return true;
}

// ---------------- GET COUNT ----------------

int EmergencyManager::getEmergencyCount() const
{
    return emergencyCount;
}

// ---------------- CHECK EMPTY ----------------

bool EmergencyManager::isEmpty() const
{
    return emergencyCount == 0;
}

// ---------------- LOAD EMERGENCIES ----------------

bool EmergencyManager::loadEmergencies()
{
    ifstream file("DATA/emergencies.txt");

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

        string fields[11];
        stringstream ss(line);

        bool valid = true;

        for (int i = 0; i < 10; i++)
        {
            if (!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }

        if (!valid || !getline(ss, fields[10]))
        {
            continue;
        }

        EmergencyCase emergency;

        emergency.emergencyID = fields[0];
        emergency.patientID = fields[1];
        emergency.reason = fields[2];
        emergency.triageDoctorID = fields[3];
        emergency.assessmentDateTime = fields[4];
        emergency.severity = fields[5];
        emergency.specialistDepartment = fields[6];
        emergency.specialistDoctorID = fields[7];
        emergency.bedStatus = fields[8];
        emergency.status = fields[9];

        stringstream numberStream(fields[10]);

        if (!(numberStream >> emergency.arrivalOrder))
        {
            continue;
        }

        if (emergency.emergencyID == "" ||
            emergency.patientID == "" ||
            getPriority(emergency.severity) == 0 ||
            searchEmergency(emergency.emergencyID) != NULL)
        {
            continue;
        }

        if (emergencyCount == capacity)
        {
            resizeHeap();
        }

        heap[emergencyCount] = emergency;

        heapifyUp(emergencyCount);

        emergencyCount++;

        if (emergency.arrivalOrder >= nextArrivalOrder)
        {
            nextArrivalOrder = emergency.arrivalOrder + 1;
        }

        string numberPart = emergency.emergencyID.substr(3);

        stringstream idStream(numberPart);

        int idNumber = 0;

        if (idStream >> idNumber)
        {
            if (idNumber >= nextEmergencyNumber)
            {
                nextEmergencyNumber = idNumber + 1;
            }
        }
    }

    file.close();

    return true;
}

// ---------------- SAVE EMERGENCIES ----------------

bool EmergencyManager::saveEmergencies() const
{
    ofstream file("DATA/emergencies.txt");

    if (!file.is_open())
    {
        cout << "Unable to save emergency records.\n";
        return false;
    }

    for (int i = 0; i < emergencyCount; i++)
    {
        file << heap[i].emergencyID << "|"
             << heap[i].patientID << "|"
             << heap[i].reason << "|"
             << heap[i].triageDoctorID << "|"
             << heap[i].assessmentDateTime << "|"
             << heap[i].severity << "|"
             << heap[i].specialistDepartment << "|"
             << heap[i].specialistDoctorID << "|"
             << heap[i].bedStatus << "|"
             << heap[i].status << "|"
             << heap[i].arrivalOrder << "\n";
    }

    file.close();

    return true;
}

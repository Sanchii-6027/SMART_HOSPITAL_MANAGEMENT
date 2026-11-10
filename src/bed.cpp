
#include "../include/bed.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// Split a line using |
int splitBedLine(const string& line, string fields[], int maxFields)
{
    stringstream ss(line);
    string field;
    int count = 0;

    while (getline(ss, field, '|') && count < maxFields)
    {
        fields[count++] = field;
    }

    return count;
}

// Bed constructor
Bed::Bed()
{
    next = NULL;
}

// BedManager constructor
BedManager::BedManager()
{
    head = NULL;
    tail = NULL;

    bedCount = 0;
    nextBedNumber = 1;

    loadBeds();
}

// Destructor
BedManager::~BedManager()
{
    saveBeds();
    clearBeds();
}

// Add bed at the end
void BedManager::appendBed(Bed* bed)
{
    bed->next = NULL;

    if (head == NULL)
    {
        head = tail = bed;
    }
    else
    {
        tail->next = bed;
        tail = bed;
    }

    bedCount++;
}

// Delete all beds
void BedManager::clearBeds()
{
    Bed* current = head;

    while (current != NULL)
    {
        Bed* temp = current;
        current = current->next;
        delete temp;
    }

    head = NULL;
    tail = NULL;
    bedCount = 0;
}

// Generate unique bed ID
string BedManager::generateBedID()
{
    string id = "BED00000" + to_string(nextBedNumber);
    nextBedNumber++;

    return id;
}

// Save beds to file
bool BedManager::saveBeds() const
{
    ofstream file("DATA/beds.txt");

    if (!file)
        return false;

    Bed* current = head;

    while (current != NULL)
    {
        file << current->bedID << "|"
             << current->roomID << "|"
             << current->department << "|"
             << current->category << "|"
             << current->status << "|"
             << current->patientID << "\n";

        current = current->next;
    }

    file.close();
    return true;
}

// Load beds from file
bool BedManager::loadBeds()
{
    ifstream file("DATA/beds.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[6];

        if (splitBedLine(line, fields, 6) != 6)
            continue;

        Bed* bed = new Bed();

        bed->bedID = fields[0];
        bed->roomID = fields[1];
        bed->department = fields[2];
        bed->category = fields[3];
        bed->status = fields[4];
        bed->patientID = fields[5];

        if (bed->bedID.length() > 3 &&
            bed->bedID.substr(0, 3) == "BED")
        {
            int number = atoi(bed->bedID.substr(3).c_str());

            if (number >= nextBedNumber)
                nextBedNumber = number + 1;
        }

        appendBed(bed);
    }

    file.close();
    return true;
}

// Add a new bed
bool BedManager::addBed(const Bed& bed)
{
    if (bed.roomID.empty() ||
        bed.department.empty() ||
        bed.category.empty())
    {
        return false;
    }

    if (bed.category != "General" &&
        bed.category != "ICU" &&
        bed.category != "Emergency" &&
        bed.category != "OT")
    {
        return false;
    }

    Bed* newBed = new Bed();

    *newBed = bed;

    newBed->bedID = generateBedID();
    newBed->patientID = "";

    if (newBed->status.empty())
        newBed->status = "Available";

    if (newBed->status != "Available")
    {
        delete newBed;
        return false;
    }

    appendBed(newBed);

    return saveBeds();
}

// Search bed by ID
Bed* BedManager::searchBed(const string& bedID) const
{
    Bed* current = head;

    while (current != NULL)
    {
        if (current->bedID == bedID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Display one bed
void BedManager::displayBed(const string& bedID) const
{
    Bed* bed = searchBed(bedID);

    if (bed == NULL)
    {
        cout << "\nBed not found.\n";
        return;
    }

    cout << "\nBed ID: " << bed->bedID;
    cout << "\nRoom ID: " << bed->roomID;
    cout << "\nDepartment: " << bed->department;
    cout << "\nCategory: " << bed->category;
    cout << "\nStatus: " << bed->status;
    cout << "\nPatient ID: ";

    if (bed->patientID.empty())
        cout << "Not assigned\n";
    else
        cout << bed->patientID << "\n";
}

// Display all beds
void BedManager::displayAllBeds() const
{
    if (head == NULL)
    {
        cout << "\nNo beds registered.\n";
        return;
    }

    Bed* current = head;

    while (current != NULL)
    {
        displayBed(current->bedID);
        cout << "-------------------------\n";

        current = current->next;
    }
}

// Display available beds
void BedManager::displayAvailableBeds() const
{
    Bed* current = head;
    bool found = false;

    while (current != NULL)
    {
        if (current->status == "Available")
        {
            displayBed(current->bedID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo available beds.\n";
}

// Display beds by category
void BedManager::displayBedsByCategory(const string& category) const
{
    Bed* current = head;
    bool found = false;

    while (current != NULL)
    {
        if (current->category == category)
        {
            displayBed(current->bedID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo beds found in this category.\n";
}

// Display beds by department
void BedManager::displayBedsByDepartment(const string& department) const
{
    Bed* current = head;
    bool found = false;

    while (current != NULL)
    {
        if (current->department == department)
        {
            displayBed(current->bedID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo beds found in this department.\n";
}

// Assign bed to patient
bool BedManager::assignBed(
    const string& bedID,
    const string& patientID)
{
    Bed* bed = searchBed(bedID);

    if (bed == NULL ||
        patientID.empty() ||
        bed->status != "Available")
    {
        return false;
    }

    bed->patientID = patientID;
    bed->status = "Occupied";

    return saveBeds();
}

// Release a bed
bool BedManager::releaseBed(const string& bedID)
{
    Bed* bed = searchBed(bedID);

    if (bed == NULL || bed->status != "Occupied")
        return false;

    bed->patientID = "";
    bed->status = "Available";

    return saveBeds();
}

// Get total bed count
int BedManager::getBedCount() const
{
    return bedCount;
}

// Get available bed count
int BedManager::getAvailableBedCount() const
{
    int count = 0;
    Bed* current = head;

    while (current != NULL)
    {
        if (current->status == "Available")
            count++;

        current = current->next;
    }

    return count;
}

// Check bed availability
bool BedManager::isBedAvailable(const string& bedID) const
{
    Bed* bed = searchBed(bedID);

    return (bed != NULL && bed->status == "Available");
}

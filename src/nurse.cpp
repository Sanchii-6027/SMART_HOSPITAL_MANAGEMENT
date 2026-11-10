#include "../include/nurse.h"

#include <fstream>
#include <sstream>

using namespace std;

const char NURSE_SEPARATOR = '|';

// Check invalid characters
bool hasInvalidNurseCharacter(const string& value)
{
    return value.find(NURSE_SEPARATOR) != string::npos ||
           value.find('\n') != string::npos ||
           value.find('\r') != string::npos;
}

// Split file line into fields
int splitNurseFields(const string& line, string fields[], int maxFields)
{
    int count = 0;
    string current = "";

    for (int i = 0; i < (int)line.length(); i++)
    {
        if (line[i] == NURSE_SEPARATOR)
        {
            if (count >= maxFields)
                return -1;

            fields[count++] = current;
            current = "";
        }
        else
        {
            current += line[i];
        }
    }

    if (count >= maxFields)
        return -1;

    fields[count++] = current;

    return count;
}

// Read integer safely
bool readNurseNumber(const string& value, int& number)
{
    stringstream ss(value);
    ss >> number;

    return !ss.fail() && ss.eof();
}

// --------------------------------------------------
// Nurse constructor
// --------------------------------------------------

Nurse::Nurse()
{
    nurseID = "";
    name = "";
    username = "";
    department = "";
    assignedDoctorID = "";
    assignedDoctorName = "";
    phone = "";
    availability = "Available";

    next = NULL;
}

// --------------------------------------------------
// NurseManager constructor
// --------------------------------------------------

NurseManager::NurseManager()
{
    head = NULL;
    tail = NULL;
    nurseCount = 0;
    nextNurseNumber = 1;

    loadNurses();
}

// --------------------------------------------------
// NurseManager destructor
// --------------------------------------------------

NurseManager::~NurseManager()
{
    saveNurses();
    clearNurses();
}

// --------------------------------------------------
// Append nurse to linked list
// --------------------------------------------------

void NurseManager::appendNurse(Nurse* nurse)
{
    if (nurse == NULL)
        return;

    nurse->next = NULL;

    if (head == NULL)
    {
        head = tail = nurse;
    }
    else
    {
        tail->next = nurse;
        tail = nurse;
    }

    nurseCount++;
}

// --------------------------------------------------
// Clear linked list
// --------------------------------------------------

void NurseManager::clearNurses()
{
    Nurse* current = head;

    while (current != NULL)
    {
        Nurse* nextNurse = current->next;

        delete current;
        current = nextNurse;
    }

    head = NULL;
    tail = NULL;
    nurseCount = 0;
}

// --------------------------------------------------
// Generate unique nurse ID
// --------------------------------------------------

string NurseManager::generateNurseID()
{
    string number = to_string(nextNurseNumber++);

    while (number.length() < 5)
        number = "0" + number;

    return "NUR" + number;
}

// --------------------------------------------------
// Register a new nurse
// --------------------------------------------------

bool NurseManager::registerNurse(const Nurse& nurse)
{
    if (nurse.name.empty() ||
        nurse.username.empty() ||
        nurse.department.empty() ||
        nurse.assignedDoctorID.empty() ||
        nurse.assignedDoctorName.empty() ||
        nurse.phone.empty())
    {
        cout << "\nPlease fill all nurse details.\n";
        return false;
    }

    const string fields[] = {
        nurse.name,
        nurse.username,
        nurse.department,
        nurse.assignedDoctorID,
        nurse.assignedDoctorName,
        nurse.phone,
        nurse.availability
    };

    for (int i = 0; i < 7; i++)
    {
        if (hasInvalidNurseCharacter(fields[i]))
        {
            cout << "\nInvalid character in nurse details.\n";
            return false;
        }
    }

    Nurse* current = head;

    while (current != NULL)
    {
        if (current->username == nurse.username)
        {
            cout << "\nUsername already exists.\n";
            return false;
        }

        if (current->phone == nurse.phone)
        {
            cout << "\nPhone number already registered.\n";
            return false;
        }

        current = current->next;
    }

    Nurse* newNurse = new Nurse();

    newNurse->nurseID = generateNurseID();
    newNurse->name = nurse.name;
    newNurse->username = nurse.username;
    newNurse->department = nurse.department;
    newNurse->assignedDoctorID = nurse.assignedDoctorID;
    newNurse->assignedDoctorName = nurse.assignedDoctorName;
    newNurse->phone = nurse.phone;
    newNurse->availability = nurse.availability;

    appendNurse(newNurse);

    if (!saveNurses())
    {
        cout << "\nWarning: Nurse data could not be saved.\n";
        return false;
    }

    cout << "\nNurse registered successfully!\n";
    cout << "Nurse ID: " << newNurse->nurseID << '\n';

    return true;
}

// --------------------------------------------------
// Search nurse by ID
// --------------------------------------------------

Nurse* NurseManager::searchByID(const string& nurseID) const
{
    Nurse* current = head;

    while (current != NULL)
    {
        if (current->nurseID == nurseID)
            return current;

        current = current->next;
    }

    return NULL;
}

// --------------------------------------------------
// Search nurse by name
// --------------------------------------------------

Nurse* NurseManager::searchByName(const string& name) const
{
    Nurse* current = head;

    while (current != NULL)
    {
        if (current->name == name)
            return current;

        current = current->next;
    }

    return NULL;
}

// --------------------------------------------------
// Display one nurse
// --------------------------------------------------

void NurseManager::displayNurse(const string& nurseID) const
{
    Nurse* nurse = searchByID(nurseID);

    if (nurse == NULL)
    {
        cout << "\nNurse not found.\n";
        return;
    }

    cout << "\n========== NURSE DETAILS ==========\n";

    cout << "Nurse ID: " << nurse->nurseID << '\n';
    cout << "Name: " << nurse->name << '\n';
    cout << "Username: " << nurse->username << '\n';
    cout << "Department: " << nurse->department << '\n';
    cout << "Assigned Doctor ID: "
         << nurse->assignedDoctorID << '\n';
    cout << "Assigned Doctor: "
         << nurse->assignedDoctorName << '\n';
    cout << "Phone: " << nurse->phone << '\n';
    cout << "Availability: " << nurse->availability << '\n';
}

// --------------------------------------------------
// Display all nurses
// --------------------------------------------------

void NurseManager::displayAllNurses() const
{
    if (head == NULL)
    {
        cout << "\nNo nurses registered.\n";
        return;
    }

    Nurse* current = head;

    cout << "\n========== ALL NURSES ==========\n";

    while (current != NULL)
    {
        cout << "\nNurse ID: " << current->nurseID;
        cout << "\nName: " << current->name;
        cout << "\nDepartment: " << current->department;
        cout << "\nAssigned Doctor: "
             << current->assignedDoctorName;
        cout << "\nPhone: " << current->phone;
        cout << "\nAvailability: " << current->availability;
        cout << "\n----------------------------------\n";

        current = current->next;
    }
}

// --------------------------------------------------
// Display nurses by department
// --------------------------------------------------

void NurseManager::displayNursesByDepartment(
    const string& department) const
{
    Nurse* current = head;
    bool found = false;

    cout << "\nNurses in " << department << ":\n";

    while (current != NULL)
    {
        if (current->department == department)
        {
            cout << "\nNurse ID: " << current->nurseID;
            cout << "\nName: " << current->name;
            cout << "\nAssigned Doctor: "
                 << current->assignedDoctorName;
            cout << "\nAvailability: " << current->availability;
            cout << "\n-----------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo nurses found in this department.\n";
}

// --------------------------------------------------
// Display nurses assigned to a doctor
// --------------------------------------------------

void NurseManager::displayNursesByDoctor(
    const string& doctorID) const
{
    Nurse* current = head;
    bool found = false;

    cout << "\nNurses assigned to doctor "
         << doctorID << ":\n";

    while (current != NULL)
    {
        if (current->assignedDoctorID == doctorID)
        {
            cout << "\nNurse ID: " << current->nurseID;
            cout << "\nName: " << current->name;
            cout << "\nDepartment: " << current->department;
            cout << "\nPhone: " << current->phone;
            cout << "\n-----------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo nurses assigned to this doctor.\n";
}

// --------------------------------------------------
// Get nurse count
// --------------------------------------------------

int NurseManager::getNurseCount() const
{
    return nurseCount;
}

// --------------------------------------------------
// Save nurses to file
// --------------------------------------------------

bool NurseManager::saveNurses() const
{
    ofstream file("DATA/nurses.txt");

    if (!file.is_open())
        return false;

    Nurse* current = head;

    while (current != NULL)
    {
        file << current->nurseID << NURSE_SEPARATOR
             << current->name << NURSE_SEPARATOR
             << current->username << NURSE_SEPARATOR
             << current->department << NURSE_SEPARATOR
             << current->assignedDoctorID << NURSE_SEPARATOR
             << current->assignedDoctorName << NURSE_SEPARATOR
             << current->phone << NURSE_SEPARATOR
             << current->availability << '\n';

        current = current->next;
    }

    return file.good();
}

// --------------------------------------------------
// Load nurses from file
// --------------------------------------------------

bool NurseManager::loadNurses()
{
    ifstream file("DATA/nurses.txt");

    if (!file.is_open())
        return true;

    string line;
    int maximumID = 0;

    while (getline(file, line))
    {
        string fields[8];

        if (splitNurseFields(line, fields, 8) != 8)
            continue;

        if (fields[0].empty() ||
            fields[1].empty() ||
            fields[2].empty() ||
            fields[3].empty() ||
            fields[4].empty() ||
            fields[5].empty() ||
            fields[6].empty())
        {
            continue;
        }

        bool duplicate = false;
        Nurse* current = head;

        while (current != NULL)
        {
            if (current->nurseID == fields[0] ||
                current->username == fields[2] ||
                current->phone == fields[6])
            {
                duplicate = true;
                break;
            }

            current = current->next;
        }

        if (duplicate)
            continue;

        Nurse* nurse = new Nurse();

        nurse->nurseID = fields[0];
        nurse->name = fields[1];
        nurse->username = fields[2];
        nurse->department = fields[3];
        nurse->assignedDoctorID = fields[4];
        nurse->assignedDoctorName = fields[5];
        nurse->phone = fields[6];
        nurse->availability = fields[7];

        appendNurse(nurse);

        if (nurse->nurseID.length() > 3 &&
            nurse->nurseID.substr(0, 3) == "NUR")
        {
            int idNumber;

            if (readNurseNumber(
                    nurse->nurseID.substr(3), idNumber) &&
                idNumber > maximumID)
            {
                maximumID = idNumber;
            }
        }
    }

    nextNurseNumber = maximumID + 1;

    return true;
}
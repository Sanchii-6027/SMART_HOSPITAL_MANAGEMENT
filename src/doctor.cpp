#include "../include/doctor.h"

#include <fstream>
#include <sstream>

using namespace std;

const char DOCTOR_SEPARATOR = '|';

// Check invalid characters
bool hasInvalidCharacter(const string& value)
{
    return value.find(DOCTOR_SEPARATOR) != string::npos ||
           value.find('\n') != string::npos ||
           value.find('\r') != string::npos;
}

// Split file fields
int splitDoctorFields(const string& line, string fields[], int maxFields)
{
    int count = 0;
    string current = "";

    for (int i = 0; i < (int)line.length(); i++)
    {
        if (line[i] == DOCTOR_SEPARATOR)
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
bool readDoctorNumber(const string& value, int& number)
{
    stringstream ss(value);
    ss >> number;

    return !ss.fail() && ss.eof();
}

// Doctor constructor
Doctor::Doctor()
{
    doctorID = "";
    name = "";
    username = "";
    department = "";
    specialization = "";
    phone = "";
    availability = "Available";

    next = NULL;
}

// DoctorManager constructor
DoctorManager::DoctorManager()
{
    head = NULL;
    tail = NULL;
    doctorCount = 0;
    nextDoctorNumber = 1;

    loadDoctors();
}

// Destructor
DoctorManager::~DoctorManager()
{
    saveDoctors();
    clearDoctors();
}

// Append doctor
void DoctorManager::appendDoctor(Doctor* doctor)
{
    if (doctor == NULL)
        return;

    doctor->next = NULL;

    if (head == NULL)
        head = tail = doctor;
    else
    {
        tail->next = doctor;
        tail = doctor;
    }

    doctorCount++;
}

// Clear doctors
void DoctorManager::clearDoctors()
{
    Doctor* current = head;

    while (current != NULL)
    {
        Doctor* nextDoctor = current->next;
        delete current;
        current = nextDoctor;
    }

    head = NULL;
    tail = NULL;
    doctorCount = 0;
}

// Generate Doctor ID
string DoctorManager::generateDoctorID()
{
    string number = to_string(nextDoctorNumber++);

    while (number.length() < 5)
        number = "0" + number;

    return "DOC" + number;
}

// Register doctor
bool DoctorManager::registerDoctor(const Doctor& doctor)
{
    if (doctor.name.empty() ||
        doctor.username.empty() ||
        doctor.department.empty() ||
        doctor.specialization.empty() ||
        doctor.phone.empty())
    {
        cout << "\nPlease fill all doctor details.\n";
        return false;
    }

    const string fields[] = {
        doctor.name,
        doctor.username,
        doctor.department,
        doctor.specialization,
        doctor.phone,
        doctor.availability
    };

    for (int i = 0; i < 6; i++)
    {
        if (hasInvalidCharacter(fields[i]))
        {
            cout << "\nInvalid character in doctor details.\n";
            return false;
        }
    }

    Doctor* current = head;

    while (current != NULL)
    {
        if (current->username == doctor.username)
        {
            cout << "\nUsername already exists.\n";
            return false;
        }

        if (current->phone == doctor.phone)
        {
            cout << "\nPhone number already registered.\n";
            return false;
        }

        current = current->next;
    }

    Doctor* newDoctor = new Doctor();

    newDoctor->doctorID = generateDoctorID();
    newDoctor->name = doctor.name;
    newDoctor->username = doctor.username;
    newDoctor->department = doctor.department;
    newDoctor->specialization = doctor.specialization;
    newDoctor->phone = doctor.phone;
    newDoctor->availability = doctor.availability;

    appendDoctor(newDoctor);

    if (!saveDoctors())
    {
        cout << "\nWarning: Doctor data could not be saved.\n";
        return false;
    }

    cout << "\nDoctor registered successfully!\n";
    cout << "Doctor ID: " << newDoctor->doctorID << '\n';

    return true;
}

// Search doctor by ID
Doctor* DoctorManager::searchByID(const string& doctorID) const
{
    Doctor* current = head;

    while (current != NULL)
    {
        if (current->doctorID == doctorID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Search doctor by name
Doctor* DoctorManager::searchByName(const string& name) const
{
    Doctor* current = head;

    while (current != NULL)
    {
        if (current->name == name)
            return current;

        current = current->next;
    }

    return NULL;
}

// Display one doctor
void DoctorManager::displayDoctor(const string& doctorID) const
{
    Doctor* doctor = searchByID(doctorID);

    if (doctor == NULL)
    {
        cout << "\nDoctor not found.\n";
        return;
    }

    cout << "\n========== DOCTOR DETAILS ==========\n";
    cout << "Doctor ID: " << doctor->doctorID << '\n';
    cout << "Name: " << doctor->name << '\n';
    cout << "Username: " << doctor->username << '\n';
    cout << "Department: " << doctor->department << '\n';
    cout << "Specialization: " << doctor->specialization << '\n';
    cout << "Phone: " << doctor->phone << '\n';
    cout << "Manual Availability: " << doctor->availability << '\n';
}

// Display all doctors
void DoctorManager::displayAllDoctors() const
{
    if (head == NULL)
    {
        cout << "\nNo doctors registered.\n";
        return;
    }

    Doctor* current = head;

    cout << "\n========== ALL DOCTORS ==========\n";

    while (current != NULL)
    {
        cout << "\nDoctor ID: " << current->doctorID;
        cout << "\nName: " << current->name;
        cout << "\nDepartment: " << current->department;
        cout << "\nSpecialization: " << current->specialization;
        cout << "\nPhone: " << current->phone;
        cout << "\nManual Availability: " << current->availability;
        cout << "\n----------------------------------\n";

        current = current->next;
    }
}

// Display doctors by department
void DoctorManager::displayDoctorsByDepartment(
    const string& department) const
{
    Doctor* current = head;
    bool found = false;

    cout << "\nDoctors in " << department << ":\n";

    while (current != NULL)
    {
        if (current->department == department)
        {
            cout << "\nDoctor ID: " << current->doctorID;
            cout << "\nName: " << current->name;
            cout << "\nSpecialization: " << current->specialization;
            cout << "\nAvailability: " << current->availability;
            cout << "\n-----------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo doctors found in this department.\n";
}

// Get doctor count
int DoctorManager::getDoctorCount() const
{
    return doctorCount;
}

// Get effective availability
string DoctorManager::getEffectiveAvailability(
    const string& doctorID,
    int activePatientCount) const
{
    Doctor* doctor = searchByID(doctorID);

    if (doctor == NULL)
        return "Doctor Not Found";

    // Preserve manually assigned statuses
    if (doctor->availability != "Available")
        return doctor->availability;

    // Automatic capacity check
    if (activePatientCount >= MAX_PATIENTS_PER_DOCTOR)
        return "Unavailable";

    return "Available";
}

// Check whether doctor can accept another patient
bool DoctorManager::canAcceptPatient(
    const string& doctorID,
    int activePatientCount) const
{
    Doctor* doctor = searchByID(doctorID);

    if (doctor == NULL)
        return false;

    if (doctor->availability != "Available")
        return false;

    if (activePatientCount >= MAX_PATIENTS_PER_DOCTOR)
        return false;

    return true;
}

// Save doctors
bool DoctorManager::saveDoctors() const
{
    ofstream file("DATA/doctors.txt");

    if (!file.is_open())
        return false;

    Doctor* current = head;

    while (current != NULL)
    {
        file << current->doctorID << DOCTOR_SEPARATOR
             << current->name << DOCTOR_SEPARATOR
             << current->username << DOCTOR_SEPARATOR
             << current->department << DOCTOR_SEPARATOR
             << current->specialization << DOCTOR_SEPARATOR
             << current->phone << DOCTOR_SEPARATOR
             << current->availability << '\n';

        current = current->next;
    }

    return file.good();
}

// Load doctors
bool DoctorManager::loadDoctors()
{
    ifstream file("DATA/doctors.txt");

    if (!file.is_open())
        return true;

    string line;
    int maximumID = 0;

    while (getline(file, line))
    {
        string fields[7];

        if (splitDoctorFields(line, fields, 7) != 7)
            continue;

        if (fields[0].empty() ||
            fields[1].empty() ||
            fields[2].empty() ||
            fields[3].empty() ||
            fields[4].empty() ||
            fields[5].empty())
            continue;

        bool duplicate = false;
        Doctor* current = head;

        while (current != NULL)
        {
            if (current->doctorID == fields[0] ||
                current->username == fields[2] ||
                current->phone == fields[5])
            {
                duplicate = true;
                break;
            }

            current = current->next;
        }

        if (duplicate)
            continue;

        Doctor* doctor = new Doctor();

        doctor->doctorID = fields[0];
        doctor->name = fields[1];
        doctor->username = fields[2];
        doctor->department = fields[3];
        doctor->specialization = fields[4];
        doctor->phone = fields[5];
        doctor->availability = fields[6];

        appendDoctor(doctor);

        if (doctor->doctorID.length() > 3 &&
            doctor->doctorID.substr(0, 3) == "DOC")
        {
            int idNumber;

            if (readDoctorNumber(
                    doctor->doctorID.substr(3), idNumber) &&
                idNumber > maximumID)
            {
                maximumID = idNumber;
            }
        }
    }

    nextDoctorNumber = maximumID + 1;

    return true;
}
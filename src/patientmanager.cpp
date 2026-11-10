#include "../include/patientmanager.h"

#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// --------------------------------------------------
// Helper functions
// --------------------------------------------------

const char DELIMITER = '|';

bool containsInvalidCharacter(const string& value)
{
    return value.find(DELIMITER) != string::npos ||
           value.find('\n') != string::npos ||
           value.find('\r') != string::npos;
}

int splitFields(const string& line, string fields[], int maxFields)
{
    int count = 0;
    string current = "";

    for (int i = 0; i < (int)line.length(); i++)
    {
        if (line[i] == DELIMITER)
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

bool readNumber(const string& value, double& number)
{
    stringstream ss(value);
    ss >> number;

    return !ss.fail() && ss.eof();
}

bool readInteger(const string& value, int& number)
{
    stringstream ss(value);
    ss >> number;

    return !ss.fail() && ss.eof();
}

void writeVital(ofstream& file, const string& patientID,
                const VitalRecord& record)
{
    file << patientID << DELIMITER
         << record.date << DELIMITER
         << record.systolicBP << DELIMITER
         << record.diastolicBP << DELIMITER
         << record.bloodSugar << DELIMITER
         << record.temperature << DELIMITER
         << record.heartRate << DELIMITER
         << record.oxygenLevel << DELIMITER
         << record.height << DELIMITER
         << record.weight << DELIMITER
         << record.BMI << DELIMITER
         << record.abnormalFlags << DELIMITER
         << record.nurseID << '\n';
}

void writeMedical(ofstream& file, const string& patientID,
                  const MedicalRecord& record)
{
    file << patientID << DELIMITER
         << record.date << DELIMITER
         << record.symptoms << DELIMITER
         << record.diagnosis << DELIMITER
         << record.clinicalNotes << DELIMITER
         << record.treatment << DELIMITER
         << record.doctorID << DELIMITER
         << record.doctorName << '\n';
}

// --------------------------------------------------
// PatientNode constructor
// --------------------------------------------------

PatientNode::PatientNode(Patient* patient)
{
    data = patient;
    next = NULL;
    prev = NULL;
}

// --------------------------------------------------
// PatientManager constructor and destructor
// --------------------------------------------------

PatientManager::PatientManager()
{
    head = NULL;
    tail = NULL;
    patientCount = 0;
    nextPatientNumber = 1;

    loadPatients();
    loadVitalHistory();
    loadMedicalHistory();

    rebuildHashTables();
}

PatientManager::~PatientManager()
{
    saveAllData();
    clearPatients();
}

// --------------------------------------------------
// Clear all patient nodes and patient objects
// --------------------------------------------------

void PatientManager::clearPatients()
{
    PatientNode* current = head;

    while (current != NULL)
    {
        PatientNode* nextNode = current->next;

        delete current->data;
        delete current;

        current = nextNode;
    }

    head = NULL;
    tail = NULL;
    patientCount = 0;

    patientIDTable.clear();
    phoneTable.clear();
}

// --------------------------------------------------
// Append patient to doubly linked list
// --------------------------------------------------

void PatientManager::appendPatient(Patient* patient)
{
    if (patient == NULL)
        return;

    PatientNode* newNode = new PatientNode(patient);

    if (head == NULL)
    {
        head = tail = newNode;
    }
    else
    {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }

    patientCount++;
}

// --------------------------------------------------
// Generate unique patient ID
// --------------------------------------------------

string PatientManager::generatePatientID()
{
    string number = to_string(nextPatientNumber++);

    while (number.length() < 5)
        number = "0" + number;

    return "P" + number;
}

// --------------------------------------------------
// Validate patient information
// --------------------------------------------------

bool PatientManager::validatePatient(const Patient& patient) const
{
    if (patient.name.empty() ||
        patient.phone.empty() ||
        patient.age < 0 ||
        patient.age > 120)
    {
        return false;
    }

    const string fields[] = {
        patient.patientID, patient.name, patient.gender,
        patient.phone, patient.address,
        patient.emergencyContactName,
        patient.emergencyContactPhone,
        patient.emergencyContactRelation,
        patient.bloodGroup, patient.allergies,
        patient.existingConditions, patient.currentMedicines,
        patient.initialSymptoms, patient.department,
        patient.doctorID, patient.doctorName,
        patient.status, patient.emergencyID,
        patient.emergencySeverity, patient.admissionDate,
        patient.dischargeDate, patient.roomID, patient.bedID
    };

    int count = sizeof(fields) / sizeof(fields[0]);

    for (int i = 0; i < count; i++)
    {
        if (containsInvalidCharacter(fields[i]))
            return false;
    }

    return true;
}

// --------------------------------------------------
// Rebuild both hash tables
// --------------------------------------------------

void PatientManager::rebuildHashTables()
{
    patientIDTable.clear();
    phoneTable.clear();

    PatientNode* current = head;

    while (current != NULL)
    {
        if (current->data != NULL)
        {
            patientIDTable.insert(
                current->data->patientID, current->data
            );

            phoneTable.insert(
                current->data->phone, current->data
            );
        }

        current = current->next;
    }
}

// --------------------------------------------------
// Register a new patient
// --------------------------------------------------

bool PatientManager::registerPatient(Patient& patient)
{
    if (patient.patientID.empty())
        patient.patientID = generatePatientID();

    if (!validatePatient(patient))
    {
        cout << "\nInvalid patient information.\n";
        return false;
    }

    if (searchByID(patient.patientID) != NULL)
    {
        cout << "\nPatient ID already exists.\n";
        return false;
    }

    if (searchByPhone(patient.phone) != NULL)
    {
        cout << "\nPhone number already registered.\n";
        return false;
    }

    Patient* newPatient = new Patient(patient);

    appendPatient(newPatient);
    rebuildHashTables();

    if (!saveAllData())
    {
        cout << "\nWarning: Could not save all patient data.\n";
        return false;
    }

    cout << "\nPatient registered successfully.\n";
    cout << "Patient ID: " << patient.patientID << '\n';

    return true;
}

// --------------------------------------------------
// Search patient by ID using Hash Table
// --------------------------------------------------

Patient* PatientManager::searchByID(const string& patientID) const
{
    return patientIDTable.search(patientID);
}

// --------------------------------------------------
// Search patient by phone using Hash Table
// --------------------------------------------------

Patient* PatientManager::searchByPhone(const string& phone) const
{
    return phoneTable.search(phone);
}

// --------------------------------------------------
// Search patient by name
// --------------------------------------------------

Patient* PatientManager::searchByName(const string& name) const
{
    PatientNode* current = head;

    while (current != NULL)
    {
        if (current->data->name == name)
            return current->data;

        current = current->next;
    }

    return NULL;
}

// --------------------------------------------------
// Display one patient
// --------------------------------------------------

void PatientManager::displayPatient(const string& patientID) const
{
    Patient* patient = searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    patient->displayBasicInfo();
}

// --------------------------------------------------
// Display all patients
// --------------------------------------------------

void PatientManager::displayAllPatients() const
{
    if (head == NULL)
    {
        cout << "\nNo patients registered.\n";
        return;
    }

    PatientNode* current = head;

    cout << "\n========== ALL PATIENTS ==========\n";

    while (current != NULL)
    {
        current->data->displayBasicInfo();
        cout << "----------------------------------\n";

        current = current->next;
    }
}

// --------------------------------------------------
// Update patient information
// --------------------------------------------------

bool PatientManager::updatePatient(const string& patientID)
{
    Patient* patient = searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return false;
    }

    string newName, newPhone, newAddress, newGender;
    string newEmergencyName, newEmergencyPhone;
    string newEmergencyRelation;
    string input;
    int newAge;

    cout << "\nEnter new name: ";
    getline(cin, newName);

    cout << "Enter new age: ";
    getline(cin, input);

    if (!readInteger(input, newAge) ||
        newAge < 0 || newAge > 120)
    {
        cout << "\nInvalid age.\n";
        return false;
    }

    cout << "Enter new gender: ";
    getline(cin, newGender);

    cout << "Enter new phone: ";
    getline(cin, newPhone);

    cout << "Enter new address: ";
    getline(cin, newAddress);

    cout << "Enter emergency contact name: ";
    getline(cin, newEmergencyName);

    cout << "Enter emergency contact phone: ";
    getline(cin, newEmergencyPhone);

    cout << "Enter emergency contact relation: ";
    getline(cin, newEmergencyRelation);

    Patient* duplicate = searchByPhone(newPhone);

    if (duplicate != NULL && duplicate != patient)
    {
        cout << "\nThis phone number belongs to another patient.\n";
        return false;
    }

    Patient updatedPatient = *patient;

    updatedPatient.name = newName;
    updatedPatient.age = newAge;
    updatedPatient.gender = newGender;
    updatedPatient.phone = newPhone;
    updatedPatient.address = newAddress;
    updatedPatient.emergencyContactName = newEmergencyName;
    updatedPatient.emergencyContactPhone = newEmergencyPhone;
    updatedPatient.emergencyContactRelation = newEmergencyRelation;

    if (!validatePatient(updatedPatient))
    {
        cout << "\nInvalid information. Update cancelled.\n";
        return false;
    }

    *patient = updatedPatient;

    rebuildHashTables();

    if (!saveAllData())
    {
        cout << "\nWarning: Could not save all updated data.\n";
        return false;
    }

    cout << "\nPatient information updated successfully.\n";
    return true;
}

// --------------------------------------------------
// Delete patient
// --------------------------------------------------

bool PatientManager::deletePatient(const string& patientID)
{
    PatientNode* current = head;

    while (current != NULL)
    {
        if (current->data->patientID == patientID)
            break;

        current = current->next;
    }

    if (current == NULL)
    {
        cout << "\nPatient not found.\n";
        return false;
    }

    if (current->prev != NULL)
        current->prev->next = current->next;
    else
        head = current->next;

    if (current->next != NULL)
        current->next->prev = current->prev;
    else
        tail = current->prev;

    delete current->data;
    delete current;

    patientCount--;

    rebuildHashTables();

    if (!saveAllData())
    {
        cout << "\nWarning: Could not save all patient data.\n";
        return false;
    }

    cout << "\nPatient deleted successfully.\n";
    return true;
}

// --------------------------------------------------
// Merge Sort: split doubly linked list
// --------------------------------------------------

PatientNode* PatientManager::splitList(PatientNode* first)
{
    if (first == NULL || first->next == NULL)
        return NULL;

    PatientNode* slow = first;
    PatientNode* fast = first->next;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    PatientNode* second = slow->next;

    slow->next = NULL;

    if (second != NULL)
        second->prev = NULL;

    return second;
}

// --------------------------------------------------
// Merge two sorted lists by age
// --------------------------------------------------

PatientNode* PatientManager::merge(
    PatientNode* left, PatientNode* right)
{
    if (left == NULL)
        return right;

    if (right == NULL)
        return left;

    PatientNode* result = NULL;

    if (left->data->age <= right->data->age)
    {
        result = left;
        result->next = merge(left->next, right);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next);
    }

    if (result->next != NULL)
        result->next->prev = result;

    result->prev = NULL;

    return result;
}

// --------------------------------------------------
// Recursive Merge Sort
// --------------------------------------------------

PatientNode* PatientManager::mergeSort(PatientNode* first)
{
    if (first == NULL || first->next == NULL)
        return first;

    PatientNode* second = splitList(first);

    first = mergeSort(first);
    second = mergeSort(second);

    return merge(first, second);
}

// --------------------------------------------------
// Sort patients by age
// --------------------------------------------------

void PatientManager::sortPatientsByAge()
{
    if (head == NULL || head->next == NULL)
    {
        cout << "\nNot enough patients to sort.\n";
        return;
    }

    head = mergeSort(head);

    PatientNode* current = head;
    tail = NULL;

    while (current != NULL)
    {
        tail = current;
        current = current->next;
    }

    rebuildHashTables();

    cout << "\nPatients sorted by age successfully.\n";
    displayAllPatients();
}

// --------------------------------------------------
// Patient count and empty check
// --------------------------------------------------

int PatientManager::getPatientCount() const
{
    return patientCount;
}

bool PatientManager::isEmpty() const
{
    return head == NULL;
}

// --------------------------------------------------
// Save patient basic information
// --------------------------------------------------

bool PatientManager::savePatients() const
{
    ofstream file("DATA/patients.txt");

    if (!file.is_open())
        return false;

    PatientNode* current = head;

    while (current != NULL)
    {
        Patient* p = current->data;

        file << p->patientID << DELIMITER
             << p->name << DELIMITER
             << p->age << DELIMITER
             << p->gender << DELIMITER
             << p->phone << DELIMITER
             << p->address << DELIMITER
             << p->emergencyContactName << DELIMITER
             << p->emergencyContactPhone << DELIMITER
             << p->emergencyContactRelation << DELIMITER
             << p->bloodGroup << DELIMITER
             << p->allergies << DELIMITER
             << p->existingConditions << DELIMITER
             << p->currentMedicines << DELIMITER
             << p->initialSymptoms << DELIMITER
             << p->department << DELIMITER
             << p->doctorID << DELIMITER
             << p->doctorName << DELIMITER
             << p->status << DELIMITER
             << p->isEmergency << DELIMITER
             << p->emergencyID << DELIMITER
             << p->emergencySeverity << DELIMITER
             << p->isAdmitted << DELIMITER
             << p->admissionDate << DELIMITER
             << p->dischargeDate << DELIMITER
             << p->roomID << DELIMITER
             << p->bedID << '\n';

        current = current->next;
    }

    return file.good();
}

// --------------------------------------------------
// Load patient basic information
// --------------------------------------------------

bool PatientManager::loadPatients()
{
    ifstream file("DATA/patients.txt");

    if (!file.is_open())
        return true;

    string line;
    int maximumID = 0;

    while (getline(file, line))
    {
        string fields[26];

        if (splitFields(line, fields, 26) != 26)
            continue;

        Patient* p = new Patient();

        p->patientID = fields[0];
        p->name = fields[1];

        if (!readInteger(fields[2], p->age))
        {
            delete p;
            continue;
        }

        p->gender = fields[3];
        p->phone = fields[4];
        p->address = fields[5];
        p->emergencyContactName = fields[6];
        p->emergencyContactPhone = fields[7];
        p->emergencyContactRelation = fields[8];
        p->bloodGroup = fields[9];
        p->allergies = fields[10];
        p->existingConditions = fields[11];
        p->currentMedicines = fields[12];
        p->initialSymptoms = fields[13];
        p->department = fields[14];
        p->doctorID = fields[15];
        p->doctorName = fields[16];
        p->status = fields[17];

        int emergencyValue, admittedValue;

        if (!readInteger(fields[18], emergencyValue) ||
            !readInteger(fields[21], admittedValue))
        {
            delete p;
            continue;
        }

        p->isEmergency = (emergencyValue != 0);
        p->emergencyID = fields[19];
        p->emergencySeverity = fields[20];
        p->isAdmitted = (admittedValue != 0);
        p->admissionDate = fields[22];
        p->dischargeDate = fields[23];
        p->roomID = fields[24];
        p->bedID = fields[25];

        if (!validatePatient(*p) ||
            searchByID(p->patientID) != NULL ||
            searchByPhone(p->phone) != NULL)
        {
            delete p;
            continue;
        }

        appendPatient(p);

        if (p->patientID.length() > 1 &&
            p->patientID[0] == 'P')
        {
            int idNumber;

            if (readInteger(p->patientID.substr(1), idNumber) &&
                idNumber > maximumID)
            {
                maximumID = idNumber;
            }
        }
    }

    nextPatientNumber = maximumID + 1;

    rebuildHashTables();

    return true;
}

// --------------------------------------------------
// Save all vital history
// --------------------------------------------------

bool PatientManager::saveVitalHistory() const
{
    ofstream file("DATA/vitals.txt");

    if (!file.is_open())
        return false;

    PatientNode* current = head;

    while (current != NULL)
    {
        const VitalNode* vital = current->data->getVitalHead();

        while (vital != NULL)
        {
            const VitalRecord& r = vital->data;

            if (containsInvalidCharacter(r.date) ||
                containsInvalidCharacter(r.abnormalFlags) ||
                containsInvalidCharacter(r.nurseID))
                return false;

            writeVital(file, current->data->patientID, r);
            vital = vital->next;
        }

        current = current->next;
    }

    return file.good();
}

// --------------------------------------------------
// Load all vital history
// --------------------------------------------------

bool PatientManager::loadVitalHistory()
{
    ifstream file("DATA/vitals.txt");

    if (!file.is_open())
        return true;

    string line;

    while (getline(file, line))
    {
        string f[13];

        if (splitFields(line, f, 13) != 13)
            continue;

        Patient* patient = searchByID(f[0]);

        if (patient == NULL)
            continue;

        VitalRecord r;

        r.date = f[1];

        if (!readNumber(f[2], r.systolicBP) ||
            !readNumber(f[3], r.diastolicBP) ||
            !readNumber(f[4], r.bloodSugar) ||
            !readNumber(f[5], r.temperature) ||
            !readNumber(f[6], r.heartRate) ||
            !readNumber(f[7], r.oxygenLevel) ||
            !readNumber(f[8], r.height) ||
            !readNumber(f[9], r.weight) ||
            !readNumber(f[10], r.BMI))
            continue;

        r.abnormalFlags = f[11];
        r.nurseID = f[12];

        patient->addVitalRecord(r);
    }

    return true;
}

// --------------------------------------------------
// Save all medical history
// --------------------------------------------------

bool PatientManager::saveMedicalHistory() const
{
    ofstream file("DATA/medical_records.txt");

    if (!file.is_open())
        return false;

    PatientNode* current = head;

    while (current != NULL)
    {
        const MedicalNode* medical =
            current->data->getMedicalHead();

        while (medical != NULL)
        {
            const MedicalRecord& r = medical->data;

            if (containsInvalidCharacter(r.date) ||
                containsInvalidCharacter(r.symptoms) ||
                containsInvalidCharacter(r.diagnosis) ||
                containsInvalidCharacter(r.clinicalNotes) ||
                containsInvalidCharacter(r.treatment) ||
                containsInvalidCharacter(r.doctorID) ||
                containsInvalidCharacter(r.doctorName))
                return false;

            writeMedical(file, current->data->patientID, r);
            medical = medical->next;
        }

        current = current->next;
    }

    return file.good();
}

// --------------------------------------------------
// Load all medical history
// --------------------------------------------------

bool PatientManager::loadMedicalHistory()
{
    ifstream file("DATA/medical_records.txt");

    if (!file.is_open())
        return true;

    string line;

    while (getline(file, line))
    {
        string f[8];

        if (splitFields(line, f, 8) != 8)
            continue;

        Patient* patient = searchByID(f[0]);

        if (patient == NULL)
            continue;

        MedicalRecord r;

        r.date = f[1];
        r.symptoms = f[2];
        r.diagnosis = f[3];
        r.clinicalNotes = f[4];
        r.treatment = f[5];
        r.doctorID = f[6];
        r.doctorName = f[7];

        patient->addMedicalRecord(r);
    }

    return true;
}

// --------------------------------------------------
// Save all three patient data files
// --------------------------------------------------

bool PatientManager::saveAllData() const
{
    bool patientsSaved = savePatients();
    bool vitalsSaved = saveVitalHistory();
    bool medicalSaved = saveMedicalHistory();

    return patientsSaved && vitalsSaved && medicalSaved;
}

// --------------------------------------------------
// Reload all patient data
// --------------------------------------------------

bool PatientManager::reloadAllData()
{
    clearPatients();

    nextPatientNumber = 1;

    if (!loadPatients())
        return false;

    if (!loadVitalHistory())
        return false;

    if (!loadMedicalHistory())
        return false;

    rebuildHashTables();

    return true;
}

// --------------------------------------------------
// Display hash table information
// --------------------------------------------------

void PatientManager::displaySearchStructures() const
{
    cout << "\n========== SEARCH STRUCTURES ==========\n";

    cout << "Patient ID Hash Table:\n";
    patientIDTable.display();

    cout << "\nPhone Number Hash Table:\n";
    phoneTable.display();

    cout << "\nTotal patients: " << patientCount << '\n';
}
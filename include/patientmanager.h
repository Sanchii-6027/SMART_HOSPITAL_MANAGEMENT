#ifndef PATIENT_MANAGER_H
#define PATIENT_MANAGER_H

#include "patient.h"
#include "dataStructures.h"
#include <iostream>
#include <string>

using namespace std;

// Doubly Linked List Node for Patients
struct PatientNode
{
    Patient* data;
    PatientNode* next;
    PatientNode* prev;

    PatientNode(Patient* patient);
};

class PatientManager
{
private:

    // Main patient Doubly Linked List
    PatientNode* head;
    PatientNode* tail;

    int patientCount;
    int nextPatientNumber;

    // Hash tables for fast searching
    HashTable patientIDTable;
    HashTable phoneTable;

    // File handling
    bool loadPatients();
    bool savePatients() const;
    bool loadVitalHistory();
    bool saveVitalHistory() const;
    bool loadMedicalHistory();
    bool saveMedicalHistory() const;

    // ID generation
    string generatePatientID();

    // Linked list operations
    void appendPatient(Patient* patient);
    void clearPatients();

    // Merge Sort helpers
    PatientNode* mergeSort(PatientNode* first);
    PatientNode* merge(PatientNode* left, PatientNode* right);
    PatientNode* splitList(PatientNode* first);

    // Rebuild hash tables after sorting
    void rebuildHashTables();

    // Validate patient information
    bool validatePatient(const Patient& patient) const;

public:

    // Constructor and destructor
    PatientManager();
    ~PatientManager();

    PatientManager(const PatientManager& other) = delete;
    PatientManager& operator=(const PatientManager& other) = delete;

    // Patient registration
    bool registerPatient(Patient& patient);

    // Patient searching
    Patient* searchByID(const string& patientID) const;
    Patient* searchByPhone(const string& phone) const;
    Patient* searchByName(const string& name) const;

    // Patient information
    void displayPatient(const string& patientID) const;
    void displayAllPatients() const;

    // Patient updating and deletion
    bool updatePatient(const string& patientID);
    bool deletePatient(const string& patientID);

    // Sorting
    void sortPatientsByAge();

    // Patient statistics
    int getPatientCount() const;
    bool isEmpty() const;

    // Doctor patient capacity management
    int countActivePatientsForDoctor(
        const string& doctorID
    ) const;

    bool isDoctorAtCapacity(
        const string& doctorID
    ) const;

    // File operations
    bool saveAllData() const;
    bool reloadAllData();

    // Display hash table information
    void displaySearchStructures() const;
};

#endif
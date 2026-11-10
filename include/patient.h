#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <string>

using namespace std;

// ================= VITAL RECORD =================

struct VitalRecord
{
    string date;
    double systolicBP;
    double diastolicBP;
    double bloodSugar;
    double temperature;
    double heartRate;
    double oxygenLevel;
    double height;
    double weight;
    double BMI;
    string abnormalFlags;
    string nurseID;

    VitalRecord();
};

struct VitalNode
{
    VitalRecord data;
    VitalNode* next;
    VitalNode* prev;

    VitalNode(const VitalRecord& record);
};

// ================= MEDICAL RECORD =================

struct MedicalRecord
{
    string date;
    string symptoms;
    string diagnosis;
    string clinicalNotes;
    string treatment;
    string doctorID;
    string doctorName;

    MedicalRecord();
};

struct MedicalNode
{
    MedicalRecord data;
    MedicalNode* next;
    MedicalNode* prev;

    MedicalNode(const MedicalRecord& record);
};

// ================= PATIENT CLASS =================

class Patient
{
private:
    VitalNode* vitalHead;
    VitalNode* vitalTail;

    MedicalNode* medicalHead;
    MedicalNode* medicalTail;

    void clearVitalHistory();
    void clearMedicalHistory();

    void copyVitalHistory(const VitalNode* source);
    void copyMedicalHistory(const MedicalNode* source);

public:

    // Basic information
    string patientID;
    string name;
    int age;
    string gender;
    string phone;
    string address;

    // Emergency contact
    string emergencyContactName;
    string emergencyContactPhone;
    string emergencyContactRelation;

    // Medical information
    string bloodGroup;
    string allergies;
    string existingConditions;
    string currentMedicines;
    string initialSymptoms;

    // Doctor assignment
    string department;
    string doctorID;
    string doctorName;

    // Patient status
    string status;
    bool isEmergency;
    string emergencyID;
    string emergencySeverity;

    // Admission information
    bool isAdmitted;
    string admissionDate;
    string dischargeDate;
    string roomID;
    string bedID;

    // Constructors and destructor
    Patient();
    Patient(const Patient& other);
    Patient& operator=(const Patient& other);
    ~Patient();

    // Vital history
    void addVitalRecord(const VitalRecord& record);
    void displayVitalHistory() const;
    int getVitalRecordCount() const;

    // Medical history
    void addMedicalRecord(const MedicalRecord& record);
    void displayMedicalHistory() const;
    int getMedicalRecordCount() const;

    // Read-only access to history lists
    const VitalNode* getVitalHead() const;
    const MedicalNode* getMedicalHead() const;

    // Display patient information
    void displayBasicInfo() const;
};

#endif
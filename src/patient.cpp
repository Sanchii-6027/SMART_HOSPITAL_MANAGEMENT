#include "patient.h"

// ================= VITAL RECORD =================

VitalRecord::VitalRecord()
{
    date = "";
    systolicBP = 0;
    diastolicBP = 0;
    bloodSugar = 0;
    temperature = 0;
    heartRate = 0;
    oxygenLevel = 0;
    height = 0;
    weight = 0;
    BMI = 0;
    abnormalFlags = "";
    nurseID = "";
}

VitalNode::VitalNode(const VitalRecord& record)
{
    data = record;
    next = NULL;
    prev = NULL;
}

// ================= MEDICAL RECORD =================

MedicalRecord::MedicalRecord()
{
    date = "";
    symptoms = "";
    diagnosis = "";
    clinicalNotes = "";
    treatment = "";
    doctorID = "";
    doctorName = "";
}

MedicalNode::MedicalNode(const MedicalRecord& record)
{
    data = record;
    next = NULL;
    prev = NULL;
}

// ================= PATIENT CONSTRUCTOR =================

Patient::Patient()
{
    vitalHead = NULL;
    vitalTail = NULL;
    medicalHead = NULL;
    medicalTail = NULL;

    patientID = "";
    name = "";
    age = 0;
    gender = "";
    phone = "";
    address = "";

    emergencyContactName = "";
    emergencyContactPhone = "";
    emergencyContactRelation = "";

    bloodGroup = "";
    allergies = "";
    existingConditions = "";
    currentMedicines = "";
    initialSymptoms = "";

    department = "";
    doctorID = "";
    doctorName = "";

    status = "Registered";
    isEmergency = false;
    emergencyID = "";
    emergencySeverity = "";

    isAdmitted = false;
    admissionDate = "";
    dischargeDate = "";
    roomID = "";
    bedID = "";
}

// ================= COPY CONSTRUCTOR =================

Patient::Patient(const Patient& other)
{
    vitalHead = NULL;
    vitalTail = NULL;
    medicalHead = NULL;
    medicalTail = NULL;

    *this = other;
}

// ================= ASSIGNMENT OPERATOR =================

Patient& Patient::operator=(const Patient& other)
{
    if (this == &other)
        return *this;

    patientID = other.patientID;
    name = other.name;
    age = other.age;
    gender = other.gender;
    phone = other.phone;
    address = other.address;

    emergencyContactName = other.emergencyContactName;
    emergencyContactPhone = other.emergencyContactPhone;
    emergencyContactRelation = other.emergencyContactRelation;

    bloodGroup = other.bloodGroup;
    allergies = other.allergies;
    existingConditions = other.existingConditions;
    currentMedicines = other.currentMedicines;
    initialSymptoms = other.initialSymptoms;

    department = other.department;
    doctorID = other.doctorID;
    doctorName = other.doctorName;

    status = other.status;
    isEmergency = other.isEmergency;
    emergencyID = other.emergencyID;
    emergencySeverity = other.emergencySeverity;

    isAdmitted = other.isAdmitted;
    admissionDate = other.admissionDate;
    dischargeDate = other.dischargeDate;
    roomID = other.roomID;
    bedID = other.bedID;

    clearVitalHistory();
    clearMedicalHistory();

    copyVitalHistory(other.vitalHead);
    copyMedicalHistory(other.medicalHead);

    return *this;
}

// ================= DESTRUCTOR =================

Patient::~Patient()
{
    clearVitalHistory();
    clearMedicalHistory();
}

// ================= CLEAR VITAL HISTORY =================

void Patient::clearVitalHistory()
{
    while (vitalHead != NULL)
    {
        VitalNode* temp = vitalHead;
        vitalHead = vitalHead->next;
        delete temp;
    }

    vitalTail = NULL;
}

// ================= CLEAR MEDICAL HISTORY =================

void Patient::clearMedicalHistory()
{
    while (medicalHead != NULL)
    {
        MedicalNode* temp = medicalHead;
        medicalHead = medicalHead->next;
        delete temp;
    }

    medicalTail = NULL;
}

// ================= COPY VITAL HISTORY =================

void Patient::copyVitalHistory(const VitalNode* source)
{
    while (source != NULL)
    {
        addVitalRecord(source->data);
        source = source->next;
    }
}

// ================= COPY MEDICAL HISTORY =================

void Patient::copyMedicalHistory(const MedicalNode* source)
{
    while (source != NULL)
    {
        addMedicalRecord(source->data);
        source = source->next;
    }
}

// ================= ADD VITAL RECORD =================

void Patient::addVitalRecord(const VitalRecord& record)
{
    VitalNode* newNode = new VitalNode(record);

    if (vitalHead == NULL)
    {
        vitalHead = newNode;
        vitalTail = newNode;
    }
    else
    {
        newNode->prev = vitalTail;
        vitalTail->next = newNode;
        vitalTail = newNode;
    }
}

// ================= DISPLAY VITAL HISTORY =================

void Patient::displayVitalHistory() const
{
    if (vitalHead == NULL)
    {
        cout << "\nNo vital records available.\n";
        return;
    }

    const VitalNode* current = vitalHead;

    cout << "\n========== VITAL HISTORY ==========\n";

    while (current != NULL)
    {
        const VitalRecord& r = current->data;

        cout << "\nDate: " << r.date;
        cout << "\nBlood Pressure: " << r.systolicBP
             << "/" << r.diastolicBP;
        cout << "\nBlood Sugar: " << r.bloodSugar;
        cout << "\nTemperature: " << r.temperature;
        cout << "\nHeart Rate: " << r.heartRate;
        cout << "\nOxygen Level: " << r.oxygenLevel;
        cout << "\nHeight: " << r.height;
        cout << "\nWeight: " << r.weight;
        cout << "\nBMI: " << r.BMI;
        cout << "\nAbnormal Flags: " << r.abnormalFlags;
        cout << "\nNurse ID: " << r.nurseID;
        cout << "\n-----------------------------------\n";

        current = current->next;
    }
}

// ================= COUNT VITAL RECORDS =================

int Patient::getVitalRecordCount() const
{
    int count = 0;
    const VitalNode* current = vitalHead;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

// ================= ADD MEDICAL RECORD =================

void Patient::addMedicalRecord(const MedicalRecord& record)
{
    MedicalNode* newNode = new MedicalNode(record);

    if (medicalHead == NULL)
    {
        medicalHead = newNode;
        medicalTail = newNode;
    }
    else
    {
        newNode->prev = medicalTail;
        medicalTail->next = newNode;
        medicalTail = newNode;
    }
}

// ================= DISPLAY MEDICAL HISTORY =================

void Patient::displayMedicalHistory() const
{
    if (medicalHead == NULL)
    {
        cout << "\nNo medical records available.\n";
        return;
    }

    const MedicalNode* current = medicalHead;

    cout << "\n========== MEDICAL HISTORY ==========\n";

    while (current != NULL)
    {
        const MedicalRecord& r = current->data;

        cout << "\nDate: " << r.date;
        cout << "\nSymptoms: " << r.symptoms;
        cout << "\nDiagnosis: " << r.diagnosis;
        cout << "\nClinical Notes: " << r.clinicalNotes;
        cout << "\nTreatment: " << r.treatment;
        cout << "\nDoctor ID: " << r.doctorID;
        cout << "\nDoctor Name: " << r.doctorName;
        cout << "\n------------------------------------\n";

        current = current->next;
    }
}

// ================= COUNT MEDICAL RECORDS =================

int Patient::getMedicalRecordCount() const
{
    int count = 0;
    const MedicalNode* current = medicalHead;

    while (current != NULL)
    {
        count++;
        current = current->next;
    }

    return count;
}

// ================= HISTORY ACCESSORS =================

const VitalNode* Patient::getVitalHead() const
{
    return vitalHead;
}

const MedicalNode* Patient::getMedicalHead() const
{
    return medicalHead;
}

// ================= DISPLAY BASIC INFORMATION =================

void Patient::displayBasicInfo() const
{
    cout << "\n========== PATIENT INFORMATION ==========\n";

    cout << "\nPatient ID: " << patientID;
    cout << "\nName: " << name;
    cout << "\nAge: " << age;
    cout << "\nGender: " << gender;
    cout << "\nPhone: " << phone;
    cout << "\nAddress: " << address;

    cout << "\n\n========== EMERGENCY CONTACT ==========\n";

    cout << "\nContact Name: " << emergencyContactName;
    cout << "\nContact Phone: " << emergencyContactPhone;
    cout << "\nRelation: " << emergencyContactRelation;

    cout << "\n\n========== MEDICAL INFORMATION ==========\n";

    cout << "\nBlood Group: " << bloodGroup;
    cout << "\nAllergies: " << allergies;
    cout << "\nExisting Conditions: " << existingConditions;
    cout << "\nCurrent Medicines: " << currentMedicines;
    cout << "\nInitial Symptoms: " << initialSymptoms;

    cout << "\n\n========== DOCTOR ASSIGNMENT ==========\n";

    cout << "\nDepartment: " << department;
    cout << "\nDoctor ID: " << doctorID;
    cout << "\nDoctor Name: " << doctorName;

    cout << "\n\n========== PATIENT STATUS ==========\n";

    cout << "\nStatus: " << status;
    cout << "\nEmergency: " << (isEmergency ? "Yes" : "No");
    cout << "\nEmergency ID: " << emergencyID;
    cout << "\nEmergency Severity: " << emergencySeverity;

    cout << "\n\n========== ADMISSION DETAILS ==========\n";

    cout << "\nAdmitted: " << (isAdmitted ? "Yes" : "No");
    cout << "\nAdmission Date: " << admissionDate;
    cout << "\nDischarge Date: " << dischargeDate;
    cout << "\nRoom ID: " << roomID;
    cout << "\nBed ID: " << bedID;

    cout << "\nVital Records: " << getVitalRecordCount();
    cout << "\nMedical Records: " << getMedicalRecordCount();

    cout << "\n========================================\n";
}
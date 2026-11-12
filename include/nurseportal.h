#ifndef NURSE_PORTAL_H
#define NURSE_PORTAL_H

#include <iostream>
#include <string>

#include "nurse.h"
#include "patientmanager.h"
#include "bed.h"
#include "resource.h"

using namespace std;

class NursePortal
{
private:
    string loggedInNurseID;
    string assignedDoctorID;

    // Check whether a patient belongs to this nurse's doctor
    bool hasPatientAccess(const Patient* patient) const;

public:

    NursePortal();

    // Login and logout
    bool login(
        const string& nurseID,
        NurseManager& nurseManager
    );

    void logout();

    bool isLoggedIn() const;

    // Dashboard
    void displayDashboard() const;

    // Patient management
    void viewAssignedPatients(
        PatientManager& patientManager
    ) const;

    void viewPatientDetails(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Vital signs
    void recordVitals(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Medical instructions
    void viewMedicalInstructions(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Bed management
    void assignPatientBed(
        PatientManager& patientManager,
        BedManager& bedManager,
        const string& patientID
    ) const;

    void releasePatientBed(
        PatientManager& patientManager,
        BedManager& bedManager,
        const string& patientID
    ) const;

    // Discharge admitted and outpatient patients
    void dischargePatient(
        PatientManager& patientManager,
        BedManager& bedManager,
        const string& patientID
    ) const;

    // Equipment management
    void assignEquipment(
        PatientManager& patientManager,
        ResourceManager& resourceManager,
        const string& patientID
    ) const;

    void returnEquipment(
        ResourceManager& resourceManager,
        const string& assignmentID
    ) const;
};

#endif
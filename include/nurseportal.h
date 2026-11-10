
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

    // Check whether a patient belongs to this nurse's assigned doctor
    bool hasPatientAccess(const Patient* patient) const;

public:
    NursePortal();

    // Nurse login and logout
    bool login(
        const string& nurseID,
        NurseManager& nurseManager
    );

    void logout();

    bool isLoggedIn() const;

    // Nurse dashboard
    void displayDashboard() const;

    // View assigned patients
    void viewAssignedPatients(
        PatientManager& patientManager
    ) const;

    // View patient profile and medical history
    void viewPatientDetails(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Record patient vital signs
    void recordVitals(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // View patient medical instructions
    void viewMedicalInstructions(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Assign bed to patient
    void assignPatientBed(
        PatientManager& patientManager,
        BedManager& bedManager,
        const string& patientID
    ) const;

    // Release bed after discharge
    void releasePatientBed(
        PatientManager& patientManager,
        BedManager& bedManager,
        const string& patientID
    ) const;

    // Assign equipment to patient
    void assignEquipment(
        PatientManager& patientManager,
        ResourceManager& resourceManager,
        const string& patientID
    ) const;

    // Return equipment
    void returnEquipment(
        ResourceManager& resourceManager,
        const string& assignmentID
    ) const;
};

#endif

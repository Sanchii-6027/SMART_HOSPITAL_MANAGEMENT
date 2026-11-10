
#ifndef DOCTOR_PORTAL_H
#define DOCTOR_PORTAL_H

#include <iostream>
#include <string>

#include "patientmanager.h"
#include "doctor.h"
#include "lab.h"
#include "pharmacist.h"
#include "appointment.h"
#include "emergency.h"
#include "bed.h"
#include "billing.h"

using namespace std;

class DoctorPortal
{
private:
    string loggedInDoctorID;

public:
    DoctorPortal();

    // Doctor login session
    void login(const string& doctorID);

    void logout();

    bool isLoggedIn() const;

    // Doctor dashboard
    void displayDashboard() const;

    // View assigned patients
    void viewAssignedPatients(
        PatientManager& patientManager
    ) const;

    // View a particular patient's records
    void viewPatientRecords(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Add diagnosis and medical notes
    void addMedicalRecord(
        PatientManager& patientManager,
        const string& patientID
    );

    // View appointments
    void viewAppointments(
        AppointmentManager& appointmentManager
    ) const;

    // Order laboratory tests
    void orderLabTest(
        PatientManager& patientManager,
        LabManager& labManager,
        const string& patientID
    ) const;

    // View laboratory reports
    void viewLabReports(
        LabManager& labManager,
        const string& patientID
    ) const;

    // Create prescription
    void createPrescription(
        PatientManager& patientManager,
        PharmacistManager& pharmacistManager,
        const string& patientID
    ) const;

    // View emergency cases
    void viewEmergencyCases(
        EmergencyManager& emergencyManager
    ) const;

    // Recommend patient admission
    void recommendAdmission(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // Recommend patient discharge
    void recommendDischarge(
        PatientManager& patientManager,
        const string& patientID
    ) const;

    // View patient billing information
    void viewPatientBill(
        BillingManager& billingManager,
        const string& patientID
    ) const;
};

#endif

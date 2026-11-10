
#ifndef PATIENT_PORTAL_H
#define PATIENT_PORTAL_H

#include <iostream>
#include <string>

#include "patientmanager.h"
#include "appointment.h"
#include "lab.h"
#include "pharmacist.h"
#include "billing.h"

using namespace std;

class PatientPortal
{
private:
    Patient* loggedInPatient;

public:
    PatientPortal();

    // Patient login using ID and registered phone
    bool authenticate(
        const string& patientID,
        const string& registeredPhone,
        PatientManager& patientManager
    );

    // Logout
    void logout();

    // Check login status
    bool isLoggedIn() const;

    // Display patient dashboard
    void displayDashboard() const;

    // View patient profile
    void viewProfile() const;

    // View vital history
    void viewVitalHistory() const;

    // View medical history
    void viewMedicalHistory() const;

    // View appointments
    void viewAppointments(
        AppointmentManager& appointmentManager
    ) const;

    // View appointment requests
    void viewAppointmentRequests(
        AppointmentManager& appointmentManager
    ) const;

    // View laboratory reports
    void viewLabReports(
        LabManager& labManager
    ) const;

    // View prescriptions
    void viewPrescriptions(
        PharmacistManager& pharmacistManager
    ) const;

    // View bills
    void viewBills(
        BillingManager& billingManager
    ) const;

    // View outstanding payment
    void viewOutstandingAmount(
        BillingManager& billingManager
    ) const;
};

#endif

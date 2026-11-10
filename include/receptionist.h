
#ifndef RECEPTIONIST_H
#define RECEPTIONIST_H

#include <iostream>
#include <string>

#include "patientmanager.h"
#include "appointment.h"
#include "doctor.h"
#include "emergency.h"
#include "bed.h"

using namespace std;

class Receptionist
{
private:
    // Display receptionist menu sections
    void displayPatientMenu() const;
    void displayAppointmentMenu() const;
    void displayEmergencyMenu() const;

public:
    Receptionist();

    // Main receptionist dashboard
    void displayDashboard() const;

    // Patient registration and management
    void registerNewPatient(
        PatientManager& patientManager
    );

    void searchPatient(
        PatientManager& patientManager
    ) const;

    void viewAllPatients(
        PatientManager& patientManager
    ) const;

    void updatePatientDetails(
        PatientManager& patientManager
    );

    // Appointment management
    void manageAppointmentRequests(
        AppointmentManager& appointmentManager
    ) const;

    void viewAppointments(
        AppointmentManager& appointmentManager
    ) const;

    // Emergency patient routing
    void viewEmergencyCases(
        EmergencyManager& emergencyManager
    ) const;

    // View available beds
    void viewAvailableBeds(
        BedManager& bedManager
    ) const;

};

#endif


#include "../include/patientportal.h"

// Constructor
PatientPortal::PatientPortal()
{
    loggedInPatient = NULL;
}

// Patient login using Patient ID and registered phone
bool PatientPortal::authenticate(
    const string& patientID,
    const string& registeredPhone,
    PatientManager& patientManager
)
{
    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient ID not found.\n";
        return false;
    }

    if (patient->phone != registeredPhone)
    {
        cout << "\nPhone number does not match our records.\n";
        return false;
    }

    loggedInPatient = patient;

    cout << "\nPatient login successful!\n";
    cout << "Welcome, " << loggedInPatient->name << "!\n";

    return true;
}

// Logout
void PatientPortal::logout()
{
    loggedInPatient = NULL;
    cout << "\nLogged out successfully.\n";
}

// Check login status
bool PatientPortal::isLoggedIn() const
{
    return loggedInPatient != NULL;
}

// Display patient dashboard
void PatientPortal::displayDashboard() const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== PATIENT PORTAL ==========\n";
    cout << "1. View Profile\n";
    cout << "2. View Vital History\n";
    cout << "3. View Medical History\n";
    cout << "4. View Appointments\n";
    cout << "5. View Appointment Requests\n";
    cout << "6. View Laboratory Reports\n";
    cout << "7. View Prescriptions\n";
    cout << "8. View Bills\n";
    cout << "9. View Outstanding Payment\n";
    cout << "10. Logout\n";
    cout << "====================================\n";
}

// View patient profile
void PatientPortal::viewProfile() const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY PROFILE ==========\n";
    loggedInPatient->displayBasicInfo();
}

// View vital history
void PatientPortal::viewVitalHistory() const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY VITAL HISTORY ==========\n";
    loggedInPatient->displayVitalHistory();
}

// View medical history
void PatientPortal::viewMedicalHistory() const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY MEDICAL HISTORY ==========\n";
    loggedInPatient->displayMedicalHistory();
}

// View appointments
void PatientPortal::viewAppointments(
    AppointmentManager& appointmentManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY APPOINTMENTS ==========\n";

    appointmentManager.displayPatientAppointments(
        loggedInPatient->patientID
    );
}

// View appointment requests
void PatientPortal::viewAppointmentRequests(
    AppointmentManager& appointmentManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY APPOINTMENT REQUESTS ==========\n";

    appointmentManager.displayPatientRequests(
        loggedInPatient->patientID
    );
}

// View laboratory reports
void PatientPortal::viewLabReports(
    LabManager& labManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY LABORATORY REPORTS ==========\n";

    labManager.displayPatientReports(
        loggedInPatient->patientID
    );
}

// View prescriptions
void PatientPortal::viewPrescriptions(
    PharmacistManager& pharmacistManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY PRESCRIPTIONS ==========\n";

    pharmacistManager.displayPatientPrescriptions(
        loggedInPatient->patientID
    );
}

// View bills
void PatientPortal::viewBills(
    BillingManager& billingManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY BILLS ==========\n";

    billingManager.displayPatientBills(
        loggedInPatient->patientID
    );
}

// View outstanding payment
void PatientPortal::viewOutstandingAmount(
    BillingManager& billingManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    double amount = billingManager.getPatientOutstandingAmount(
        loggedInPatient->patientID
    );

    cout << "\n========== OUTSTANDING PAYMENT ==========\n";
    cout << "Patient ID: " << loggedInPatient->patientID << endl;
    cout << "Outstanding Amount: Rs. " << amount << endl;
}

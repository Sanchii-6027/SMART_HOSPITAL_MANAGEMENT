
#include "../include/doctorportal.h"

// Constructor
DoctorPortal::DoctorPortal()
{
    loggedInDoctorID = "";
}

// Doctor login session
void DoctorPortal::login(const string& doctorID)
{
    loggedInDoctorID = doctorID;

    cout << "\nDoctor session started successfully.\n";
}

// Logout
void DoctorPortal::logout()
{
    loggedInDoctorID = "";

    cout << "\nDoctor logged out successfully.\n";
}

// Check login status
bool DoctorPortal::isLoggedIn() const
{
    return !loggedInDoctorID.empty();
}

// Display Doctor Dashboard
void DoctorPortal::displayDashboard() const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== DOCTOR DASHBOARD ==========\n";
    cout << "Doctor ID: " << loggedInDoctorID << endl;

    cout << "\n1. View Assigned Patients\n";
    cout << "2. View Patient Records\n";
    cout << "3. Add Diagnosis and Medical Notes\n";
    cout << "4. View Appointments\n";
    cout << "5. Order Laboratory Test\n";
    cout << "6. View Laboratory Reports\n";
    cout << "7. Create Prescription\n";
    cout << "8. View Emergency Cases\n";
    cout << "9. Recommend Admission\n";
    cout << "10. Recommend Discharge\n";
    cout << "11. View Patient Bill\n";
    cout << "12. Logout\n";

    cout << "======================================\n";
}

// View Assigned Patients
void DoctorPortal::viewAssignedPatients(
    PatientManager& patientManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== ASSIGNED PATIENTS ==========\n";

    cout << "Doctor ID: " << loggedInDoctorID << endl;

    cout << "\nNote: Doctor-wise patient filtering requires "
         << "a public search function in PatientManager.\n";

    cout << "The filtering function will be connected during integration.\n";
}

// View Patient Records
void DoctorPortal::viewPatientRecords(
    PatientManager& patientManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    if (patient->doctorID != loggedInDoctorID)
    {
        cout << "\nAccess denied. This patient is not assigned to you.\n";
        return;
    }

    cout << "\n========== PATIENT RECORDS ==========\n";

    patient->displayBasicInfo();

    cout << "\n---------- VITAL HISTORY ----------\n";
    patient->displayVitalHistory();

    cout << "\n---------- MEDICAL HISTORY ----------\n";
    patient->displayMedicalHistory();
}

// Add Diagnosis and Medical Notes
void DoctorPortal::addMedicalRecord(
    PatientManager& patientManager,
    const string& patientID
)
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    if (patient->doctorID != loggedInDoctorID)
    {
        cout << "\nAccess denied. Patient is not assigned to you.\n";
        return;
    }

    MedicalRecord record;

    record.doctorID = loggedInDoctorID;
    record.doctorName = patient->doctorName;

    cout << "\n========== ADD MEDICAL RECORD ==========\n";

    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin >> ws, record.date);

    cout << "Enter Symptoms: ";
    getline(cin, record.symptoms);

    cout << "Enter Diagnosis: ";
    getline(cin, record.diagnosis);

    cout << "Enter Clinical Notes: ";
    getline(cin, record.clinicalNotes);

    cout << "Enter Treatment: ";
    getline(cin, record.treatment);

    patient->addMedicalRecord(record);

    if (patientManager.saveAllData())
    {
        cout << "\nMedical record saved successfully.\n";
    }
    else
    {
        cout << "\nWarning: Unable to save medical record to file.\n";
    }
}

// View Appointments
void DoctorPortal::viewAppointments(
    AppointmentManager& appointmentManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== MY APPOINTMENTS ==========\n";

    appointmentManager.displayDoctorAppointments(
        loggedInDoctorID
    );
}

// Order Laboratory Test
void DoctorPortal::orderLabTest(
    PatientManager& patientManager,
    LabManager& labManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    if (patient->doctorID != loggedInDoctorID)
    {
        cout << "\nAccess denied. Patient is not assigned to you.\n";
        return;
    }

    LabRecord record;

    record.patientID = patientID;
    record.doctorID = loggedInDoctorID;

    cout << "\n========== ORDER LAB TEST ==========\n";

    cout << "Enter Test Name: ";
    getline(cin >> ws, record.testName);

    cout << "Enter Order Date (YYYY-MM-DD): ";
    getline(cin, record.orderDate);

    record.sampleStatus = "Pending";
    record.result = "";
    record.referenceRange = "";
    record.abnormalFlag = "";
    record.reportDate = "";
    record.labStaffID = "";

    if (labManager.orderTest(record))
    {
        cout << "\nLaboratory test ordered successfully.\n";
    }
    else
    {
        cout << "\nUnable to place laboratory order.\n";
    }
}

// View Laboratory Reports
void DoctorPortal::viewLabReports(
    LabManager& labManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== LABORATORY REPORTS ==========\n";

    labManager.displayPatientReports(patientID);
}

// Create Prescription
void DoctorPortal::createPrescription(
    PatientManager& patientManager,
    PharmacistManager& pharmacistManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    if (patient->doctorID != loggedInDoctorID)
    {
        cout << "\nAccess denied. Patient is not assigned to you.\n";
        return;
    }

    Prescription prescription;

    prescription.patientID = patientID;
    prescription.doctorID = loggedInDoctorID;

    cout << "\n========== CREATE PRESCRIPTION ==========\n";

    cout << "Enter Medicine ID: ";
    getline(cin >> ws, prescription.medicineID);

    cout << "Enter Medicine Name: ";
    getline(cin, prescription.medicineName);

    cout << "Enter Prescribed Quantity: ";
    cin >> prescription.prescribedQuantity;

    cout << "Enter Prescription Date (YYYY-MM-DD): ";
    cin >> prescription.prescriptionDate;

    prescription.pharmacistID = "";
    prescription.totalPrice = 0;
    prescription.status = "Pending";

    if (prescription.prescribedQuantity <= 0)
    {
        cout << "\nInvalid medicine quantity.\n";
        return;
    }

    if (pharmacistManager.issuePrescription(prescription))
    {
        cout << "\nPrescription processed successfully.\n";
    }
    else
    {
        cout << "\nUnable to process prescription.\n";
    }
}

// View Emergency Cases
void DoctorPortal::viewEmergencyCases(
    EmergencyManager& emergencyManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== EMERGENCY CASES ==========\n";

    emergencyManager.displayAllEmergencies();
}

// Recommend Admission
void DoctorPortal::recommendAdmission(
    PatientManager& patientManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    if (patient->doctorID != loggedInDoctorID)
    {
        cout << "\nAccess denied. Patient is not assigned to you.\n";
        return;
    }

    if (patient->isAdmitted)
    {
        cout << "\nPatient is already admitted.\n";
        return;
    }

    patient->status = "Admission Recommended";

    if (patientManager.saveAllData())
    {
        cout << "\nAdmission recommendation saved.\n";
        cout << "Nurse will handle the actual bed assignment.\n";
    }
    else
    {
        cout << "\nUnable to save admission recommendation.\n";
    }
}

// Recommend Discharge
void DoctorPortal::recommendDischarge(
    PatientManager& patientManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (patient == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    if (patient->doctorID != loggedInDoctorID)
    {
        cout << "\nAccess denied. Patient is not assigned to you.\n";
        return;
    }

    if (!patient->isAdmitted)
    {
        cout << "\nPatient is not currently admitted.\n";
        return;
    }

    patient->status = "Discharge Recommended";

    if (patientManager.saveAllData())
    {
        cout << "\nDischarge recommendation saved.\n";
    }
    else
    {
        cout << "\nUnable to save discharge recommendation.\n";
    }
}

// View Patient Bill
void DoctorPortal::viewPatientBill(
    BillingManager& billingManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== PATIENT BILL ==========\n";

    billingManager.displayPatientBills(patientID);
}

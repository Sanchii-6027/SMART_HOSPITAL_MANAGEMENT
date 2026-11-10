
#include "../include/nurseportal.h"

// Constructor
NursePortal::NursePortal()
{
    loggedInNurseID = "";
    assignedDoctorID = "";
}

// Check patient access
bool NursePortal::hasPatientAccess(const Patient* patient) const
{
    if (patient == NULL)
        return false;

    return patient->doctorID == assignedDoctorID;
}

// Nurse Login
bool NursePortal::login(
    const string& nurseID,
    NurseManager& nurseManager
)
{
    Nurse* nurse = nurseManager.searchByID(nurseID);

    if (nurse == NULL)
    {
        cout << "\nNurse ID not found.\n";
        return false;
    }

    loggedInNurseID = nurse->nurseID;
    assignedDoctorID = nurse->assignedDoctorID;

    cout << "\nNurse login successful!\n";
    cout << "Welcome, " << nurse->name << "!\n";
    cout << "Assigned Doctor: " << assignedDoctorID << endl;

    return true;
}

// Logout
void NursePortal::logout()
{
    loggedInNurseID = "";
    assignedDoctorID = "";

    cout << "\nNurse logged out successfully.\n";
}

// Check login status
bool NursePortal::isLoggedIn() const
{
    return !loggedInNurseID.empty();
}

// Display Nurse Dashboard
void NursePortal::displayDashboard() const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== NURSE DASHBOARD ==========\n";
    cout << "Nurse ID: " << loggedInNurseID << endl;
    cout << "Assigned Doctor ID: " << assignedDoctorID << endl;

    cout << "\n1. View Assigned Patients\n";
    cout << "2. View Patient Details\n";
    cout << "3. Record Vital Signs\n";
    cout << "4. View Medical Instructions\n";
    cout << "5. Assign Bed\n";
    cout << "6. Release Bed\n";
    cout << "7. Assign Equipment\n";
    cout << "8. Return Equipment\n";
    cout << "9. Logout\n";

    cout << "=====================================\n";
}

// View Assigned Patients
void NursePortal::viewAssignedPatients(
    PatientManager& patientManager
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    cout << "\n========== ASSIGNED PATIENTS ==========\n";

    cout << "Nurse ID: " << loggedInNurseID << endl;
    cout << "Assigned Doctor ID: " << assignedDoctorID << endl;

    cout << "\nDoctor-wise patient listing will be connected "
         << "through PatientManager during integration.\n";
}

// View Patient Details
void NursePortal::viewPatientDetails(
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

    if (!hasPatientAccess(patient))
    {
        cout << "\nAccess denied or patient not found.\n";
        return;
    }

    cout << "\n========== PATIENT DETAILS ==========\n";

    patient->displayBasicInfo();

    cout << "\n---------- VITAL HISTORY ----------\n";
    patient->displayVitalHistory();

    cout << "\n---------- MEDICAL HISTORY ----------\n";
    patient->displayMedicalHistory();
}

// Record Patient Vital Signs
void NursePortal::recordVitals(
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

    if (!hasPatientAccess(patient))
    {
        cout << "\nAccess denied or patient not found.\n";
        return;
    }

    VitalRecord record;

    cout << "\n========== RECORD VITAL SIGNS ==========\n";

    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin >> ws, record.date);

    cout << "Enter Systolic BP (mmHg): ";
    cin >> record.systolicBP;

    cout << "Enter Diastolic BP (mmHg): ";
    cin >> record.diastolicBP;

    cout << "Enter Blood Sugar: ";
    cin >> record.bloodSugar;

    cout << "Enter Temperature (Celsius): ";
    cin >> record.temperature;

    cout << "Enter Heart Rate (bpm): ";
    cin >> record.heartRate;

    cout << "Enter Oxygen Level (SpO2 %): ";
    cin >> record.oxygenLevel;

    cout << "Enter Height (cm): ";
    cin >> record.height;

    cout << "Enter Weight (kg): ";
    cin >> record.weight;

    if (record.height <= 0 || record.weight <= 0)
    {
        cout << "\nInvalid height or weight.\n";
        return;
    }

    // BMI calculation
    double heightInMeters = record.height / 100.0;

    record.BMI = record.weight /
                 (heightInMeters * heightInMeters);

    record.nurseID = loggedInNurseID;

    // Basic abnormal reading alerts
    record.abnormalFlags = "";

    if (record.systolicBP < 90 || record.systolicBP > 140 ||
        record.diastolicBP < 60 || record.diastolicBP > 90)
    {
        record.abnormalFlags += "BP Alert; ";
    }

    if (record.temperature < 36.0 || record.temperature > 37.5)
    {
        record.abnormalFlags += "Temperature Alert; ";
    }

    if (record.heartRate < 60 || record.heartRate > 100)
    {
        record.abnormalFlags += "Heart Rate Alert; ";
    }

    if (record.oxygenLevel < 95)
    {
        record.abnormalFlags += "Low Oxygen Alert; ";
    }

    if (record.abnormalFlags.empty())
    {
        record.abnormalFlags = "No basic alerts";
    }

    cout << "\nCalculated BMI: " << record.BMI << endl;
    cout << "Reading Alerts: " << record.abnormalFlags << endl;

    patient->addVitalRecord(record);

    if (patientManager.saveAllData())
    {
        cout << "\nVital record saved successfully.\n";
    }
    else
    {
        cout << "\nWarning: Unable to save vital record.\n";
    }
}

// View Medical Instructions
void NursePortal::viewMedicalInstructions(
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

    if (!hasPatientAccess(patient))
    {
        cout << "\nAccess denied or patient not found.\n";
        return;
    }

    cout << "\n========== DOCTOR'S MEDICAL INSTRUCTIONS ==========\n";

    patient->displayMedicalHistory();
}

// Assign Bed
void NursePortal::assignPatientBed(
    PatientManager& patientManager,
    BedManager& bedManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (!hasPatientAccess(patient))
    {
        cout << "\nAccess denied or patient not found.\n";
        return;
    }

    if (patient->isAdmitted)
    {
        cout << "\nPatient is already admitted.\n";
        return;
    }

    if (patient->status != "Admission Recommended")
    {
        cout << "\nDoctor admission recommendation is required first.\n";
        return;
    }

    string bedID;
    string admissionDate;

    cout << "\nEnter Available Bed ID: ";
    cin >> bedID;

    cout << "Enter Admission Date (YYYY-MM-DD): ";
    cin >> admissionDate;

    if (!bedManager.assignBed(bedID, patientID))
    {
        cout << "\nUnable to assign bed. Check bed availability.\n";
        return;
    }

    Bed* bed = bedManager.searchBed(bedID);

    patient->isAdmitted = true;
    patient->admissionDate = admissionDate;
    patient->roomID = bed->roomID;
    patient->bedID = bedID;
    patient->status = "Admitted";

    if (patientManager.saveAllData())
    {
        cout << "\nBed assigned successfully!\n";
        cout << "Room ID: " << patient->roomID << endl;
        cout << "Bed ID: " << patient->bedID << endl;
    }
    else
    {
        cout << "\nWarning: Patient admission details could not be saved.\n";
        cout << "Please verify the bed and patient records.\n";
    }
}

// Release Bed
void NursePortal::releasePatientBed(
    PatientManager& patientManager,
    BedManager& bedManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (!hasPatientAccess(patient))
    {
        cout << "\nAccess denied or patient not found.\n";
        return;
    }

    if (!patient->isAdmitted || patient->bedID.empty())
    {
        cout << "\nPatient does not have an assigned bed.\n";
        return;
    }

    string dischargeDate;

    cout << "\nEnter Discharge Date (YYYY-MM-DD): ";
    cin >> dischargeDate;

    if (!bedManager.releaseBed(patient->bedID))
    {
        cout << "\nUnable to release bed.\n";
        return;
    }

    patient->isAdmitted = false;
    patient->dischargeDate = dischargeDate;
    patient->roomID = "";
    patient->bedID = "";
    patient->status = "Discharged";

    if (patientManager.saveAllData())
    {
        cout << "\nBed released successfully.\n";
        cout << "Patient discharge details saved.\n";
    }
    else
    {
        cout << "\nWarning: Patient discharge details could not be saved.\n";
    }
}

// Assign Equipment
void NursePortal::assignEquipment(
    PatientManager& patientManager,
    ResourceManager& resourceManager,
    const string& patientID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    Patient* patient = patientManager.searchByID(patientID);

    if (!hasPatientAccess(patient))
    {
        cout << "\nAccess denied or patient not found.\n";
        return;
    }

    ResourceAssignment assignment;

    assignment.patientID = patientID;
    assignment.staffID = loggedInNurseID;

    cout << "\n========== ASSIGN EQUIPMENT ==========\n";

    cout << "Enter Resource ID: ";
    cin >> assignment.resourceID;

    cout << "Enter Quantity: ";
    cin >> assignment.quantity;

    if (assignment.quantity <= 0)
    {
        cout << "\nInvalid quantity.\n";
        return;
    }

    if (resourceManager.assignResource(assignment))
    {
        cout << "\nEquipment assigned successfully.\n";
    }
    else
    {
        cout << "\nUnable to assign equipment.\n";
        cout << "Check resource availability and quantity.\n";
    }
}

// Return Equipment
void NursePortal::returnEquipment(
    ResourceManager& resourceManager,
    const string& assignmentID
) const
{
    if (!isLoggedIn())
    {
        cout << "\nPlease login first.\n";
        return;
    }

    if (resourceManager.returnResource(assignmentID))
    {
        cout << "\nEquipment returned successfully.\n";
    }
    else
    {
        cout << "\nUnable to return equipment.\n";
    }
}

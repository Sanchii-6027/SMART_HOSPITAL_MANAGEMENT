#include "../include/receptionist.h"

// Constructor
Receptionist::Receptionist()
{
}

// Main Receptionist Dashboard
void Receptionist::displayDashboard() const
{
    cout << "\n========== RECEPTIONIST DASHBOARD ==========\n";
    cout << "1. Patient Management\n";
    cout << "2. Appointment Management\n";
    cout << "3. View Emergency Cases\n";
    cout << "4. View Available Beds\n";
    cout << "5. Logout\n";
    cout << "============================================\n";
}

// Patient Menu
void Receptionist::displayPatientMenu() const
{
    cout << "\n========== PATIENT MANAGEMENT ==========\n";
    cout << "1. Register New Patient\n";
    cout << "2. Search Patient\n";
    cout << "3. View All Patients\n";
    cout << "4. Update Patient Details\n";
    cout << "5. Back\n";
    cout << "========================================\n";
}

// Appointment Menu
void Receptionist::displayAppointmentMenu() const
{
    cout << "\n========== APPOINTMENT MANAGEMENT ==========\n";
    cout << "1. View Appointment Requests\n";
    cout << "2. Confirm Appointment Request\n";
    cout << "3. Reject Appointment Request\n";
    cout << "4. View All Appointments\n";
    cout << "5. Back\n";
    cout << "============================================\n";
}

// Emergency Menu
void Receptionist::displayEmergencyMenu() const
{
    cout << "\n========== EMERGENCY CASES ==========\n";
    cout << "1. View All Emergency Cases\n";
    cout << "2. View Emergency Priority Queue\n";
    cout << "3. Back\n";
    cout << "=====================================\n";
}

// Register New Patient and Assign Doctor
void Receptionist::registerNewPatient(
    PatientManager& patientManager,
    DoctorManager& doctorManager
)
{
    Patient patient;

    cout << "\n========== NEW PATIENT REGISTRATION ==========\n";

    cout << "Enter Patient Name: ";
    cin.ignore(10000, '\n');
    getline(cin, patient.name);

    cout << "Enter Age: ";
    cin >> patient.age;
    cin.ignore(10000, '\n');

    cout << "Enter Gender: ";
    getline(cin, patient.gender);

    cout << "Enter Phone Number: ";
    getline(cin, patient.phone);

    cout << "Enter Address: ";
    getline(cin, patient.address);

    cout << "\n========== EMERGENCY CONTACT ==========\n";

    cout << "Emergency Contact Name: ";
    getline(cin, patient.emergencyContactName);

    cout << "Emergency Contact Phone: ";
    getline(cin, patient.emergencyContactPhone);

    cout << "Emergency Contact Relation: ";
    getline(cin, patient.emergencyContactRelation);

    cout << "\n========== MEDICAL INFORMATION ==========\n";

    cout << "Blood Group: ";
    getline(cin, patient.bloodGroup);

    cout << "Known Allergies (enter None if not applicable): ";
    getline(cin, patient.allergies);

    cout << "Existing Medical Conditions: ";
    getline(cin, patient.existingConditions);

    cout << "Current Medicines: ";
    getline(cin, patient.currentMedicines);

    cout << "Initial Symptoms: ";
    getline(cin, patient.initialSymptoms);

    // Initialize patient status
    patient.status = "Registered";
    patient.isEmergency = false;
    patient.emergencyID = "";
    patient.emergencySeverity = "";

    patient.isAdmitted = false;
    patient.admissionDate = "";
    patient.dischargeDate = "";
    patient.roomID = "";
    patient.bedID = "";

    patient.department = "";
    patient.doctorID = "";
    patient.doctorName = "";

    // Display registered doctors
    cout << "\n========== AVAILABLE DOCTORS ==========\n";
    doctorManager.displayAllDoctors();

    string selectedDoctorID;

    cout << "\nEnter Doctor ID to assign: ";
    getline(cin, selectedDoctorID);

    Doctor* selectedDoctor =
        doctorManager.searchByID(selectedDoctorID);

    if (selectedDoctor == NULL)
    {
        cout << "\nInvalid Doctor ID. Patient was not registered.\n";
        return;
    }

    int activeCount =
        patientManager.countActivePatientsForDoctor(selectedDoctorID);

    string effectiveStatus =
        doctorManager.getEffectiveAvailability(
            selectedDoctorID,
            activeCount
        );

    cout << "\nDoctor: " << selectedDoctor->name;
    cout << "\nActive Patients: " << activeCount << "/"
         << MAX_PATIENTS_PER_DOCTOR;
    cout << "\nAvailability: " << effectiveStatus << '\n';

    if (!doctorManager.canAcceptPatient(
            selectedDoctorID, activeCount))
    {
        cout << "\nThis doctor cannot accept another patient.\n";
        cout << "Please select another available doctor.\n";
        return;
    }

    // Assign doctor to patient
    patient.doctorID = selectedDoctor->doctorID;
    patient.doctorName = selectedDoctor->name;
    patient.department = selectedDoctor->department;

    // Register patient
    if (patientManager.registerPatient(patient))
    {
        cout << "\nPatient registered successfully!\n";
        cout << "Generated Patient ID: "
             << patient.patientID << '\n';

        cout << "Assigned Doctor: "
             << patient.doctorName << '\n';

        cout << "Department: "
             << patient.department << '\n';

        cout << "Doctor Capacity: "
             << activeCount + 1 << "/"
             << MAX_PATIENTS_PER_DOCTOR << '\n';

        if (activeCount + 1 == MAX_PATIENTS_PER_DOCTOR)
        {
            cout << "\nDoctor has reached maximum patient capacity.\n";
            cout << "Effective Availability: Unavailable\n";
        }
    }
    else
    {
        cout << "\nPatient registration failed.\n";
        cout << "Please check the entered details or duplicate records.\n";
    }
}

// Search Patient
void Receptionist::searchPatient(
    PatientManager& patientManager
) const
{
    int choice;

    cout << "\n========== SEARCH PATIENT ==========\n";
    cout << "1. Search by Patient ID\n";
    cout << "2. Search by Name\n";
    cout << "3. Search by Phone Number\n";
    cout << "Enter choice: ";
    cin >> choice;

    cin.ignore(10000, '\n');

    Patient* patient = NULL;
    string searchValue;

    switch (choice)
    {
        case 1:
            cout << "Enter Patient ID: ";
            getline(cin, searchValue);
            patient = patientManager.searchByID(searchValue);
            break;

        case 2:
            cout << "Enter Patient Name: ";
            getline(cin, searchValue);
            patient = patientManager.searchByName(searchValue);
            break;

        case 3:
            cout << "Enter Phone Number: ";
            getline(cin, searchValue);
            patient = patientManager.searchByPhone(searchValue);
            break;

        default:
            cout << "\nInvalid choice.\n";
            return;
    }

    if (patient != NULL)
    {
        cout << "\nPatient Found!\n";
        patient->displayBasicInfo();
    }
    else
    {
        cout << "\nPatient not found.\n";
    }
}

// View All Patients
void Receptionist::viewAllPatients(
    PatientManager& patientManager
) const
{
    cout << "\n========== ALL REGISTERED PATIENTS ==========\n";
    patientManager.displayAllPatients();
}

// Update Patient Details
void Receptionist::updatePatientDetails(
    PatientManager& patientManager
)
{
    string patientID;

    cout << "\nEnter Patient ID to update: ";
    cin.ignore(10000, '\n');
    getline(cin, patientID);

    if (patientManager.searchByID(patientID) == NULL)
    {
        cout << "\nPatient not found.\n";
        return;
    }

    patientManager.updatePatient(patientID);
}

// Manage Appointment Requests
void Receptionist::manageAppointmentRequests(
    AppointmentManager& appointmentManager
) const
{
    int choice;

    do
    {
        displayAppointmentMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                appointmentManager.displayAppointmentRequests();
                break;

            case 2:
            {
                string requestID;

                cout << "Enter Request ID to confirm: ";
                cin >> requestID;

                if (appointmentManager.confirmAppointment(requestID))
                    cout << "\nAppointment confirmed successfully.\n";
                else
                    cout << "\nUnable to confirm appointment.\n";

                break;
            }

            case 3:
            {
                string requestID;

                cout << "Enter Request ID to reject: ";
                cin >> requestID;

                if (appointmentManager.rejectAppointment(requestID))
                    cout << "\nAppointment request rejected.\n";
                else
                    cout << "\nUnable to reject request.\n";

                break;
            }

            case 4:
                appointmentManager.displayAllAppointments();
                break;

            case 5:
                cout << "\nReturning to Receptionist Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 5);
}

// View All Appointments
void Receptionist::viewAppointments(
    AppointmentManager& appointmentManager
) const
{
    appointmentManager.displayAllAppointments();
}

// View Emergency Cases
void Receptionist::viewEmergencyCases(
    EmergencyManager& emergencyManager
) const
{
    int choice;

    do
    {
        displayEmergencyMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                emergencyManager.displayAllEmergencies();
                break;

            case 2:
                emergencyManager.displayEmergencyQueue();
                break;

            case 3:
                cout << "\nReturning to Receptionist Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 3);
}

// View Available Beds
void Receptionist::viewAvailableBeds(
    BedManager& bedManager
) const
{
    cout << "\n========== AVAILABLE BEDS ==========\n";
    bedManager.displayAvailableBeds();
}
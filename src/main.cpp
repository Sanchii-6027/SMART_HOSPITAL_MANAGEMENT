
#include <iostream>
#include <string>

#include "../include/login.h"
#include "../include/admin.h"
#include "../include/receptionist.h"
#include "../include/doctorportal.h"
#include "../include/nurseportal.h"
#include "../include/patientportal.h"
#include "../include/emergency.h"
#include "../include/lab.h"
#include "../include/pharmacist.h"
#include "../include/resource.h"
#include "../include/bed.h"
#include "../include/appointment.h"
#include "../include/billing.h"
#include "../include/auditlog.h"
#include "../include/department.h"
#include "../include/doctor.h"
#include "../include/nurse.h"
#include "../include/patientmanager.h"

using namespace std;

// Clear input buffer
void clearInput()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

// ADMIN DASHBOARD
void runAdmin(
    Admin& admin,
    Login& loginManager,
    DoctorManager& doctorManager,
    NurseManager& nurseManager,
    DepartmentManager& departmentManager,
    ResourceManager& resourceManager,
    BedManager& bedManager,
    PatientManager& patientManager,
    AuditLogManager& auditManager
)
{
    int choice;

    do
    {
        admin.displayDashboard();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
               admin.manageStaff(
               loginManager,
              doctorManager,
              nurseManager,
              departmentManager
);

            case 2:
                admin.manageDepartments(departmentManager);
                break;

            case 3:
                admin.manageResources(resourceManager);
                break;

            case 4:
                admin.manageBeds(bedManager);
                break;

            case 5:
                admin.managePatients(patientManager);
                break;

            case 6:
                admin.manageAuditLogs(auditManager);
                break;

            case 7:
                cout << "\nAdmin logged out.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 7);
}

// RECEPTIONIST DASHBOARD
void runReceptionist(
    Receptionist& receptionist,
    PatientManager& patientManager,
    DoctorManager& doctorManager,
    AppointmentManager& appointmentManager,
    EmergencyManager& emergencyManager,
    BedManager& bedManager
)
{
    int choice;

    do
    {
        receptionist.displayDashboard();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                receptionist.registerNewPatient(
    patientManager,
    doctorManager
);
                break;

            case 2:
                receptionist.searchPatient(patientManager);
                break;

            case 3:
                receptionist.viewAllPatients(patientManager);
                break;

            case 4:
                receptionist.updatePatientDetails(patientManager);
                break;

            case 5:
                receptionist.manageAppointmentRequests(
                    appointmentManager
                );
                break;

            case 6:
                receptionist.viewAppointments(appointmentManager);
                break;

            case 7:
                receptionist.viewEmergencyCases(emergencyManager);
                break;

            case 8:
                receptionist.viewAvailableBeds(bedManager);
                break;

            case 9:
                cout << "\nReceptionist logged out.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 9);
}

// DOCTOR DASHBOARD
void runDoctor(
    DoctorPortal& doctorPortal,
    PatientManager& patientManager,
    AppointmentManager& appointmentManager,
    LabManager& labManager,
    PharmacistManager& pharmacistManager,
    EmergencyManager& emergencyManager,
    BillingManager& billingManager
)
{
    int choice;
    string patientID;

    do
    {
        doctorPortal.displayDashboard();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                doctorPortal.viewAssignedPatients(patientManager);
                break;

            case 2:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.viewPatientRecords(
                    patientManager, patientID
                );
                break;

            case 3:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.addMedicalRecord(
                    patientManager, patientID
                );
                break;

            case 4:
                doctorPortal.viewAppointments(appointmentManager);
                break;

            case 5:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.orderLabTest(
                    patientManager, labManager, patientID
                );
                break;

            case 6:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.viewLabReports(
                    labManager, patientID
                );
                break;

            case 7:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.createPrescription(
                    patientManager, pharmacistManager, patientID
                );
                break;

            case 8:
                doctorPortal.viewEmergencyCases(emergencyManager);
                break;

            case 9:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.recommendAdmission(
                    patientManager, patientID
                );
                break;

            case 10:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.recommendDischarge(
                    patientManager, patientID
                );
                break;

            case 11:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                doctorPortal.viewPatientBill(
                    billingManager, patientID
                );
                break;

            case 12:
                doctorPortal.logout();
                cout << "\nDoctor logged out.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 12);
}

// NURSE DASHBOARD
void runNurse(
    NursePortal& nursePortal,
    PatientManager& patientManager,
    NurseManager& nurseManager,
    BedManager& bedManager,
    ResourceManager& resourceManager
)
{
    int choice;
    string patientID;
    string assignmentID;

    do
    {
        nursePortal.displayDashboard();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                nursePortal.viewAssignedPatients(patientManager);
                break;

            case 2:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                nursePortal.viewPatientDetails(patientManager, patientID);
                break;

            case 3:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                nursePortal.recordVitals(patientManager, patientID);
                break;

            case 4:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                nursePortal.viewMedicalInstructions(patientManager, patientID);
                break;

            case 5:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                nursePortal.assignPatientBed(
                    patientManager, bedManager, patientID
                );
                break;

            case 6:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                nursePortal.releasePatientBed(
                    patientManager, bedManager, patientID
                );
                break;

            case 7:
                cout << "Enter Patient ID: ";
                cin >> patientID;
                nursePortal.assignEquipment(
                    patientManager, resourceManager, patientID
                );
                break;

            case 8:
                cout << "Enter Assignment ID: ";
                cin >> assignmentID;
                nursePortal.returnEquipment(resourceManager, assignmentID);
                break;

            case 9:
                cout << "Enter Patient ID to discharge: ";
                cin >> patientID;
                nursePortal.dischargePatient(
                    patientManager, bedManager, patientID
                );
                break;

            case 10:
                nursePortal.logout();
                cout << "\nNurse logged out.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 10);
}



// PATIENT PORTAL
void runPatientPortal(
    PatientPortal& portal,
    PatientManager& patientManager,
    AppointmentManager& appointmentManager,
    LabManager& labManager,
    PharmacistManager& pharmacistManager,
    BillingManager& billingManager
)
{
    string patientID, phone;
    int choice;

    cout << "\n========== PATIENT LOGIN ==========\n";

    cout << "Enter Patient ID: ";
    cin >> patientID;

    cout << "Enter Registered Phone: ";
    cin >> phone;

    if (!portal.authenticate(patientID, phone, patientManager))
    {
        cout << "\nPatient login failed.\n";
        return;
    }

    do
    {
        portal.displayDashboard();

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                portal.viewProfile();
                break;

            case 2:
                portal.viewVitalHistory();
                break;

            case 3:
                portal.viewMedicalHistory();
                break;

            case 4:
                portal.viewAppointments(appointmentManager);
                break;

            case 5:
                portal.viewAppointmentRequests(appointmentManager);
                break;

            case 6:
                portal.viewLabReports(labManager);
                break;

            case 7:
                portal.viewPrescriptions(pharmacistManager);
                break;

            case 8:
                portal.viewBills(billingManager);
                break;

            case 9:
                portal.viewOutstandingAmount(billingManager);
                break;

            case 10:
                portal.logout();
                cout << "\nPatient logged out.\n";
                break;

            default:
                cout << "\nInvalid choice.\n";
        }

    } while (choice != 10);
}

// EMERGENCY MANAGEMENT
void runEmergency(EmergencyManager& emergencyManager)
{
    int choice;
    string emergencyID, status;

    do
    {
        cout << "\n====== EMERGENCY MANAGEMENT ======\n";
        cout << "1. View Emergency Queue\n";
        cout << "2. View All Emergency Cases\n";
        cout << "3. Search Emergency Case\n";
        cout << "4. Update Emergency Status\n";
        cout << "5. Logout\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                emergencyManager.displayEmergencyQueue();
                break;

            case 2:
                emergencyManager.displayAllEmergencies();
                break;

            case 3:
                cout << "Enter Emergency ID: ";
                cin >> emergencyID;
                emergencyManager.displayEmergency(emergencyID);
                break;

            case 4:
                cout << "Enter Emergency ID: ";
                cin >> emergencyID;

                cout << "Enter New Status: ";
                cin.ignore();
                getline(cin, status);

                if (emergencyManager.updateEmergencyStatus(
                    emergencyID, status))
                    cout << "Status updated.\n";
                else
                    cout << "Unable to update status.\n";

                break;

            case 5:
                cout << "Emergency staff logged out.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}

// LABORATORY DASHBOARD
void runLaboratory(LabManager& labManager)
{
    int choice;
    string orderID, result, range, date, staffID, flag, status;

    do
    {
        cout << "\n========== LABORATORY DASHBOARD ==========\n";
        cout << "1. View All Test Records\n";
        cout << "2. View Pending Samples\n";
        cout << "3. Update Sample Status\n";
        cout << "4. Enter Test Result\n";
        cout << "5. Logout\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                labManager.displayAllRecords();
                break;

            case 2:
                labManager.displayPendingSamples();
                break;

            case 3:
                cout << "Enter Order ID: ";
                cin >> orderID;

                cout << "Enter Status (Collected/Rejected): ";
                cin >> status;

                if (labManager.updateSampleStatus(orderID, status))
                    cout << "Sample status updated.\n";
                else
                    cout << "Unable to update sample status.\n";

                break;

            case 4:
                cout << "Enter Order ID: ";
                cin >> orderID;

                cout << "Enter Result: ";
                cin.ignore();
                getline(cin, result);

                cout << "Enter Reference Range: ";
                getline(cin, range);

                cout << "Enter Report Date: ";
                getline(cin, date);

                cout << "Enter Lab Staff ID: ";
                getline(cin, staffID);

                cout << "Enter Flag (Normal/Abnormal): ";
                getline(cin, flag);

                if (labManager.enterTestResult(
                    orderID, result, range, date, staffID, flag))
                    cout << "Test result saved.\n";
                else
                    cout << "Unable to save test result.\n";

                break;

            case 5:
                cout << "Laboratory staff logged out.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}

// PHARMACIST DASHBOARD
void runPharmacist(PharmacistManager& pharmacistManager)
{
    int choice;

    do
    {
        cout << "\n========== PHARMACIST DASHBOARD ==========\n";
        cout << "1. View All Medicines\n";
        cout << "2. View Low Stock Medicines\n";
        cout << "3. View Expired Medicines\n";
        cout << "4. Search Medicine\n";
        cout << "5. Logout\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                pharmacistManager.displayAllMedicines();
                break;

            case 2:
                pharmacistManager.displayLowStockMedicines();
                break;

            case 3:
                pharmacistManager.displayExpiredMedicines();
                break;

            case 4:
            {
                string medicineID;

                cout << "Enter Medicine ID: ";
                cin >> medicineID;

                pharmacistManager.displayMedicine(medicineID);
                break;
            }

            case 5:
                cout << "Pharmacist logged out.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}

// MAIN FUNCTION
int main()
{
    // Create all managers only once
    Login loginManager;

    PatientManager patientManager;
    DoctorManager doctorManager;
    NurseManager nurseManager;

    DepartmentManager departmentManager;
    Admin admin;
    Receptionist receptionist;

    DoctorPortal doctorPortal;
    NursePortal nursePortal;
    PatientPortal patientPortal;

    EmergencyManager emergencyManager;
    LabManager labManager;
    PharmacistManager pharmacistManager;

    ResourceManager resourceManager;
    BedManager bedManager;
    AppointmentManager appointmentManager;
    BillingManager billingManager;
    AuditLogManager auditManager;

    // Create initial Admin if there are no accounts.
    // If accounts already exist, the function will not create another.
    loginManager.createInitialAdmin();

    int choice;
    Account loggedInAccount;

    do
    {
        cout << "\n";
        cout << "============================================\n";
        cout << "       SMART HOSPITAL MANAGEMENT SYSTEM\n";
        cout << "============================================\n";

        cout << "1. Admin / HR\n";
        cout << "2. Receptionist\n";
        cout << "3. Doctor\n";
        cout << "4. Patient Portal\n";
        cout << "5. Nurse\n";
        cout << "6. Emergency Management\n";
        cout << "7. Laboratory Staff\n";
        cout << "8. Pharmacist\n";
        cout << "9. Exit\n";

        cout << "============================================\n";
        cout << "Select your role: ";

        if (!(cin >> choice))
        {
            clearInput();
            cout << "\nInvalid input. Enter a number.\n";
            continue;
        }

        clearInput();

        switch (choice)
        {
            case 1:
                if (loginManager.authenticate("Admin", loggedInAccount))
                {
                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Admin logged in"
                    );

                    runAdmin(
                        admin, loginManager,
                        doctorManager, nurseManager,
                        departmentManager, resourceManager,
                        bedManager, patientManager, auditManager
                    );

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Admin logged out"
                    );
                }
                break;

            case 2:
                if (loginManager.authenticate(
                    "Receptionist", loggedInAccount))
                {
                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Receptionist logged in"
                    );

                    runReceptionist(
                           receptionist,
                           patientManager,
                            doctorManager,
                          appointmentManager,
                         emergencyManager,
                         bedManager
);

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Receptionist logged out"
                    );
                }
                break;

            case 3:
                if (loginManager.authenticate("Doctor", loggedInAccount))
                {
                    Doctor* doctor =
                        doctorManager.searchByID(loggedInAccount.id);

                    if (doctor == NULL)
                    {
                        cout << "\nNo doctor record matches this account ID.\n";
                        break;
                    }

                    doctorPortal.login(loggedInAccount.id);

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Doctor logged in"
                    );

                    runDoctor(
                        doctorPortal, patientManager,
                        appointmentManager, labManager,
                        pharmacistManager, emergencyManager,
                        billingManager
                    );

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Doctor logged out"
                    );
                }
                break;

            case 4:
                runPatientPortal(
                    patientPortal, patientManager,
                    appointmentManager, labManager,
                    pharmacistManager, billingManager
                );
                break;

            case 5:
                if (loginManager.authenticate("Nurse", loggedInAccount))
                {
                    if (nursePortal.login(
                        loggedInAccount.id, nurseManager))
                    {
                        auditManager.addLog(
                            loggedInAccount.id,
                            loggedInAccount.role,
                            "Nurse logged in"
                        );

                        runNurse(
                            nursePortal, patientManager,
                            nurseManager, bedManager,
                            resourceManager
                        );

                        auditManager.addLog(
                            loggedInAccount.id,
                            loggedInAccount.role,
                            "Nurse logged out"
                        );
                    }
                    else
                    {
                        cout << "\nNo nurse record matches this account ID.\n";
                    }
                }
                break;

            case 6:
                if (loginManager.authenticate(
                    "Emergency Management", loggedInAccount))
                {
                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Emergency staff logged in"
                    );

                    runEmergency(emergencyManager);

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Emergency staff logged out"
                    );
                }
                break;

            case 7:
                if (loginManager.authenticate(
                    "Laboratory Staff", loggedInAccount))
                {
                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Laboratory staff logged in"
                    );

                    runLaboratory(labManager);

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Laboratory staff logged out"
                    );
                }
                break;

            case 8:
                if (loginManager.authenticate(
                    "Pharmacist", loggedInAccount))
                {
                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Pharmacist logged in"
                    );

                    runPharmacist(pharmacistManager);

                    auditManager.addLog(
                        loggedInAccount.id,
                        loggedInAccount.role,
                        "Pharmacist logged out"
                    );
                }
                break;

            case 9:
                cout << "\nThank you for using Smart Hospital Management System.\n";
                break;

            default:
                cout << "\nInvalid role selection.\n";
        }

    } while (choice != 9);

    return 0;
}

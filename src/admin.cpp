
#include "../include/admin.h"

// Constructor
Admin::Admin()
{
}

// Main Admin Dashboard
void Admin::displayDashboard() const
{
    cout << "\n========== ADMIN DASHBOARD ==========\n";
    cout << "1. Staff Management\n";
    cout << "2. Department Management\n";
    cout << "3. Resource Management\n";
    cout << "4. Bed Management\n";
    cout << "5. Patient Records\n";
    cout << "6. Audit Logs\n";
    cout << "7. Logout\n";
    cout << "=====================================\n";
}

// Staff Menu
void Admin::displayStaffMenu() const
{
    cout << "\n========== STAFF MANAGEMENT ==========\n";
    cout << "1. View Staff Accounts\n";
    cout << "2. View All Doctors\n";
    cout << "3. View All Nurses\n";
    cout << "4. Back\n";
    cout << "======================================\n";
}

// Department Menu
void Admin::displayDepartmentMenu() const
{
    cout << "\n========== DEPARTMENT MANAGEMENT ==========\n";
    cout << "1. View All Departments\n";
    cout << "2. Back\n";
    cout << "===========================================\n";
}

// Resource Menu
void Admin::displayResourceMenu() const
{
    cout << "\n========== RESOURCE MANAGEMENT ==========\n";
    cout << "1. Add Resource\n";
    cout << "2. View All Resources\n";
    cout << "3. View Available Resources\n";
    cout << "4. Back\n";
    cout << "=========================================\n";
}

// Bed Menu
void Admin::displayBedMenu() const
{
    cout << "\n========== BED MANAGEMENT ==========\n";
    cout << "1. Add Bed\n";
    cout << "2. View All Beds\n";
    cout << "3. View Available Beds\n";
    cout << "4. Back\n";
    cout << "====================================\n";
}

// Patient Menu
void Admin::displayPatientMenu() const
{
    cout << "\n========== PATIENT RECORDS ==========\n";
    cout << "1. View All Patients\n";
    cout << "2. Back\n";
    cout << "=====================================\n";
}

// Audit Menu
void Admin::displayAuditMenu() const
{
    cout << "\n========== AUDIT LOGS ==========\n";
    cout << "1. View All Logs\n";
    cout << "2. Back\n";
    cout << "===============================\n";
}

// Manage Staff
void Admin::manageStaff(
    Login& loginManager,
    DoctorManager& doctorManager,
    NurseManager& nurseManager
)
{
    int choice;

    do
    {
        displayStaffMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                loginManager.displayAccounts();
                break;

            case 2:
                doctorManager.displayAllDoctors();
                break;

            case 3:
                nurseManager.displayAllNurses();
                break;

            case 4:
                cout << "\nReturning to Admin Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 4);
}

// Manage Departments
void Admin::manageDepartments(
    DepartmentManager& departmentManager
) const
{
    int choice;

    do
    {
        displayDepartmentMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                departmentManager.displayDepartments();
                break;

            case 2:
                cout << "\nReturning to Admin Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 2);
}

// Manage Resources
void Admin::manageResources(
    ResourceManager& resourceManager
)
{
    int choice;

    do
    {
        displayResourceMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                Resource resource;

                cout << "\nEnter Resource Name: ";
                cin.ignore();
                getline(cin, resource.resourceName);

                cout << "Enter Category: ";
                getline(cin, resource.category);

                cout << "Enter Total Quantity: ";
                cin >> resource.totalQuantity;

                cout << "Enter Location: ";
                cin.ignore();
                getline(cin, resource.location);

                resource.availableQuantity = resource.totalQuantity;
                resource.status = "Available";

                if (resourceManager.addResource(resource))
                    cout << "\nResource added successfully.\n";
                else
                    cout << "\nUnable to add resource.\n";

                break;
            }

            case 2:
                resourceManager.displayAllResources();
                break;

            case 3:
                resourceManager.displayAvailableResources();
                break;

            case 4:
                cout << "\nReturning to Admin Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 4);
}

// Manage Beds
void Admin::manageBeds(BedManager& bedManager)
{
    int choice;

    do
    {
        displayBedMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                Bed bed;

                cout << "\nEnter Room ID: ";
                cin.ignore();
                getline(cin, bed.roomID);

                cout << "Enter Department: ";
                getline(cin, bed.department);

                cout << "Enter Bed Category (General/ICU/Emergency/OT): ";
                getline(cin, bed.category);

                bed.status = "Available";
                bed.patientID = "";

                if (bedManager.addBed(bed))
                    cout << "\nBed added successfully.\n";
                else
                    cout << "\nUnable to add bed.\n";

                break;
            }

            case 2:
                bedManager.displayAllBeds();
                break;

            case 3:
                bedManager.displayAvailableBeds();
                break;

            case 4:
                cout << "\nReturning to Admin Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 4);
}

// Manage Patients
void Admin::managePatients(
    PatientManager& patientManager
) const
{
    int choice;

    do
    {
        displayPatientMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                patientManager.displayAllPatients();
                break;

            case 2:
                cout << "\nReturning to Admin Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 2);
}

// Manage Audit Logs
void Admin::manageAuditLogs(
    AuditLogManager& auditManager
) const
{
    int choice;

    do
    {
        displayAuditMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                auditManager.displayAllLogs();
                break;

            case 2:
                cout << "\nReturning to Admin Dashboard.\n";
                break;

            default:
                cout << "\nInvalid choice. Try again.\n";
        }

    } while (choice != 2);
}

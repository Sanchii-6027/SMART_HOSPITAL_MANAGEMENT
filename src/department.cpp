#include "../include/department.h"

// Constructor
DepartmentManager::DepartmentManager()
{
    departmentCount = 0;
    initializeDepartments();
}

// Initialize hospital departments
void DepartmentManager::initializeDepartments()
{
    departments[0] = {"D01", "Cardiology"};
    departments[1] = {"D02", "Neurology"};
    departments[2] = {"D03", "Orthopedics"};
    departments[3] = {"D04", "General Surgery"};
    departments[4] = {"D05", "Pulmonology"};
    departments[5] = {"D06", "Pediatrics"};
    departments[6] = {"D07", "Gynecology & Obstetrics"};
    departments[7] = {"D08", "Dermatology"};
    departments[8] = {"D09", "Endocrinology"};
    departments[9] = {"D10", "General Medicine"};

    departmentCount = MAX_DEPARTMENTS;
}

// Display all departments
void DepartmentManager::displayDepartments() const
{
    cout << "\n========== HOSPITAL DEPARTMENTS ==========\n";

    for (int i = 0; i < departmentCount; i++)
    {
        cout << departments[i].departmentID
             << " - "
             << departments[i].departmentName
             << '\n';
    }
}

// Check whether a department exists
bool DepartmentManager::departmentExists(const string& name) const
{
    return findDepartment(name) != -1;
}

// Find department by name
int DepartmentManager::findDepartment(const string& name) const
{
    for (int i = 0; i < departmentCount; i++)
    {
        if (departments[i].departmentName == name)
            return i;
    }

    return -1;
}

// Get department name using index
string DepartmentManager::getDepartmentName(int index) const
{
    if (index < 0 || index >= departmentCount)
        return "";

    return departments[index].departmentName;
}

// Get total department count
int DepartmentManager::getDepartmentCount() const
{
    return departmentCount;
}
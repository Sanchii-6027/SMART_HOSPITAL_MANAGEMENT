
#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>

#include "login.h"
#include "patientmanager.h"
#include "department.h"
#include "doctor.h"
#include "nurse.h"
#include "resource.h"
#include "bed.h"
#include "auditlog.h"

using namespace std;

class Admin
{
private:
    // Display individual management menus
    void displayStaffMenu() const;
    void displayDepartmentMenu() const;
    void displayResourceMenu() const;
    void displayBedMenu() const;
    void displayPatientMenu() const;
    void displayAuditMenu() const;

public:
    Admin();

    // Main Admin dashboard
    void displayDashboard() const;

    // Admin management operations
    void manageStaff(
        Login& loginManager,
        DoctorManager& doctorManager,
        NurseManager& nurseManager
    );

    void manageDepartments(
        DepartmentManager& departmentManager
    ) const;

    void manageResources(
        ResourceManager& resourceManager
    );

    void manageBeds(
        BedManager& bedManager
    );

    void managePatients(
        PatientManager& patientManager
    ) const;

    void manageAuditLogs(
        AuditLogManager& auditManager
    ) const;
};

#endif

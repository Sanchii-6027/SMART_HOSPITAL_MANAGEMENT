
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
    void displayStaffMenu() const;
    void displayDepartmentMenu() const;
    void displayResourceMenu() const;
    void displayBedMenu() const;
    void displayPatientMenu() const;
    void displayAuditMenu() const;

    void registerDoctor(
        Login& loginManager,
        DoctorManager& doctorManager,
        DepartmentManager& departmentManager
    );

    void registerNurse(
        Login& loginManager,
        NurseManager& nurseManager,
        DoctorManager& doctorManager,
        DepartmentManager& departmentManager
    );

public:
    Admin();

    void displayDashboard() const;

    void manageStaff(
        Login& loginManager,
        DoctorManager& doctorManager,
        NurseManager& nurseManager,
        DepartmentManager& departmentManager
    );

    void manageDepartments(
        DepartmentManager& departmentManager
    ) const;

    void manageResources(ResourceManager& resourceManager);

    void manageBeds(BedManager& bedManager);

    void managePatients(PatientManager& patientManager) const;

    void manageAuditLogs(AuditLogManager& auditManager) const;
};

#endif

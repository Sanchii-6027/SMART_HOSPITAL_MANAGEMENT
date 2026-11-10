#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include <iostream>
#include <string>

using namespace std;

const int MAX_DEPARTMENTS = 10;

struct Department
{
    string departmentID;
    string departmentName;
};

class DepartmentManager
{
private:
    Department departments[MAX_DEPARTMENTS];
    int departmentCount;

    void initializeDepartments();

public:
    DepartmentManager();

    void displayDepartments() const;

    bool departmentExists(const string& name) const;

    int findDepartment(const string& name) const;

    string getDepartmentName(int index) const;

    int getDepartmentCount() const;
};

#endif
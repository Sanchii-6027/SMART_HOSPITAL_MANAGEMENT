#ifndef NURSE_H
#define NURSE_H

#include <iostream>
#include <string>

using namespace std;

struct Nurse
{
    string nurseID;
    string name;
    string username;
    string department;
    string assignedDoctorID;
    string assignedDoctorName;
    string phone;
    string availability;

    Nurse* next;

    Nurse();
};

class NurseManager
{
private:
    Nurse* head;
    Nurse* tail;

    int nurseCount;
    int nextNurseNumber;

    void appendNurse(Nurse* nurse);
    void clearNurses();

    bool loadNurses();
    bool saveNurses() const;

    string generateNurseID();

public:
    NurseManager();
    ~NurseManager();

    NurseManager(const NurseManager& other) = delete;
    NurseManager& operator=(const NurseManager& other) = delete;

    bool registerNurse(const Nurse& nurse);

    Nurse* searchByID(const string& nurseID) const;
    Nurse* searchByName(const string& name) const;

    void displayNurse(const string& nurseID) const;
    void displayAllNurses() const;

    void displayNursesByDepartment(
        const string& department) const;

    void displayNursesByDoctor(
        const string& doctorID) const;

    int getNurseCount() const;
};

#endif
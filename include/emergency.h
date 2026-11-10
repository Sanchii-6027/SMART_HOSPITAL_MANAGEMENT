
#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <iostream>
#include <string>

using namespace std;

struct EmergencyCase
{
    string emergencyID;
    string patientID;
    string reason;

    string triageDoctorID;
    string assessmentDateTime;

    string severity;

    string specialistDepartment;
    string specialistDoctorID;

    string bedStatus;
    string status;

    int arrivalOrder;

    EmergencyCase();
};

class EmergencyManager
{
private:
    EmergencyCase* heap;
    int capacity;
    int emergencyCount;
    int nextEmergencyNumber;
    int nextArrivalOrder;

    void resizeHeap();
    void swapCases(int first, int second);

    int getPriority(const string& severity) const;

    bool hasHigherPriority(
        const EmergencyCase& first,
        const EmergencyCase& second
    ) const;

    void heapifyUp(int index);
    void heapifyDown(int index);

    bool loadEmergencies();
    bool saveEmergencies() const;

    string generateEmergencyID();

public:
    EmergencyManager();
    ~EmergencyManager();

    EmergencyManager(const EmergencyManager& other) = delete;
    EmergencyManager& operator=(const EmergencyManager& other) = delete;

    bool addEmergency(const EmergencyCase& emergency);

    EmergencyCase* getHighestPriorityEmergency();

    bool removeEmergency(const string& emergencyID);

    EmergencyCase* searchEmergency(const string& emergencyID) const;

    void displayEmergency(const string& emergencyID) const;
    void displayAllEmergencies() const;

    void displayEmergencyQueue() const;

    bool updateEmergencyStatus(
        const string& emergencyID,
        const string& newStatus
    );

    int getEmergencyCount() const;
    bool isEmpty() const;
};

#endif

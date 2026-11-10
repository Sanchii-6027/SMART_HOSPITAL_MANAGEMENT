
#ifndef BED_H
#define BED_H

#include <iostream>
#include <string>

using namespace std;

struct Bed
{
    string bedID;
    string roomID;
    string department;
    string category;
    string status;
    string patientID;

    Bed* next;

    Bed();
};

class BedManager
{
private:
    Bed* head;
    Bed* tail;

    int bedCount;
    int nextBedNumber;

    void appendBed(Bed* bed);
    void clearBeds();

    string generateBedID();

    bool loadBeds();
    bool saveBeds() const;

public:
    BedManager();
    ~BedManager();

    BedManager(const BedManager& other) = delete;
    BedManager& operator=(const BedManager& other) = delete;

    // Add and search beds

    bool addBed(const Bed& bed);

    Bed* searchBed(const string& bedID) const;

    // Display beds

    void displayBed(const string& bedID) const;

    void displayAllBeds() const;

    void displayAvailableBeds() const;

    void displayBedsByCategory(const string& category) const;

    void displayBedsByDepartment(const string& department) const;

    // Bed assignment and release

    bool assignBed(
        const string& bedID,
        const string& patientID
    );

    bool releaseBed(const string& bedID);

    // Information

    int getBedCount() const;

    int getAvailableBedCount() const;

    bool isBedAvailable(const string& bedID) const;
};

#endif

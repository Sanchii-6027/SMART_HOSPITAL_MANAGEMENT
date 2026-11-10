
#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <string>

using namespace std;

struct Resource
{
    string resourceID;
    string resourceName;
    string category;

    int totalQuantity;
    int availableQuantity;

    string status;
    string location;

    Resource* next;

    Resource();
};

struct ResourceAssignment
{
    string assignmentID;
    string resourceID;
    string patientID;
    string staffID;

    int quantity;

    string assignmentDate;
    string returnDate;

    string status;

    ResourceAssignment* next;

    ResourceAssignment();
};

class ResourceManager
{
private:
    Resource* resourceHead;
    Resource* resourceTail;

    ResourceAssignment* assignmentHead;
    ResourceAssignment* assignmentTail;

    int resourceCount;
    int assignmentCount;

    int nextResourceNumber;
    int nextAssignmentNumber;

    void appendResource(Resource* resource);
    void appendAssignment(ResourceAssignment* assignment);

    void clearResources();
    void clearAssignments();

    string generateResourceID();
    string generateAssignmentID();

    bool loadResources();
    bool saveResources() const;

    bool loadAssignments();
    bool saveAssignments() const;

public:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager& other) = delete;
    ResourceManager& operator=(const ResourceManager& other) = delete;

    // Resource inventory

    bool addResource(const Resource& resource);

    Resource* searchResource(const string& resourceID) const;

    void displayResource(const string& resourceID) const;

    void displayAllResources() const;

    void displayAvailableResources() const;

    bool updateResourceQuantity(
        const string& resourceID,
        int newTotalQuantity
    );

    // Resource assignment

    bool assignResource(const ResourceAssignment& assignment);

    bool returnResource(const string& assignmentID);

    void displayAssignments() const;

    void displayPatientResources(const string& patientID) const;

    int getResourceCount() const;

    int getAssignmentCount() const;
};

#endif

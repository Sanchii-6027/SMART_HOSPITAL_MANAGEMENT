
#include "../include/resource.h"
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

// Get current date
string getResourceDate()
{
    time_t now = time(0);
    tm* currentTime = localtime(&now);

    char date[20];

    strftime(date, sizeof(date), "%Y-%m-%d", currentTime);

    return string(date);
}

// Split a line using |
int splitResourceLine(const string& line, string fields[], int maxFields)
{
    stringstream ss(line);
    string field;
    int count = 0;

    while (getline(ss, field, '|') && count < maxFields)
    {
        fields[count++] = field;
    }

    return count;
}

// Resource constructor
Resource::Resource()
{
    totalQuantity = 0;
    availableQuantity = 0;
    next = NULL;
}

// Assignment constructor
ResourceAssignment::ResourceAssignment()
{
    quantity = 0;
    next = NULL;
}

// ResourceManager constructor
ResourceManager::ResourceManager()
{
    resourceHead = NULL;
    resourceTail = NULL;

    assignmentHead = NULL;
    assignmentTail = NULL;

    resourceCount = 0;
    assignmentCount = 0;

    nextResourceNumber = 1;
    nextAssignmentNumber = 1;

    loadResources();
    loadAssignments();
}

// Destructor
ResourceManager::~ResourceManager()
{
    saveResources();
    saveAssignments();

    clearResources();
    clearAssignments();
}

// Add resource at the end
void ResourceManager::appendResource(Resource* resource)
{
    resource->next = NULL;

    if (resourceHead == NULL)
    {
        resourceHead = resourceTail = resource;
    }
    else
    {
        resourceTail->next = resource;
        resourceTail = resource;
    }

    resourceCount++;
}

// Add assignment at the end
void ResourceManager::appendAssignment(ResourceAssignment* assignment)
{
    assignment->next = NULL;

    if (assignmentHead == NULL)
    {
        assignmentHead = assignmentTail = assignment;
    }
    else
    {
        assignmentTail->next = assignment;
        assignmentTail = assignment;
    }

    assignmentCount++;
}

// Delete all resources
void ResourceManager::clearResources()
{
    Resource* current = resourceHead;

    while (current != NULL)
    {
        Resource* temp = current;
        current = current->next;
        delete temp;
    }

    resourceHead = NULL;
    resourceTail = NULL;
    resourceCount = 0;
}

// Delete all assignments
void ResourceManager::clearAssignments()
{
    ResourceAssignment* current = assignmentHead;

    while (current != NULL)
    {
        ResourceAssignment* temp = current;
        current = current->next;
        delete temp;
    }

    assignmentHead = NULL;
    assignmentTail = NULL;
    assignmentCount = 0;
}

// Generate Resource ID
string ResourceManager::generateResourceID()
{
    string id = "RES00000" + to_string(nextResourceNumber);
    nextResourceNumber++;
    return id;
}

// Generate Assignment ID
string ResourceManager::generateAssignmentID()
{
    string id = "ASN00000" + to_string(nextAssignmentNumber);
    nextAssignmentNumber++;
    return id;
}

// Save resources
bool ResourceManager::saveResources() const
{
    ofstream file("DATA/resources.txt");

    if (!file)
        return false;

    Resource* current = resourceHead;

    while (current != NULL)
    {
        file << current->resourceID << "|"
             << current->resourceName << "|"
             << current->category << "|"
             << current->totalQuantity << "|"
             << current->availableQuantity << "|"
             << current->status << "|"
             << current->location << "\n";

        current = current->next;
    }

    file.close();
    return true;
}

// Load resources
bool ResourceManager::loadResources()
{
    ifstream file("DATA/resources.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[7];

        if (splitResourceLine(line, fields, 7) != 7)
            continue;

        Resource* resource = new Resource();

        resource->resourceID = fields[0];
        resource->resourceName = fields[1];
        resource->category = fields[2];

        stringstream totalStream(fields[3]);
        stringstream availableStream(fields[4]);

        if (!(totalStream >> resource->totalQuantity) ||
            !(availableStream >> resource->availableQuantity) ||
            resource->totalQuantity < 0 ||
            resource->availableQuantity < 0 ||
            resource->availableQuantity > resource->totalQuantity)
        {
            delete resource;
            continue;
        }

        resource->status = fields[5];
        resource->location = fields[6];

        if (resource->resourceID.length() > 3 &&
            resource->resourceID.substr(0, 3) == "RES")
        {
            int number = atoi(resource->resourceID.substr(3).c_str());

            if (number >= nextResourceNumber)
                nextResourceNumber = number + 1;
        }

        appendResource(resource);
    }

    file.close();
    return true;
}

// Save assignments
bool ResourceManager::saveAssignments() const
{
    ofstream file("DATA/resource_assignments.txt");

    if (!file)
        return false;

    ResourceAssignment* current = assignmentHead;

    while (current != NULL)
    {
        file << current->assignmentID << "|"
             << current->resourceID << "|"
             << current->patientID << "|"
             << current->staffID << "|"
             << current->quantity << "|"
             << current->assignmentDate << "|"
             << current->returnDate << "|"
             << current->status << "\n";

        current = current->next;
    }

    file.close();
    return true;
}

// Load assignments
bool ResourceManager::loadAssignments()
{
    ifstream file("DATA/resource_assignments.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[8];

        if (splitResourceLine(line, fields, 8) != 8)
            continue;

        ResourceAssignment* assignment = new ResourceAssignment();

        assignment->assignmentID = fields[0];
        assignment->resourceID = fields[1];
        assignment->patientID = fields[2];
        assignment->staffID = fields[3];

        stringstream quantityStream(fields[4]);

        if (!(quantityStream >> assignment->quantity) ||
            assignment->quantity <= 0)
        {
            delete assignment;
            continue;
        }

        assignment->assignmentDate = fields[5];
        assignment->returnDate = fields[6];
        assignment->status = fields[7];

        if (assignment->assignmentID.length() > 3 &&
            assignment->assignmentID.substr(0, 3) == "ASN")
        {
            int number = atoi(assignment->assignmentID.substr(3).c_str());

            if (number >= nextAssignmentNumber)
                nextAssignmentNumber = number + 1;
        }

        appendAssignment(assignment);
    }

    file.close();
    return true;
}

// Add a new resource
bool ResourceManager::addResource(const Resource& resource)
{
    if (resource.resourceName.empty() ||
        resource.category.empty() ||
        resource.totalQuantity < 0 ||
        resource.availableQuantity < 0 ||
        resource.availableQuantity > resource.totalQuantity)
    {
        return false;
    }

    Resource* newResource = new Resource();

    *newResource = resource;
    newResource->resourceID = generateResourceID();
    newResource->next = NULL;

    if (newResource->status.empty())
        newResource->status = "Available";

    appendResource(newResource);

    return saveResources();
}

// Search resource by ID
Resource* ResourceManager::searchResource(const string& resourceID) const
{
    Resource* current = resourceHead;

    while (current != NULL)
    {
        if (current->resourceID == resourceID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Display one resource
void ResourceManager::displayResource(const string& resourceID) const
{
    Resource* resource = searchResource(resourceID);

    if (resource == NULL)
    {
        cout << "\nResource not found.\n";
        return;
    }

    cout << "\nResource ID: " << resource->resourceID;
    cout << "\nName: " << resource->resourceName;
    cout << "\nCategory: " << resource->category;
    cout << "\nTotal Quantity: " << resource->totalQuantity;
    cout << "\nAvailable Quantity: " << resource->availableQuantity;
    cout << "\nStatus: " << resource->status;
    cout << "\nLocation: " << resource->location << "\n";
}

// Display all resources
void ResourceManager::displayAllResources() const
{
    if (resourceHead == NULL)
    {
        cout << "\nNo resources available.\n";
        return;
    }

    Resource* current = resourceHead;

    while (current != NULL)
    {
        displayResource(current->resourceID);
        cout << "-------------------------\n";

        current = current->next;
    }
}

// Display available resources
void ResourceManager::displayAvailableResources() const
{
    Resource* current = resourceHead;
    bool found = false;

    while (current != NULL)
    {
        if (current->availableQuantity > 0 &&
            current->status != "Maintenance")
        {
            displayResource(current->resourceID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo resources currently available.\n";
}

// Update total quantity
bool ResourceManager::updateResourceQuantity(
    const string& resourceID,
    int newTotalQuantity)
{
    Resource* resource = searchResource(resourceID);

    if (resource == NULL || newTotalQuantity < 0)
        return false;

    int assignedQuantity =
        resource->totalQuantity - resource->availableQuantity;

    if (newTotalQuantity < assignedQuantity)
        return false;

    resource->totalQuantity = newTotalQuantity;
    resource->availableQuantity = newTotalQuantity - assignedQuantity;

    if (resource->status != "Maintenance")
    {
        resource->status =
            (resource->availableQuantity > 0) ? "Available" : "In Use";
    }

    return saveResources();
}

// Assign resource to patient
bool ResourceManager::assignResource(const ResourceAssignment& assignment)
{
    Resource* resource = searchResource(assignment.resourceID);

    if (resource == NULL ||
        assignment.patientID.empty() ||
        assignment.staffID.empty() ||
        assignment.quantity <= 0 ||
        resource->status == "Maintenance" ||
        resource->availableQuantity < assignment.quantity)
    {
        return false;
    }

    ResourceAssignment* newAssignment = new ResourceAssignment();

    *newAssignment = assignment;

    newAssignment->assignmentID = generateAssignmentID();
    newAssignment->assignmentDate = getResourceDate();
    newAssignment->returnDate = "Not Returned";
    newAssignment->status = "Assigned";
    newAssignment->next = NULL;

    resource->availableQuantity -= newAssignment->quantity;

    if (resource->availableQuantity == 0)
        resource->status = "In Use";

    appendAssignment(newAssignment);

    bool resourceSaved = saveResources();
    bool assignmentSaved = saveAssignments();

    return resourceSaved && assignmentSaved;
}

// Return assigned resource
bool ResourceManager::returnResource(const string& assignmentID)
{
    ResourceAssignment* assignment = assignmentHead;

    while (assignment != NULL)
    {
        if (assignment->assignmentID == assignmentID)
            break;

        assignment = assignment->next;
    }

    if (assignment == NULL || assignment->status != "Assigned")
        return false;

    Resource* resource = searchResource(assignment->resourceID);

    if (resource == NULL)
        return false;

    if (resource->availableQuantity + assignment->quantity >
        resource->totalQuantity)
    {
        return false;
    }

    resource->availableQuantity += assignment->quantity;

    if (resource->status != "Maintenance")
        resource->status = "Available";

    assignment->status = "Returned";
    assignment->returnDate = getResourceDate();

    bool resourceSaved = saveResources();
    bool assignmentSaved = saveAssignments();

    return resourceSaved && assignmentSaved;
}

// Display all assignments
void ResourceManager::displayAssignments() const
{
    if (assignmentHead == NULL)
    {
        cout << "\nNo resource assignments found.\n";
        return;
    }

    ResourceAssignment* current = assignmentHead;

    while (current != NULL)
    {
        cout << "\nAssignment ID: " << current->assignmentID;
        cout << "\nResource ID: " << current->resourceID;
        cout << "\nPatient ID: " << current->patientID;
        cout << "\nStaff ID: " << current->staffID;
        cout << "\nQuantity: " << current->quantity;
        cout << "\nAssignment Date: " << current->assignmentDate;
        cout << "\nReturn Date: " << current->returnDate;
        cout << "\nStatus: " << current->status;
        cout << "\n-------------------------\n";

        current = current->next;
    }
}

// Display resources assigned to a patient
void ResourceManager::displayPatientResources(const string& patientID) const
{
    ResourceAssignment* current = assignmentHead;
    bool found = false;

    while (current != NULL)
    {
        if (current->patientID == patientID)
        {
            cout << "\nAssignment ID: " << current->assignmentID;
            cout << "\nResource ID: " << current->resourceID;
            cout << "\nQuantity: " << current->quantity;
            cout << "\nStatus: " << current->status;
            cout << "\nAssignment Date: " << current->assignmentDate;
            cout << "\nReturn Date: " << current->returnDate;
            cout << "\n-------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo resource assignments found for this patient.\n";
}

// Get resource count
int ResourceManager::getResourceCount() const
{
    return resourceCount;
}

// Get assignment count
int ResourceManager::getAssignmentCount() const
{
    return assignmentCount;
}

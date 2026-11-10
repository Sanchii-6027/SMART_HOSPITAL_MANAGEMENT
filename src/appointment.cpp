
#include "../include/appointment.h"
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// Split a line using |
int splitAppointmentLine(
    const string& line,
    string fields[],
    int maxFields)
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

// Appointment constructor
Appointment::Appointment()
{
    next = NULL;
}

// AppointmentManager constructor
AppointmentManager::AppointmentManager()
{
    appointmentHead = NULL;
    appointmentTail = NULL;

    requestHead = NULL;
    requestTail = NULL;

    appointmentCount = 0;
    requestCount = 0;

    nextAppointmentNumber = 1;
    nextRequestNumber = 1;

    loadAppointments();
    loadRequests();
}

// Destructor
AppointmentManager::~AppointmentManager()
{
    saveAppointments();
    saveRequests();

    clearAppointments();
    clearRequests();
}

// Append confirmed appointment
void AppointmentManager::appendAppointment(Appointment* appointment)
{
    appointment->next = NULL;

    if (appointmentHead == NULL)
    {
        appointmentHead = appointmentTail = appointment;
    }
    else
    {
        appointmentTail->next = appointment;
        appointmentTail = appointment;
    }

    appointmentCount++;
}

// Append appointment request
void AppointmentManager::appendRequest(Appointment* request)
{
    request->next = NULL;

    if (requestHead == NULL)
    {
        requestHead = requestTail = request;
    }
    else
    {
        requestTail->next = request;
        requestTail = request;
    }

    requestCount++;
}

// Clear confirmed appointments
void AppointmentManager::clearAppointments()
{
    Appointment* current = appointmentHead;

    while (current != NULL)
    {
        Appointment* temp = current;
        current = current->next;
        delete temp;
    }

    appointmentHead = NULL;
    appointmentTail = NULL;
    appointmentCount = 0;
}

// Clear appointment requests
void AppointmentManager::clearRequests()
{
    Appointment* current = requestHead;

    while (current != NULL)
    {
        Appointment* temp = current;
        current = current->next;
        delete temp;
    }

    requestHead = NULL;
    requestTail = NULL;
    requestCount = 0;
}

// Generate appointment ID
string AppointmentManager::generateAppointmentID()
{
    string id = "APPT00000" + to_string(nextAppointmentNumber);
    nextAppointmentNumber++;

    return id;
}

// Generate request ID
string AppointmentManager::generateRequestID()
{
    string id = "REQ00000" + to_string(nextRequestNumber);
    nextRequestNumber++;

    return id;
}

// Check if doctor already has an appointment at this time
bool AppointmentManager::hasDoctorConflict(
    const string& doctorID,
    const string& date,
    const string& time,
    const string& excludedAppointmentID) const
{
    Appointment* current = appointmentHead;

    while (current != NULL)
    {
        if (current->doctorID == doctorID &&
            current->appointmentDate == date &&
            current->appointmentTime == time &&
            current->status == "Confirmed" &&
            current->appointmentID != excludedAppointmentID)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

// Save confirmed appointments
bool AppointmentManager::saveAppointments() const
{
    ofstream file("DATA/appointments.txt");

    if (!file)
        return false;

    Appointment* current = appointmentHead;

    while (current != NULL)
    {
        file << current->appointmentID << "|"
             << current->patientID << "|"
             << current->doctorID << "|"
             << current->department << "|"
             << current->appointmentDate << "|"
             << current->appointmentTime << "|"
             << current->reason << "|"
             << current->status << "\n";

        current = current->next;
    }

    file.close();
    return true;
}

// Load confirmed appointments
bool AppointmentManager::loadAppointments()
{
    ifstream file("DATA/appointments.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[8];

        if (splitAppointmentLine(line, fields, 8) != 8)
            continue;

        Appointment* appointment = new Appointment();

        appointment->appointmentID = fields[0];
        appointment->patientID = fields[1];
        appointment->doctorID = fields[2];
        appointment->department = fields[3];
        appointment->appointmentDate = fields[4];
        appointment->appointmentTime = fields[5];
        appointment->reason = fields[6];
        appointment->status = fields[7];

        if (appointment->appointmentID.length() > 4 &&
            appointment->appointmentID.substr(0, 4) == "APPT")
        {
            int number = atoi(
                appointment->appointmentID.substr(4).c_str()
            );

            if (number >= nextAppointmentNumber)
                nextAppointmentNumber = number + 1;
        }

        appendAppointment(appointment);
    }

    file.close();
    return true;
}

// Save appointment requests
bool AppointmentManager::saveRequests() const
{
    ofstream file("DATA/appointment_requests.txt");

    if (!file)
        return false;

    Appointment* current = requestHead;

    while (current != NULL)
    {
        file << current->appointmentID << "|"
             << current->patientID << "|"
             << current->doctorID << "|"
             << current->department << "|"
             << current->appointmentDate << "|"
             << current->appointmentTime << "|"
             << current->reason << "|"
             << current->status << "\n";

        current = current->next;
    }

    file.close();
    return true;
}

// Load appointment requests
bool AppointmentManager::loadRequests()
{
    ifstream file("DATA/appointment_requests.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[8];

        if (splitAppointmentLine(line, fields, 8) != 8)
            continue;

        Appointment* request = new Appointment();

        request->appointmentID = fields[0];
        request->patientID = fields[1];
        request->doctorID = fields[2];
        request->department = fields[3];
        request->appointmentDate = fields[4];
        request->appointmentTime = fields[5];
        request->reason = fields[6];
        request->status = fields[7];

        if (request->appointmentID.length() > 3 &&
            request->appointmentID.substr(0, 3) == "REQ")
        {
            int number = atoi(
                request->appointmentID.substr(3).c_str()
            );

            if (number >= nextRequestNumber)
                nextRequestNumber = number + 1;
        }

        appendRequest(request);
    }

    file.close();
    return true;
}

// Submit an appointment request
bool AppointmentManager::requestAppointment(
    const Appointment& appointment)
{
    if (appointment.patientID.empty() ||
        appointment.doctorID.empty() ||
        appointment.department.empty() ||
        appointment.appointmentDate.empty() ||
        appointment.appointmentTime.empty())
    {
        return false;
    }

    Appointment* request = new Appointment();

    *request = appointment;

    request->appointmentID = generateRequestID();
    request->status = "Pending";
    request->next = NULL;

    appendRequest(request);

    return saveRequests();
}

// Search request by ID
Appointment* AppointmentManager::searchRequest(
    const string& requestID) const
{
    Appointment* current = requestHead;

    while (current != NULL)
    {
        if (current->appointmentID == requestID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Display all appointment requests
void AppointmentManager::displayAppointmentRequests() const
{
    if (requestHead == NULL)
    {
        cout << "\nNo appointment requests found.\n";
        return;
    }

    Appointment* current = requestHead;

    while (current != NULL)
    {
        cout << "\nRequest ID: " << current->appointmentID;
        cout << "\nPatient ID: " << current->patientID;
        cout << "\nDoctor ID: " << current->doctorID;
        cout << "\nDepartment: " << current->department;
        cout << "\nDate: " << current->appointmentDate;
        cout << "\nTime: " << current->appointmentTime;
        cout << "\nReason: " << current->reason;
        cout << "\nStatus: " << current->status;
        cout << "\n-------------------------\n";

        current = current->next;
    }
}

// Display requests of a particular patient
void AppointmentManager::displayPatientRequests(
    const string& patientID) const
{
    Appointment* current = requestHead;
    bool found = false;

    while (current != NULL)
    {
        if (current->patientID == patientID)
        {
            cout << "\nRequest ID: " << current->appointmentID;
            cout << "\nDoctor ID: " << current->doctorID;
            cout << "\nDate: " << current->appointmentDate;
            cout << "\nTime: " << current->appointmentTime;
            cout << "\nStatus: " << current->status;
            cout << "\n-------------------------\n";

            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo requests found for this patient.\n";
}

// Confirm an appointment request
bool AppointmentManager::confirmAppointment(
    const string& requestID)
{
    Appointment* request = searchRequest(requestID);

    if (request == NULL || request->status != "Pending")
        return false;

    if (hasDoctorConflict(
        request->doctorID,
        request->appointmentDate,
        request->appointmentTime))
    {
        cout << "\nDoctor is already booked at this time.\n";
        return false;
    }

    Appointment* appointment = new Appointment();

    *appointment = *request;

    appointment->appointmentID = generateAppointmentID();
    appointment->status = "Confirmed";
    appointment->next = NULL;

    appendAppointment(appointment);

    request->status = "Confirmed";

    bool appointmentSaved = saveAppointments();
    bool requestSaved = saveRequests();

    return appointmentSaved && requestSaved;
}

// Reject an appointment request
bool AppointmentManager::rejectAppointment(
    const string& requestID)
{
    Appointment* request = searchRequest(requestID);

    if (request == NULL || request->status != "Pending")
        return false;

    request->status = "Rejected";

    return saveRequests();
}

// Reschedule a confirmed appointment
bool AppointmentManager::rescheduleAppointment(
    const string& appointmentID,
    const string& newDate,
    const string& newTime)
{
    Appointment* appointment = searchAppointment(appointmentID);

    if (appointment == NULL ||
        appointment->status != "Confirmed" ||
        newDate.empty() ||
        newTime.empty())
    {
        return false;
    }

    if (hasDoctorConflict(
        appointment->doctorID,
        newDate,
        newTime,
        appointmentID))
    {
        cout << "\nDoctor is already booked at the new time.\n";
        return false;
    }

    string oldDate = appointment->appointmentDate;
    string oldTime = appointment->appointmentTime;

    appointment->appointmentDate = newDate;
    appointment->appointmentTime = newTime;

    if (!saveAppointments())
    {
        appointment->appointmentDate = oldDate;
        appointment->appointmentTime = oldTime;
        return false;
    }

    return true;
}

// Cancel a confirmed appointment
bool AppointmentManager::cancelAppointment(
    const string& appointmentID)
{
    Appointment* appointment = searchAppointment(appointmentID);

    if (appointment == NULL || appointment->status != "Confirmed")
        return false;

    appointment->status = "Cancelled";

    return saveAppointments();
}

// Search confirmed appointment
Appointment* AppointmentManager::searchAppointment(
    const string& appointmentID) const
{
    Appointment* current = appointmentHead;

    while (current != NULL)
    {
        if (current->appointmentID == appointmentID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Display one appointment
void AppointmentManager::displayAppointment(
    const string& appointmentID) const
{
    Appointment* appointment = searchAppointment(appointmentID);

    if (appointment == NULL)
    {
        cout << "\nAppointment not found.\n";
        return;
    }

    cout << "\nAppointment ID: " << appointment->appointmentID;
    cout << "\nPatient ID: " << appointment->patientID;
    cout << "\nDoctor ID: " << appointment->doctorID;
    cout << "\nDepartment: " << appointment->department;
    cout << "\nDate: " << appointment->appointmentDate;
    cout << "\nTime: " << appointment->appointmentTime;
    cout << "\nReason: " << appointment->reason;
    cout << "\nStatus: " << appointment->status << "\n";
}

// Display all confirmed appointments and their history
void AppointmentManager::displayAllAppointments() const
{
    if (appointmentHead == NULL)
    {
        cout << "\nNo appointments found.\n";
        return;
    }

    Appointment* current = appointmentHead;

    while (current != NULL)
    {
        displayAppointment(current->appointmentID);
        cout << "-------------------------\n";

        current = current->next;
    }
}

// Display appointments of a patient
void AppointmentManager::displayPatientAppointments(
    const string& patientID) const
{
    Appointment* current = appointmentHead;
    bool found = false;

    while (current != NULL)
    {
        if (current->patientID == patientID)
        {
            displayAppointment(current->appointmentID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo appointments found for this patient.\n";
}

// Display appointments of a doctor
void AppointmentManager::displayDoctorAppointments(
    const string& doctorID) const
{
    Appointment* current = appointmentHead;
    bool found = false;

    while (current != NULL)
    {
        if (current->doctorID == doctorID &&
            current->status == "Confirmed")
        {
            displayAppointment(current->appointmentID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo confirmed appointments for this doctor.\n";
}

// Get appointment count
int AppointmentManager::getAppointmentCount() const
{
    return appointmentCount;
}

// Get request count
int AppointmentManager::getRequestCount() const
{
    return requestCount;
}

// Check doctor availability
bool AppointmentManager::isDoctorAvailable(
    const string& doctorID,
    const string& date,
    const string& time) const
{
    if (doctorID.empty() || date.empty() || time.empty())
        return false;

    return !hasDoctorConflict(doctorID, date, time);
}

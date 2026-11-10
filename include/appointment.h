
#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include <string>

using namespace std;

struct Appointment
{
    string appointmentID;
    string patientID;
    string doctorID;
    string department;

    string appointmentDate;
    string appointmentTime;

    string reason;
    string status;

    Appointment* next;

    Appointment();
};

class AppointmentManager
{
private:
    Appointment* appointmentHead;
    Appointment* appointmentTail;

    Appointment* requestHead;
    Appointment* requestTail;

    int appointmentCount;
    int requestCount;

    int nextAppointmentNumber;
    int nextRequestNumber;

    void appendAppointment(Appointment* appointment);
    void appendRequest(Appointment* request);

    void clearAppointments();
    void clearRequests();

    string generateAppointmentID();
    string generateRequestID();

    bool loadAppointments();
    bool saveAppointments() const;

    bool loadRequests();
    bool saveRequests() const;

    bool hasDoctorConflict(
        const string& doctorID,
        const string& date,
        const string& time,
        const string& excludedAppointmentID = ""
    ) const;

public:
    AppointmentManager();
    ~AppointmentManager();

    AppointmentManager(const AppointmentManager& other) = delete;
    AppointmentManager& operator=(const AppointmentManager& other) = delete;

    // Patient appointment requests

    bool requestAppointment(const Appointment& appointment);

    Appointment* searchRequest(const string& requestID) const;

    void displayAppointmentRequests() const;

    void displayPatientRequests(const string& patientID) const;

    // Receptionist operations

    bool confirmAppointment(const string& requestID);

    bool rejectAppointment(const string& requestID);

    bool rescheduleAppointment(
        const string& appointmentID,
        const string& newDate,
        const string& newTime
    );

    bool cancelAppointment(const string& appointmentID);

    // Appointment search and display

    Appointment* searchAppointment(const string& appointmentID) const;

    void displayAppointment(const string& appointmentID) const;

    void displayAllAppointments() const;

    void displayPatientAppointments(const string& patientID) const;

    void displayDoctorAppointments(const string& doctorID) const;

    // Information

    int getAppointmentCount() const;

    int getRequestCount() const;

    bool isDoctorAvailable(
        const string& doctorID,
        const string& date,
        const string& time
    ) const;
};

#endif

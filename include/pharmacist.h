
#ifndef PHARMACIST_H
#define PHARMACIST_H

#include <iostream>
#include <string>

using namespace std;

struct Medicine
{
    string medicineID;
    string medicineName;
    string category;

    int quantity;
    int minimumStock;

    double price;

    string expiryDate;
    string status;

    Medicine* next;

    Medicine();
};

struct Prescription
{
    string prescriptionID;
    string patientID;
    string doctorID;

    string medicineID;
    string medicineName;

    int prescribedQuantity;

    string prescriptionDate;
    string pharmacistID;

    double totalPrice;

    string status;

    Prescription* next;

    Prescription();
};

class PharmacistManager
{
private:
    Medicine* medicineHead;
    Medicine* medicineTail;

    Prescription* prescriptionHead;
    Prescription* prescriptionTail;

    int medicineCount;
    int prescriptionCount;

    int nextMedicineNumber;
    int nextPrescriptionNumber;

    void appendMedicine(Medicine* medicine);
    void appendPrescription(Prescription* prescription);

    void clearMedicines();
    void clearPrescriptions();

    string generateMedicineID();
    string generatePrescriptionID();

    bool loadMedicines();
    bool saveMedicines() const;

    bool loadPrescriptions();
    bool savePrescriptions() const;

public:
    PharmacistManager();
    ~PharmacistManager();

    PharmacistManager(const PharmacistManager& other) = delete;
    PharmacistManager& operator=(const PharmacistManager& other) = delete;

    // Medicine inventory

    bool addMedicine(const Medicine& medicine);

    Medicine* searchMedicine(const string& medicineID) const;

    void displayMedicine(const string& medicineID) const;

    void displayAllMedicines() const;

    void displayLowStockMedicines() const;

    void displayExpiredMedicines() const;

    bool updateMedicineStock(
        const string& medicineID,
        int newQuantity
    );

    // Prescription management

    bool issuePrescription(
        const Prescription& prescription
    );

    Prescription* searchPrescription(
        const string& prescriptionID
    ) const;

    void displayPrescription(
        const string& prescriptionID
    ) const;

    void displayPatientPrescriptions(
        const string& patientID
    ) const;

    int getMedicineCount() const;

    int getPrescriptionCount() const;
};

#endif

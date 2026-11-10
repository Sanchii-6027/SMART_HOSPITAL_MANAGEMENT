
#include "../include/pharmacist.h"

#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

// ---------------- MEDICINE CONSTRUCTOR ----------------

Medicine::Medicine()
{
    medicineID = "";
    medicineName = "";
    category = "";
    quantity = 0;
    minimumStock = 0;
    price = 0.0;
    expiryDate = "";
    status = "Available";
    next = NULL;
}

// ---------------- PRESCRIPTION CONSTRUCTOR ----------------

Prescription::Prescription()
{
    prescriptionID = "";
    patientID = "";
    doctorID = "";
    medicineID = "";
    medicineName = "";
    prescribedQuantity = 0;
    prescriptionDate = "";
    pharmacistID = "";
    totalPrice = 0.0;
    status = "Issued";
    next = NULL;
}

// ---------------- APPEND MEDICINE ----------------

void PharmacistManager::appendMedicine(Medicine* medicine)
{
    medicine->next = NULL;

    if (medicineHead == NULL)
    {
        medicineHead = medicine;
        medicineTail = medicine;
    }
    else
    {
        medicineTail->next = medicine;
        medicineTail = medicine;
    }

    medicineCount++;
}

// ---------------- APPEND PRESCRIPTION ----------------

void PharmacistManager::appendPrescription(Prescription* prescription)
{
    prescription->next = NULL;

    if (prescriptionHead == NULL)
    {
        prescriptionHead = prescription;
        prescriptionTail = prescription;
    }
    else
    {
        prescriptionTail->next = prescription;
        prescriptionTail = prescription;
    }

    prescriptionCount++;
}

// ---------------- CLEAR MEDICINES ----------------

void PharmacistManager::clearMedicines()
{
    Medicine* current = medicineHead;

    while (current != NULL)
    {
        Medicine* temp = current;
        current = current->next;
        delete temp;
    }

    medicineHead = NULL;
    medicineTail = NULL;
    medicineCount = 0;
}

// ---------------- CLEAR PRESCRIPTIONS ----------------

void PharmacistManager::clearPrescriptions()
{
    Prescription* current = prescriptionHead;

    while (current != NULL)
    {
        Prescription* temp = current;
        current = current->next;
        delete temp;
    }

    prescriptionHead = NULL;
    prescriptionTail = NULL;
    prescriptionCount = 0;
}

// ---------------- GENERATE MEDICINE ID ----------------

string PharmacistManager::generateMedicineID()
{
    string number = to_string(nextMedicineNumber);

    while (number.length() < 5)
    {
        number = "0" + number;
    }

    nextMedicineNumber++;

    return "MED" + number;
}

// ---------------- GENERATE PRESCRIPTION ID ----------------

string PharmacistManager::generatePrescriptionID()
{
    string number = to_string(nextPrescriptionNumber);

    while (number.length() < 5)
    {
        number = "0" + number;
    }

    nextPrescriptionNumber++;

    return "PRE" + number;
}

// ---------------- CONSTRUCTOR ----------------

PharmacistManager::PharmacistManager()
{
    medicineHead = NULL;
    medicineTail = NULL;

    prescriptionHead = NULL;
    prescriptionTail = NULL;

    medicineCount = 0;
    prescriptionCount = 0;

    nextMedicineNumber = 1;
    nextPrescriptionNumber = 1;

    loadMedicines();
    loadPrescriptions();
}

// ---------------- DESTRUCTOR ----------------

PharmacistManager::~PharmacistManager()
{
    saveMedicines();
    savePrescriptions();

    clearMedicines();
    clearPrescriptions();
}

// ---------------- SEARCH MEDICINE ----------------

Medicine* PharmacistManager::searchMedicine(
    const string& medicineID
) const
{
    Medicine* current = medicineHead;

    while (current != NULL)
    {
        if (current->medicineID == medicineID)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

// ---------------- ADD MEDICINE ----------------

bool PharmacistManager::addMedicine(const Medicine& medicine)
{
    if (medicine.medicineName == "" ||
        medicine.category == "" ||
        medicine.expiryDate == "" ||
        medicine.quantity < 0 ||
        medicine.minimumStock < 0 ||
        medicine.price < 0)
    {
        cout << "Invalid medicine details.\n";
        return false;
    }

    Medicine* current = medicineHead;

    while (current != NULL)
    {
        if (current->medicineName == medicine.medicineName)
        {
            cout << "Medicine already exists.\n";
            return false;
        }

        current = current->next;
    }

    Medicine* newMedicine = new Medicine;

    newMedicine->medicineID = generateMedicineID();
    newMedicine->medicineName = medicine.medicineName;
    newMedicine->category = medicine.category;
    newMedicine->quantity = medicine.quantity;
    newMedicine->minimumStock = medicine.minimumStock;
    newMedicine->price = medicine.price;
    newMedicine->expiryDate = medicine.expiryDate;

    if (newMedicine->quantity == 0)
    {
        newMedicine->status = "Out of Stock";
    }
    else
    {
        newMedicine->status = "Available";
    }

    appendMedicine(newMedicine);

    saveMedicines();

    cout << "\nMedicine added successfully.\n";
    cout << "Medicine ID: " << newMedicine->medicineID << endl;

    return true;
}

// ---------------- DISPLAY ONE MEDICINE ----------------

void PharmacistManager::displayMedicine(
    const string& medicineID
) const
{
    Medicine* medicine = searchMedicine(medicineID);

    if (medicine == NULL)
    {
        cout << "Medicine not found.\n";
        return;
    }

    cout << "\n---------- MEDICINE DETAILS ----------\n";

    cout << "Medicine ID: " << medicine->medicineID << endl;
    cout << "Name: " << medicine->medicineName << endl;
    cout << "Category: " << medicine->category << endl;
    cout << "Quantity: " << medicine->quantity << endl;
    cout << "Minimum Stock: " << medicine->minimumStock << endl;
    cout << "Price: " << medicine->price << endl;
    cout << "Expiry Date: " << medicine->expiryDate << endl;
    cout << "Status: " << medicine->status << endl;
}

// ---------------- DISPLAY ALL MEDICINES ----------------

void PharmacistManager::displayAllMedicines() const
{
    if (medicineHead == NULL)
    {
        cout << "No medicines available.\n";
        return;
    }

    Medicine* current = medicineHead;

    cout << "\n---------- MEDICINE INVENTORY ----------\n";

    while (current != NULL)
    {
        cout << "\nMedicine ID: " << current->medicineID;
        cout << "\nName: " << current->medicineName;
        cout << "\nQuantity: " << current->quantity;
        cout << "\nPrice: " << current->price;
        cout << "\nExpiry Date: " << current->expiryDate;
        cout << "\nStatus: " << current->status;
        cout << "\n-----------------------------\n";

        current = current->next;
    }
}

// ---------------- LOW STOCK ALERT ----------------

void PharmacistManager::displayLowStockMedicines() const
{
    Medicine* current = medicineHead;
    bool found = false;

    cout << "\n---------- LOW STOCK MEDICINES ----------\n";

    while (current != NULL)
    {
        if (current->quantity <= current->minimumStock)
        {
            cout << "\nMedicine: " << current->medicineName;
            cout << "\nAvailable Quantity: " << current->quantity;
            cout << "\nMinimum Stock: " << current->minimumStock << endl;

            found = true;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No low-stock medicines.\n";
    }
}

// ---------------- CHECK EXPIRY DATE ----------------

// Expiry dates must use YYYY-MM-DD format.

static bool isMedicineExpired(const string& expiryDate)
{
    if (expiryDate.length() != 10 ||
        expiryDate[4] != '-' ||
        expiryDate[7] != '-')
    {
        return false;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            continue;
        }

        if (expiryDate[i] < '0' || expiryDate[i] > '9')
        {
            return false;
        }
    }

    time_t currentTime = time(NULL);
    tm* currentDate = localtime(&currentTime);

    if (currentDate == NULL)
    {
        return false;
    }

    char today[11];

    strftime(today, sizeof(today), "%Y-%m-%d", currentDate);

    return expiryDate <= string(today);
}

// ---------------- DISPLAY EXPIRED MEDICINES ----------------

void PharmacistManager::displayExpiredMedicines() const
{
    Medicine* current = medicineHead;
    bool found = false;

    cout << "\n---------- EXPIRED MEDICINES ----------\n";

    while (current != NULL)
    {
        if (isMedicineExpired(current->expiryDate))
        {
            cout << "\nMedicine ID: " << current->medicineID;
            cout << "\nName: " << current->medicineName;
            cout << "\nExpiry Date: " << current->expiryDate << endl;

            found = true;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No expired medicines found.\n";
    }
}

// ---------------- UPDATE STOCK ----------------

bool PharmacistManager::updateMedicineStock(
    const string& medicineID,
    int newQuantity
)
{
    Medicine* medicine = searchMedicine(medicineID);

    if (medicine == NULL)
    {
        cout << "Medicine not found.\n";
        return false;
    }

    if (newQuantity < 0)
    {
        cout << "Quantity cannot be negative.\n";
        return false;
    }

    medicine->quantity = newQuantity;

    if (newQuantity == 0)
    {
        medicine->status = "Out of Stock";
    }
    else
    {
        medicine->status = "Available";
    }

    saveMedicines();

    cout << "Medicine stock updated successfully.\n";

    return true;
}

// ---------------- ISSUE PRESCRIPTION ----------------

bool PharmacistManager::issuePrescription(
    const Prescription& prescription
)
{
    if (prescription.patientID == "" ||
        prescription.doctorID == "" ||
        prescription.medicineID == "" ||
        prescription.pharmacistID == "" ||
        prescription.prescribedQuantity <= 0)
    {
        cout << "Invalid prescription details.\n";
        return false;
    }

    Medicine* medicine = searchMedicine(prescription.medicineID);

    if (medicine == NULL)
    {
        cout << "Medicine not found.\n";
        return false;
    }

    if (isMedicineExpired(medicine->expiryDate))
    {
        cout << "Cannot issue an expired medicine.\n";
        return false;
    }

    if (medicine->quantity < prescription.prescribedQuantity)
    {
        cout << "Insufficient medicine stock.\n";
        return false;
    }

    Prescription* newPrescription = new Prescription;

    newPrescription->prescriptionID = generatePrescriptionID();
    newPrescription->patientID = prescription.patientID;
    newPrescription->doctorID = prescription.doctorID;

    newPrescription->medicineID = medicine->medicineID;
    newPrescription->medicineName = medicine->medicineName;

    newPrescription->prescribedQuantity =
        prescription.prescribedQuantity;

    newPrescription->prescriptionDate =
        prescription.prescriptionDate;

    newPrescription->pharmacistID = prescription.pharmacistID;

    newPrescription->totalPrice =
        medicine->price * prescription.prescribedQuantity;

    newPrescription->status = "Issued";

    medicine->quantity -= prescription.prescribedQuantity;

    if (medicine->quantity == 0)
    {
        medicine->status = "Out of Stock";
    }

    appendPrescription(newPrescription);

    saveMedicines();
    savePrescriptions();

    cout << "\nMedicine issued successfully.\n";
    cout << "Prescription ID: " << newPrescription->prescriptionID << endl;
    cout << "Medicine: " << newPrescription->medicineName << endl;
    cout << "Quantity: " << newPrescription->prescribedQuantity << endl;
    cout << "Total Price: " << newPrescription->totalPrice << endl;

    return true;
}

// ---------------- SEARCH PRESCRIPTION ----------------

Prescription* PharmacistManager::searchPrescription(
    const string& prescriptionID
) const
{
    Prescription* current = prescriptionHead;

    while (current != NULL)
    {
        if (current->prescriptionID == prescriptionID)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

// ---------------- DISPLAY PRESCRIPTION ----------------

void PharmacistManager::displayPrescription(
    const string& prescriptionID
) const
{
    Prescription* prescription =
        searchPrescription(prescriptionID);

    if (prescription == NULL)
    {
        cout << "Prescription not found.\n";
        return;
    }

    cout << "\n---------- PRESCRIPTION DETAILS ----------\n";

    cout << "Prescription ID: " << prescription->prescriptionID << endl;
    cout << "Patient ID: " << prescription->patientID << endl;
    cout << "Doctor ID: " << prescription->doctorID << endl;
    cout << "Medicine ID: " << prescription->medicineID << endl;
    cout << "Medicine Name: " << prescription->medicineName << endl;
    cout << "Quantity: " << prescription->prescribedQuantity << endl;
    cout << "Date: " << prescription->prescriptionDate << endl;
    cout << "Pharmacist ID: " << prescription->pharmacistID << endl;
    cout << "Total Price: " << prescription->totalPrice << endl;
    cout << "Status: " << prescription->status << endl;
}

// ---------------- DISPLAY PATIENT PRESCRIPTIONS ----------------

void PharmacistManager::displayPatientPrescriptions(
    const string& patientID
) const
{
    Prescription* current = prescriptionHead;
    bool found = false;

    cout << "\n---------- PATIENT PRESCRIPTIONS ----------\n";

    while (current != NULL)
    {
        if (current->patientID == patientID)
        {
            displayPrescription(current->prescriptionID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
    {
        cout << "No prescriptions found for this patient.\n";
    }
}

// ---------------- COUNTS ----------------

int PharmacistManager::getMedicineCount() const
{
    return medicineCount;
}

int PharmacistManager::getPrescriptionCount() const
{
    return prescriptionCount;
}

// ---------------- LOAD MEDICINES ----------------

bool PharmacistManager::loadMedicines()
{
    ifstream file("DATA/medicines.txt");

    if (!file.is_open())
    {
        return true;
    }

    string line;

    while (getline(file, line))
    {
        if (line == "")
        {
            continue;
        }

        string fields[8];
        stringstream ss(line);

        bool valid = true;

        for (int i = 0; i < 7; i++)
        {
            if (!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }

        if (!valid || !getline(ss, fields[7]))
        {
            continue;
        }

        Medicine* medicine = new Medicine;

        medicine->medicineID = fields[0];
        medicine->medicineName = fields[1];
        medicine->category = fields[2];
        medicine->expiryDate = fields[6];
        medicine->status = fields[7];

        stringstream quantityStream(fields[3]);
        stringstream minimumStream(fields[4]);
        stringstream priceStream(fields[5]);

        if (!(quantityStream >> medicine->quantity) ||
            !(minimumStream >> medicine->minimumStock) ||
            !(priceStream >> medicine->price) ||
            medicine->quantity < 0 ||
            medicine->minimumStock < 0 ||
            medicine->price < 0 ||
            medicine->medicineID == "" ||
            medicine->medicineName == "" ||
            searchMedicine(medicine->medicineID) != NULL)
        {
            delete medicine;
            continue;
        }

        appendMedicine(medicine);

        string numberPart = medicine->medicineID.substr(3);
        stringstream idStream(numberPart);

        int idNumber = 0;

        if (idStream >> idNumber)
        {
            if (idNumber >= nextMedicineNumber)
            {
                nextMedicineNumber = idNumber + 1;
            }
        }
    }

    file.close();

    return true;
}

// ---------------- SAVE MEDICINES ----------------

bool PharmacistManager::saveMedicines() const
{
    ofstream file("DATA/medicines.txt");

    if (!file.is_open())
    {
        cout << "Unable to save medicine records.\n";
        return false;
    }

    Medicine* current = medicineHead;

    while (current != NULL)
    {
        file << current->medicineID << "|"
             << current->medicineName << "|"
             << current->category << "|"
             << current->quantity << "|"
             << current->minimumStock << "|"
             << current->price << "|"
             << current->expiryDate << "|"
             << current->status << "\n";

        current = current->next;
    }

    file.close();

    return true;
}

// ---------------- LOAD PRESCRIPTIONS ----------------

bool PharmacistManager::loadPrescriptions()
{
    ifstream file("DATA/prescriptions.txt");

    if (!file.is_open())
    {
        return true;
    }

    string line;

    while (getline(file, line))
    {
        if (line == "")
        {
            continue;
        }

        string fields[10];
        stringstream ss(line);

        bool valid = true;

        for (int i = 0; i < 9; i++)
        {
            if (!getline(ss, fields[i], '|'))
            {
                valid = false;
                break;
            }
        }

        if (!valid || !getline(ss, fields[9]))
        {
            continue;
        }

        Prescription* prescription = new Prescription;

        prescription->prescriptionID = fields[0];
        prescription->patientID = fields[1];
        prescription->doctorID = fields[2];
        prescription->medicineID = fields[3];
        prescription->medicineName = fields[4];
        prescription->prescriptionDate = fields[6];
        prescription->pharmacistID = fields[7];
        prescription->status = fields[9];

        stringstream quantityStream(fields[5]);
        stringstream priceStream(fields[8]);

        if (!(quantityStream >> prescription->prescribedQuantity) ||
            !(priceStream >> prescription->totalPrice) ||
            prescription->prescribedQuantity <= 0 ||
            prescription->totalPrice < 0 ||
            prescription->prescriptionID == "" ||
            searchPrescription(prescription->prescriptionID) != NULL)
        {
            delete prescription;
            continue;
        }

        appendPrescription(prescription);

        string numberPart = prescription->prescriptionID.substr(3);
        stringstream idStream(numberPart);

        int idNumber = 0;

        if (idStream >> idNumber)
        {
            if (idNumber >= nextPrescriptionNumber)
            {
                nextPrescriptionNumber = idNumber + 1;
            }
        }
    }

    file.close();

    return true;
}

// ---------------- SAVE PRESCRIPTIONS ----------------

bool PharmacistManager::savePrescriptions() const
{
    ofstream file("DATA/prescriptions.txt");

    if (!file.is_open())
    {
        cout << "Unable to save prescription records.\n";
        return false;
    }

    Prescription* current = prescriptionHead;

    while (current != NULL)
    {
        file << current->prescriptionID << "|"
             << current->patientID << "|"
             << current->doctorID << "|"
             << current->medicineID << "|"
             << current->medicineName << "|"
             << current->prescribedQuantity << "|"
             << current->prescriptionDate << "|"
             << current->pharmacistID << "|"
             << current->totalPrice << "|"
             << current->status << "\n";

        current = current->next;
    }

    file.close();

    return true;
}


#include "../include/billing.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Get current date
string getBillingDate()
{
    time_t now = time(0);
    tm* currentTime = localtime(&now);

    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d", currentTime);

    return string(date);
}

// Split a line using |
int splitBillingLine(
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

// BillItem constructor
BillItem::BillItem()
{
    quantity = 0;
    amount = 0.0;
    next = NULL;
}

// Payment constructor
Payment::Payment()
{
    amount = 0.0;
    next = NULL;
}

// Bill constructor
Bill::Bill()
{
    consultationCharges = 0.0;
    emergencyCharges = 0.0;
    laboratoryCharges = 0.0;
    medicineCharges = 0.0;
    otherCharges = 0.0;

    totalAmount = 0.0;
    paidAmount = 0.0;
    remainingAmount = 0.0;

    itemHead = NULL;
    itemTail = NULL;

    paymentHead = NULL;
    paymentTail = NULL;

    next = NULL;
}

// BillingManager constructor
BillingManager::BillingManager()
{
    head = NULL;
    tail = NULL;

    billCount = 0;
    nextBillNumber = 1;
    nextPaymentNumber = 1;

    loadBills();
    loadBillItems();
    loadPayments();

    Bill* current = head;

    while (current != NULL)
    {
        calculateBill(current);
        updateBillStatus(current);

        current = current->next;
    }
}

// Destructor
BillingManager::~BillingManager()
{
    saveBills();
    saveBillItems();
    savePayments();

    clearBills();
}

// Append bill
void BillingManager::appendBill(Bill* bill)
{
    bill->next = NULL;

    if (head == NULL)
    {
        head = tail = bill;
    }
    else
    {
        tail->next = bill;
        tail = bill;
    }

    billCount++;
}

// Clear bill items
void BillingManager::clearBillItems(Bill* bill)
{
    if (bill == NULL)
        return;

    BillItem* current = bill->itemHead;

    while (current != NULL)
    {
        BillItem* temp = current;
        current = current->next;
        delete temp;
    }

    bill->itemHead = NULL;
    bill->itemTail = NULL;
}

// Clear payments
void BillingManager::clearPayments(Bill* bill)
{
    if (bill == NULL)
        return;

    Payment* current = bill->paymentHead;

    while (current != NULL)
    {
        Payment* temp = current;
        current = current->next;
        delete temp;
    }

    bill->paymentHead = NULL;
    bill->paymentTail = NULL;
}

// Clear all bills and their linked lists
void BillingManager::clearBills()
{
    Bill* current = head;

    while (current != NULL)
    {
        Bill* temp = current;
        current = current->next;

        clearBillItems(temp);
        clearPayments(temp);

        delete temp;
    }

    head = NULL;
    tail = NULL;
    billCount = 0;
}

// Generate bill ID
string BillingManager::generateBillID()
{
    string id = "BILL00000" + to_string(nextBillNumber);
    nextBillNumber++;

    return id;
}

// Generate payment ID
string BillingManager::generatePaymentID()
{
    string id = "PAY00000" + to_string(nextPaymentNumber);
    nextPaymentNumber++;

    return id;
}

// Calculate total bill
void BillingManager::calculateBill(Bill* bill)
{
    if (bill == NULL)
        return;

    double itemTotal = 0.0;

    BillItem* item = bill->itemHead;

    while (item != NULL)
    {
        itemTotal += item->amount;
        item = item->next;
    }

    bill->totalAmount =
        bill->consultationCharges +
        bill->emergencyCharges +
        bill->laboratoryCharges +
        bill->medicineCharges +
        bill->otherCharges +
        itemTotal;

    bill->remainingAmount =
        bill->totalAmount - bill->paidAmount;

    if (bill->remainingAmount < 0)
        bill->remainingAmount = 0;
}

// Update payment status
void BillingManager::updateBillStatus(Bill* bill)
{
    if (bill == NULL)
        return;

    if (bill->paidAmount <= 0)
        bill->status = "Unpaid";
    else if (bill->paidAmount >= bill->totalAmount)
        bill->status = "Paid";
    else
        bill->status = "Partially Paid";
}

// Save bills
bool BillingManager::saveBills() const
{
    ofstream file("DATA/billing.txt");

    if (!file)
        return false;

    Bill* current = head;

    while (current != NULL)
    {
        file << current->billID << "|"
             << current->patientID << "|"
             << current->consultationCharges << "|"
             << current->emergencyCharges << "|"
             << current->laboratoryCharges << "|"
             << current->medicineCharges << "|"
             << current->otherCharges << "|"
             << current->totalAmount << "|"
             << current->paidAmount << "|"
             << current->remainingAmount << "|"
             << current->status << "|"
             << current->billDate << "\n";

        current = current->next;
    }

    file.close();
    return true;
}

// Load bills
bool BillingManager::loadBills()
{
    ifstream file("DATA/billing.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[12];

        if (splitBillingLine(line, fields, 12) != 12)
            continue;

        Bill* bill = new Bill();

        bill->billID = fields[0];
        bill->patientID = fields[1];

        stringstream values;

        values.str(fields[2]);
        if (!(values >> bill->consultationCharges))
        {
            delete bill;
            continue;
        }

        values.clear();
        values.str(fields[3]);
        if (!(values >> bill->emergencyCharges))
        {
            delete bill;
            continue;
        }

        values.clear();
        values.str(fields[4]);
        if (!(values >> bill->laboratoryCharges))
        {
            delete bill;
            continue;
        }

        values.clear();
        values.str(fields[5]);
        if (!(values >> bill->medicineCharges))
        {
            delete bill;
            continue;
        }

        values.clear();
        values.str(fields[6]);
        if (!(values >> bill->otherCharges))
        {
            delete bill;
            continue;
        }

        values.clear();
        values.str(fields[8]);
        if (!(values >> bill->paidAmount))
        {
            delete bill;
            continue;
        }

        if (bill->consultationCharges < 0 ||
            bill->emergencyCharges < 0 ||
            bill->laboratoryCharges < 0 ||
            bill->medicineCharges < 0 ||
            bill->otherCharges < 0 ||
            bill->paidAmount < 0)
        {
            delete bill;
            continue;
        }

        bill->status = fields[10];
        bill->billDate = fields[11];

        if (bill->billID.length() > 4 &&
            bill->billID.substr(0, 4) == "BILL")
        {
            int number = atoi(bill->billID.substr(4).c_str());

            if (number >= nextBillNumber)
                nextBillNumber = number + 1;
        }

        appendBill(bill);
    }

    file.close();
    return true;
}

// Save bill items
bool BillingManager::saveBillItems() const
{
    ofstream file("DATA/bill_items.txt");

    if (!file)
        return false;

    Bill* currentBill = head;

    while (currentBill != NULL)
    {
        BillItem* currentItem = currentBill->itemHead;

        while (currentItem != NULL)
        {
            file << currentBill->billID << "|"
                 << currentItem->itemName << "|"
                 << currentItem->category << "|"
                 << currentItem->quantity << "|"
                 << currentItem->amount << "\n";

            currentItem = currentItem->next;
        }

        currentBill = currentBill->next;
    }

    file.close();
    return true;
}

// Load bill items
bool BillingManager::loadBillItems()
{
    ifstream file("DATA/bill_items.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[5];

        if (splitBillingLine(line, fields, 5) != 5)
            continue;

        Bill* bill = searchBill(fields[0]);

        if (bill == NULL)
            continue;

        BillItem* item = new BillItem();

        item->itemName = fields[1];
        item->category = fields[2];

        stringstream quantityStream(fields[3]);
        stringstream amountStream(fields[4]);

        if (!(quantityStream >> item->quantity) ||
            !(amountStream >> item->amount) ||
            item->quantity <= 0 ||
            item->amount < 0)
        {
            delete item;
            continue;
        }

        if (bill->itemHead == NULL)
        {
            bill->itemHead = bill->itemTail = item;
        }
        else
        {
            bill->itemTail->next = item;
            bill->itemTail = item;
        }
    }

    file.close();
    return true;
}

// Save payment history
bool BillingManager::savePayments() const
{
    ofstream file("DATA/payments.txt");

    if (!file)
        return false;

    Bill* currentBill = head;

    while (currentBill != NULL)
    {
        Payment* currentPayment = currentBill->paymentHead;

        while (currentPayment != NULL)
        {
            file << currentBill->billID << "|"
                 << currentPayment->paymentID << "|"
                 << currentPayment->amount << "|"
                 << currentPayment->paymentDate << "|"
                 << currentPayment->paymentMode << "\n";

            currentPayment = currentPayment->next;
        }

        currentBill = currentBill->next;
    }

    file.close();
    return true;
}

// Load payment history
bool BillingManager::loadPayments()
{
    ifstream file("DATA/payments.txt");

    if (!file)
        return true;

    string line;

    while (getline(file, line))
    {
        string fields[5];

        if (splitBillingLine(line, fields, 5) != 5)
            continue;

        Bill* bill = searchBill(fields[0]);

        if (bill == NULL)
            continue;

        Payment* payment = new Payment();

        payment->paymentID = fields[1];

        stringstream amountStream(fields[2]);

        if (!(amountStream >> payment->amount) ||
            payment->amount <= 0)
        {
            delete payment;
            continue;
        }

        payment->paymentDate = fields[3];
        payment->paymentMode = fields[4];

        if (payment->paymentID.length() > 3 &&
            payment->paymentID.substr(0, 3) == "PAY")
        {
            int number = atoi(payment->paymentID.substr(3).c_str());

            if (number >= nextPaymentNumber)
                nextPaymentNumber = number + 1;
        }

        if (bill->paymentHead == NULL)
        {
            bill->paymentHead = bill->paymentTail = payment;
        }
        else
        {
            bill->paymentTail->next = payment;
            bill->paymentTail = payment;
        }
    }

    file.close();
    return true;
}

// Create a new bill
bool BillingManager::createBill(
    const string& patientID,
    double consultationCharges,
    double emergencyCharges,
    double laboratoryCharges,
    double medicineCharges,
    double otherCharges)
{
    if (patientID.empty() ||
        consultationCharges < 0 ||
        emergencyCharges < 0 ||
        laboratoryCharges < 0 ||
        medicineCharges < 0 ||
        otherCharges < 0)
    {
        return false;
    }

    Bill* bill = new Bill();

    bill->billID = generateBillID();
    bill->patientID = patientID;

    bill->consultationCharges = consultationCharges;
    bill->emergencyCharges = emergencyCharges;
    bill->laboratoryCharges = laboratoryCharges;
    bill->medicineCharges = medicineCharges;
    bill->otherCharges = otherCharges;

    bill->billDate = getBillingDate();

    calculateBill(bill);
    updateBillStatus(bill);

    appendBill(bill);

    return saveBills();
}

// Add an item to a bill
bool BillingManager::addBillItem(
    const string& billID,
    const string& itemName,
    const string& category,
    int quantity,
    double amount)
{
    Bill* bill = searchBill(billID);

    if (bill == NULL ||
        itemName.empty() ||
        category.empty() ||
        quantity <= 0 ||
        amount < 0)
    {
        return false;
    }

    BillItem* item = new BillItem();

    item->itemName = itemName;
    item->category = category;
    item->quantity = quantity;
    item->amount = amount;

    if (bill->itemHead == NULL)
    {
        bill->itemHead = bill->itemTail = item;
    }
    else
    {
        bill->itemTail->next = item;
        bill->itemTail = item;
    }

    calculateBill(bill);
    updateBillStatus(bill);

    bool itemsSaved = saveBillItems();
    bool billsSaved = saveBills();

    return itemsSaved && billsSaved;
}

// Search bill by ID
Bill* BillingManager::searchBill(const string& billID) const
{
    Bill* current = head;

    while (current != NULL)
    {
        if (current->billID == billID)
            return current;

        current = current->next;
    }

    return NULL;
}

// Find latest bill for a patient
Bill* BillingManager::searchLatestPatientBill(
    const string& patientID) const
{
    Bill* current = head;
    Bill* latest = NULL;

    while (current != NULL)
    {
        if (current->patientID == patientID)
            latest = current;

        current = current->next;
    }

    return latest;
}

// Make a full or partial payment
bool BillingManager::makePayment(
    const string& billID,
    double amount,
    const string& paymentMode)
{
    Bill* bill = searchBill(billID);

    if (bill == NULL ||
        amount <= 0 ||
        paymentMode.empty())
    {
        return false;
    }

    calculateBill(bill);

    if (amount > bill->remainingAmount)
    {
        cout << "\nPayment exceeds the remaining balance.\n";
        return false;
    }

    Payment* payment = new Payment();

    payment->paymentID = generatePaymentID();
    payment->amount = amount;
    payment->paymentDate = getBillingDate();
    payment->paymentMode = paymentMode;

    if (bill->paymentHead == NULL)
    {
        bill->paymentHead = bill->paymentTail = payment;
    }
    else
    {
        bill->paymentTail->next = payment;
        bill->paymentTail = payment;
    }

    bill->paidAmount += amount;

    calculateBill(bill);
    updateBillStatus(bill);

    bool paymentsSaved = savePayments();
    bool billsSaved = saveBills();

    return paymentsSaved && billsSaved;
}

// Display one bill
void BillingManager::displayBill(const string& billID) const
{
    Bill* bill = searchBill(billID);

    if (bill == NULL)
    {
        cout << "\nBill not found.\n";
        return;
    }

    cout << "\nBill ID: " << bill->billID;
    cout << "\nPatient ID: " << bill->patientID;
    cout << "\nBill Date: " << bill->billDate;
    cout << "\nTotal Amount: Rs. " << bill->totalAmount;
    cout << "\nPaid Amount: Rs. " << bill->paidAmount;
    cout << "\nRemaining Amount: Rs. " << bill->remainingAmount;
    cout << "\nStatus: " << bill->status << "\n";
}

// Display all bills
void BillingManager::displayAllBills() const
{
    if (head == NULL)
    {
        cout << "\nNo bills found.\n";
        return;
    }

    Bill* current = head;

    while (current != NULL)
    {
        displayBill(current->billID);
        cout << "-------------------------\n";

        current = current->next;
    }
}

// Display bills of a patient
void BillingManager::displayPatientBills(
    const string& patientID) const
{
    Bill* current = head;
    bool found = false;

    while (current != NULL)
    {
        if (current->patientID == patientID)
        {
            displayBill(current->billID);
            found = true;
        }

        current = current->next;
    }

    if (!found)
        cout << "\nNo bills found for this patient.\n";
}

// Display item-wise bill breakdown
void BillingManager::displayBillBreakdown(
    const string& billID) const
{
    Bill* bill = searchBill(billID);

    if (bill == NULL)
    {
        cout << "\nBill not found.\n";
        return;
    }

    cout << "\n----- BILL BREAKDOWN -----\n";

    cout << "Consultation: Rs. "
         << bill->consultationCharges << "\n";

    cout << "Emergency: Rs. "
         << bill->emergencyCharges << "\n";

    cout << "Laboratory: Rs. "
         << bill->laboratoryCharges << "\n";

    cout << "Medicines: Rs. "
         << bill->medicineCharges << "\n";

    cout << "Other Charges: Rs. "
         << bill->otherCharges << "\n";

    cout << "\nAdditional Items:\n";

    BillItem* item = bill->itemHead;

    while (item != NULL)
    {
        cout << item->itemName
             << " (" << item->category << ")"
             << " | Quantity: " << item->quantity
             << " | Amount: Rs. " << item->amount << "\n";

        item = item->next;
    }

    cout << "\nTotal: Rs. " << bill->totalAmount;
    cout << "\nPaid: Rs. " << bill->paidAmount;
    cout << "\nRemaining: Rs. " << bill->remainingAmount;
    cout << "\nStatus: " << bill->status << "\n";
}

// Display payment history
void BillingManager::displayPaymentHistory(
    const string& billID) const
{
    Bill* bill = searchBill(billID);

    if (bill == NULL)
    {
        cout << "\nBill not found.\n";
        return;
    }

    Payment* current = bill->paymentHead;

    if (current == NULL)
    {
        cout << "\nNo payments recorded.\n";
        return;
    }

    cout << "\n----- PAYMENT HISTORY -----\n";

    while (current != NULL)
    {
        cout << "\nPayment ID: " << current->paymentID;
        cout << "\nAmount: Rs. " << current->amount;
        cout << "\nDate: " << current->paymentDate;
        cout << "\nMode: " << current->paymentMode;
        cout << "\n-------------------------\n";

        current = current->next;
    }
}

// Calculate total revenue
double BillingManager::getTotalRevenue() const
{
    double revenue = 0.0;

    Bill* current = head;

    while (current != NULL)
    {
        revenue += current->paidAmount;
        current = current->next;
    }

    return revenue;
}

// Calculate total pending amount
double BillingManager::getPendingAmount() const
{
    double pending = 0.0;

    Bill* current = head;

    while (current != NULL)
    {
        pending += current->remainingAmount;
        current = current->next;
    }

    return pending;
}

// Get total number of bills
int BillingManager::getBillCount() const
{
    return billCount;
}

// Get outstanding amount of one patient
double BillingManager::getPatientOutstandingAmount(
    const string& patientID) const
{
    double amount = 0.0;

    Bill* current = head;

    while (current != NULL)
    {
        if (current->patientID == patientID)
            amount += current->remainingAmount;

        current = current->next;
    }

    return amount;
}

// Check whether patient has pending payments
bool BillingManager::hasPendingPayment(
    const string& patientID) const
{
    Bill* current = head;

    while (current != NULL)
    {
        if (current->patientID == patientID &&
            current->remainingAmount > 0)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

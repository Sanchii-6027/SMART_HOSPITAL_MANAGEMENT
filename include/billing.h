
#ifndef BILLING_H
#define BILLING_H

#include <iostream>
#include <string>

using namespace std;

struct BillItem
{
    string itemName;
    string category;

    int quantity;
    double amount;

    BillItem* next;

    BillItem();
};

struct Payment
{
    string paymentID;
    double amount;
    string paymentDate;
    string paymentMode;

    Payment* next;

    Payment();
};

struct Bill
{
    string billID;
    string patientID;

    double consultationCharges;
    double emergencyCharges;
    double laboratoryCharges;
    double medicineCharges;
    double otherCharges;

    double totalAmount;
    double paidAmount;
    double remainingAmount;

    string status;
    string billDate;

    BillItem* itemHead;
    BillItem* itemTail;

    Payment* paymentHead;
    Payment* paymentTail;

    Bill* next;

    Bill();
};

class BillingManager
{
private:
    Bill* head;
    Bill* tail;

    int billCount;
    int nextBillNumber;
    int nextPaymentNumber;

    void appendBill(Bill* bill);

    void clearBills();

    void clearBillItems(Bill* bill);

    void clearPayments(Bill* bill);

    string generateBillID();

    string generatePaymentID();

    bool loadBills();

    bool saveBills() const;

    bool loadBillItems();

    bool saveBillItems() const;

    bool loadPayments();

    bool savePayments() const;

    void calculateBill(Bill* bill);

    void updateBillStatus(Bill* bill);

public:
    BillingManager();

    ~BillingManager();

    BillingManager(const BillingManager& other) = delete;

    BillingManager& operator=(const BillingManager& other) = delete;

    // Bill creation

    bool createBill(
        const string& patientID,
        double consultationCharges,
        double emergencyCharges,
        double laboratoryCharges,
        double medicineCharges,
        double otherCharges
    );

    // Add individual bill items

    bool addBillItem(
        const string& billID,
        const string& itemName,
        const string& category,
        int quantity,
        double amount
    );

    // Search bills

    Bill* searchBill(const string& billID) const;

    Bill* searchLatestPatientBill(const string& patientID) const;

    // Payment management

    bool makePayment(
        const string& billID,
        double amount,
        const string& paymentMode
    );

    // Display bills

    void displayBill(const string& billID) const;

    void displayAllBills() const;

    void displayPatientBills(const string& patientID) const;

    void displayBillBreakdown(const string& billID) const;

    void displayPaymentHistory(const string& billID) const;

    // Billing information

    double getTotalRevenue() const;

    double getPendingAmount() const;

    int getBillCount() const;

    double getPatientOutstandingAmount(
        const string& patientID
    ) const;

    bool hasPendingPayment(const string& patientID) const;
};

#endif

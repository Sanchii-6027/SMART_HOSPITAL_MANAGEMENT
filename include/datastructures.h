#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "patient.h"
#include <iostream>
#include <string>

using namespace std;

// Hash table node
struct HashNode
{
    string key;
    Patient* patient;
    HashNode* next;

    HashNode(const string& searchKey, Patient* patientAddress);
};

// Hash table using Separate Chaining
class HashTable
{
private:
    static const int TABLE_SIZE = 101;

    HashNode* buckets[TABLE_SIZE];

    // Generate hash index
    int hashFunction(const string& key) const;

public:
    // Constructor and destructor
    HashTable();
    ~HashTable();

    // Prevent accidental shallow copying
    HashTable(const HashTable& other) = delete;
    HashTable& operator=(const HashTable& other) = delete;

    // Insert or update a patient reference
    bool insert(const string& key, Patient* patient);

    // Search patient using key
    Patient* search(const string& key) const;

    // Remove a key without deleting the patient
    bool remove(const string& key);

    // Remove all hash table nodes
    void clear();

    // Display hash table information
    void display() const;

    // Count stored keys
    int getCount() const;
};

#endif
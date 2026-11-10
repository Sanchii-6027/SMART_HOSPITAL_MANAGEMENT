#include "dataStructures.h"

// ================= HASH NODE =================

HashNode::HashNode(const string& searchKey, Patient* patientAddress)
{
    key = searchKey;
    patient = patientAddress;
    next = NULL;
}

// ================= HASH TABLE CONSTRUCTOR =================

HashTable::HashTable()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        buckets[i] = NULL;
    }
}

// ================= HASH TABLE DESTRUCTOR =================

HashTable::~HashTable()
{
    clear();
}

// ================= HASH FUNCTION =================

int HashTable::hashFunction(const string& key) const
{
    unsigned long hash = 0;

    for (int i = 0; i < (int)key.length(); i++)
    {
        hash = (hash * 31 + key[i]) % TABLE_SIZE;
    }

    return (int)hash;
}

// ================= INSERT =================

bool HashTable::insert(const string& key, Patient* patient)
{
    if (key.empty() || patient == NULL)
    {
        return false;
    }

    int index = hashFunction(key);

    HashNode* current = buckets[index];

    // Update existing key
    while (current != NULL)
    {
        if (current->key == key)
        {
            current->patient = patient;
            return true;
        }

        current = current->next;
    }

    // Insert new node at the beginning
    HashNode* newNode = new HashNode(key, patient);

    newNode->next = buckets[index];
    buckets[index] = newNode;

    return true;
}

// ================= SEARCH =================

Patient* HashTable::search(const string& key) const
{
    if (key.empty())
    {
        return NULL;
    }

    int index = hashFunction(key);

    HashNode* current = buckets[index];

    while (current != NULL)
    {
        if (current->key == key)
        {
            return current->patient;
        }

        current = current->next;
    }

    return NULL;
}

// ================= REMOVE =================

bool HashTable::remove(const string& key)
{
    if (key.empty())
    {
        return false;
    }

    int index = hashFunction(key);

    HashNode* current = buckets[index];
    HashNode* previous = NULL;

    while (current != NULL)
    {
        if (current->key == key)
        {
            if (previous == NULL)
            {
                buckets[index] = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            // Delete only the hash node, not the patient
            delete current;

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

// ================= CLEAR =================

void HashTable::clear()
{
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        HashNode* current = buckets[i];

        while (current != NULL)
        {
            HashNode* temp = current;
            current = current->next;

            delete temp;
        }

        buckets[i] = NULL;
    }
}

// ================= DISPLAY =================

void HashTable::display() const
{
    cout << "\n========== HASH TABLE ==========\n";

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        if (buckets[i] != NULL)
        {
            cout << "\nBucket " << i << ": ";

            HashNode* current = buckets[i];

            while (current != NULL)
            {
                cout << "[" << current->key << "]";

                if (current->next != NULL)
                {
                    cout << " -> ";
                }

                current = current->next;
            }
        }
    }

    cout << "\n================================\n";
}

// ================= COUNT =================

int HashTable::getCount() const
{
    int count = 0;

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        HashNode* current = buckets[i];

        while (current != NULL)
        {
            count++;
            current = current->next;
        }
    }

    return count;
}
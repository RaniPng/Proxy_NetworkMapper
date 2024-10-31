#pragma once
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashTable HashTable;
typedef void (*HashTableCallback)(const char *key, void *value);

// Function to create a new hash table
HashTable *createHashTable(size_t initialSize);

// Function to insert a key-value pair into the hash table
void hashTableInsert(HashTable *ht, const char *key, void *value);

// Function to search for a value by key in the hash table
void *hashTableSearch(HashTable *ht, const char *key);

// Function to delete a key-value pair from the hash table
void hashTableDelete(HashTable *ht, const char *key);

// loop over
void hashTableForEach(HashTable *ht, HashTableCallback callback);

// Function to free the hash table
void freeHashTable(HashTable *ht);

#include <HashTable.h>

#define INITIAL_SIZE 8
#define LOAD_FACTOR 0.75

void example()
{
    HashTable *ht = createHashTable(INITIAL_SIZE);

    hashTableInsert(ht, "name", "Alice");
    hashTableInsert(ht, "age", (void *)(long)30);

    printf("Name: %s\n", (char *)hashTableSearch(ht, "name"));
    printf("Age: %d\n", (int)(long)hashTableSearch(ht, "age"));

    hashTableDelete(ht, "name");
    printf("Name after deletion: %s\n", (char *)hashTableSearch(ht, "name"));

    // hashTableForEach(ht, printKeyValue); printKeyValue = ptr* void printKeyValue(const char *key, void *value)

    // for each keys
    // for (size_t i = 0; i < ht->size; i++)
    // {
    //     Entry *entry = ht->table[i];
    //     while (entry)
    //     {
    //         Entry *temp = entry;
    //         entry = entry->next;
    //     }
    // }

    freeHashTable(ht);
}

typedef struct Entry
{
    char *key;
    void *value;
    struct Entry *next;
} Entry;

struct HashTable
{
    Entry **table;
    size_t size;
    size_t count;
};

static unsigned long hashFunction(const char *key)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

static void resizeHashTable(HashTable *ht)
{
    size_t oldSize = ht->size;
    size_t newSize = oldSize * 2;
    Entry **newTable = calloc(newSize, sizeof(Entry *));

    for (size_t i = 0; i < oldSize; i++)
    {
        Entry *entry = ht->table[i];
        while (entry)
        {
            Entry *next = entry->next;

            size_t index = hashFunction(entry->key) % newSize;
            entry->next = newTable[index];
            newTable[index] = entry;

            entry = next;
        }
    }

    free(ht->table);
    ht->table = newTable;
    ht->size = newSize;
}

HashTable *createHashTable(size_t initialSize)
{
    HashTable *ht = malloc(sizeof(HashTable));
    ht->size = initialSize;
    ht->count = 0;
    ht->table = calloc(initialSize, sizeof(Entry *));
    return ht;
}

void hashTableInsert(HashTable *ht, const char *key, void *value)
{
    if ((float)ht->count / ht->size > LOAD_FACTOR)
    {
        resizeHashTable(ht);
    }

    size_t index = hashFunction(key) % ht->size;
    Entry *entry = ht->table[index];

    while (entry)
    {
        if (strcmp(entry->key, key) == 0)
        {
            entry->value = value; // Update value if key already exists
            return;
        }
        entry = entry->next;
    }

    Entry *newEntry = malloc(sizeof(Entry));
    newEntry->key = strdup(key);
    newEntry->value = value;
    newEntry->next = ht->table[index];
    ht->table[index] = newEntry;
    ht->count++;
}

void *hashTableSearch(HashTable *ht, const char *key)
{
    size_t index = hashFunction(key) % ht->size;
    Entry *entry = ht->table[index];

    while (entry)
    {
        if (strcmp(entry->key, key) == 0)
        {
            return entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void hashTableDelete(HashTable *ht, const char *key)
{
    size_t index = hashFunction(key) % ht->size;
    Entry *entry = ht->table[index];
    Entry *prev = NULL;

    while (entry)
    {
        if (strcmp(entry->key, key) == 0)
        {
            if (prev)
            {
                prev->next = entry->next;
            }
            else
            {
                ht->table[index] = entry->next;
            }
            free(entry->key);
            free(entry);
            ht->count--;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void hashTableForEach(HashTable *ht, HashTableCallback callback)
{
    for (size_t i = 0; i < ht->size; i++)
    {
        Entry *entry = ht->table[i];
        while (entry)
        {
            callback(entry->key, entry->value);
            entry = entry->next;
        }
    }
}

void freeHashTable(HashTable *ht)
{
    for (size_t i = 0; i < ht->size; i++)
    {
        Entry *entry = ht->table[i];
        while (entry)
        {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}
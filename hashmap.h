#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_SIZE 128
#define TOMBSTONE ((void*) -1)

typedef struct hashentry {
    char* key;
    void* value;
} HashEntry;

typedef struct hashmap {
    int size;
    HashEntry* table;
} HashMap;
// Q 1.1
unsigned long simple_hash(const char* str);
// Q1.2
HashMap* hashmap_create();
// Q1.3
int hashmap_insert(HashMap* map, const char* key, void* value);
// Q1.4
void* hashmap_get(HashMap* map, const char* key);
// Q1.5
int hashmap_remove(HashMap* map, const char* key);
//Q1.6
void hashmap_destroy(HashMap* map);


#endif // HASHMAP_H

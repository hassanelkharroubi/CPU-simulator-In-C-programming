#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

// Q 1.1
unsigned long simple_hash(const char* str) {
    unsigned long hash = 5381;

   for (int i = 0; str[i] != '\0'; i++) {
        hash += str[i]; 
    }
    return hash % TABLE_SIZE;
}

// Q 1.2
HashMap* hashmap_create() {
    HashMap* map = malloc(sizeof(HashMap));
    if (!map)
    {
        printf("No data allocation has been allowed for HashMap.\n");
        return NULL;

    } 

    map->size = TABLE_SIZE;
    map->table = calloc(TABLE_SIZE, sizeof(HashEntry));
    if (!map->table) {
        printf("No data allocation has been allowed for table.\n");
        free(map);
        return NULL;
    }

    return map;
}

// Q1.3
int hashmap_insert(HashMap* map, const char* key, void* value) {
    unsigned long index = simple_hash(key);
    for (int i = 0; i < map->size; i++) {
        int pos = (index + i) % map->size;
        HashEntry* entry = &map->table[pos];

        if (entry->value == NULL || entry->value == TOMBSTONE) {
            entry->key = strdup(key);
            entry->value = value;
            return 1;
        }

        //check the collision
        if (entry->key && strcmp(entry->key, key) == 0 && entry->value==value) {
            printf("Data already exist.\n");
                return 0;
        }
    }
    return 0;
}
// Q 1.4
void* hashmap_get(HashMap* map, const char* key) {
    unsigned long index = simple_hash(key);
    for (int i = 0; i < map->size; i++) {
        int pos = (index + i) % map->size;
        HashEntry* entry = &map->table[pos];

        if (entry->value == NULL) {
            return NULL;
        }
        if (entry->value != TOMBSTONE && strcmp(entry->key, key) == 0) {
            return entry->value;
        }
    }
    return NULL;
}
// Q 1.5
int hashmap_remove(HashMap* map, const char* key) {
    unsigned long index = simple_hash(key);
    for (int i = 0; i < map->size; i++) {
        int pos = (index + i) % map->size;
        HashEntry* entry = &map->table[pos];

        if (entry->value == NULL) return 0;
        if (entry->value != TOMBSTONE && strcmp(entry->key, key) == 0) {
            free(entry->key);
            entry->key = NULL;
            entry->value = TOMBSTONE;
            return 1;
        }
    }
    return 0;
}

void hashmap_destroy(HashMap* map) {
    for (int i = 0; i < map->size; i++) {
        if (map->table[i].value != NULL && map->table[i].value != TOMBSTONE) {
            free(map->table[i].key);
        }
    }
    free(map->table);
    free(map);
}

#ifndef MEMORY_HANDLER_H
#define MEMORY_HANDLER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct segment {
    int start;        // Position de début (adresse) du segment dans la mémoire
    int size;         // Taille du segment en unités de mémoire
    struct segment* next; // Pointeur vers le segment suivant dans la liste chaînée
} Segment;

typedef struct memoryHandler {
    void** memory;      // Tableau de pointeurs vers la mémoire allouée
    int total_size;     // Taille totale de la mémoire gérée
    Segment* free_list; // Liste chaînée des segments de mémoire libres
    void* allocated;    // Table de hachage (nom, segment)
} MemoryHandler;

// functions
//Q 2.1
MemoryHandler* memory_init(int size);
//Q 2.2
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev);
//Q 2.3
int create_segment(MemoryHandler* handler, const char* name, int start, int size);
//Q 2.4
int remove_segment(MemoryHandler* handler, const char* name);

#endif // MEMORY_HANDLER_H

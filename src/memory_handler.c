#include "memory_handler.h"
#include "hashmap.h"
#include <assert.h>

// Q 2.1
MemoryHandler* memory_init(int size) {
    MemoryHandler* handler = malloc(sizeof(MemoryHandler));
    if (!handler) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    handler->total_size = size;
    handler->memory = calloc(size, sizeof(void*)); //Memory allocation
    if (!handler->memory) {
        free(handler);
        printf("Memory allocation for memory array failed.\n");
        return NULL;
    }

    // Initialisation of free segment list
    handler->free_list = malloc(sizeof(Segment));
    if (!handler->free_list) {
        free(handler->memory);
        free(handler);
        printf("Memory allocation for free list failed.\n");
        return NULL;
    }

    handler->free_list->start = 0;
    handler->free_list->size = size;
    handler->free_list->next = NULL;

    handler->allocated =hashmap_create();

    return handler;
}

// Q 2.2
Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev) {
    Segment* curr = handler->free_list;
    *prev = curr;

    while (curr != NULL) {
        if (curr->start <= start && (curr->start + curr->size) >= (start + size)) {
            return curr; // Segment free is found
        }
        *prev = curr;
        curr = curr->next;
    }

    return NULL; // no free segment found
}

// Q 2.3
int create_segment(MemoryHandler* handler, const char* name, int start, int size) {
    Segment* prev = NULL;
    Segment* free_seg = find_free_segment(handler, start, size, &prev);
    if (!free_seg) {
        printf("No free segment available.\n");
        return 0;
    }

    // Create the new allocated segment
    Segment* new_seg = malloc(sizeof(Segment));
    
    if (!new_seg) {
        printf("Memory allocation failed.\n");
        return 0;
    }
    new_seg->start = start;
    new_seg->size = size;
    new_seg->next = NULL;
    assert(hashmap_insert(handler->allocated, name, new_seg));
    // Split the free segment into parts (before and after)
    int before_size = start - free_seg->start;
    int after_size = (free_seg->start + free_seg->size) - (start + size);
// todo : check this
    Segment* after_seg = NULL;
    if (after_size > 0) {
        after_seg = malloc(sizeof(Segment));
        after_seg->start = start + size;
        after_seg->size = after_size;
        after_seg->next = free_seg->next;
    }

    if (before_size > 0) {
        free_seg->size = before_size;
        free_seg->next = after_seg;
    } else {
        // remove free_seg from list
        if (prev == free_seg) {
            handler->free_list = after_seg;
        } else {
            prev->next = after_seg;
        }
        free(free_seg);
    }

    return 1;
}


// Q2.4
int remove_segment(MemoryHandler* handler, const char* name) {
    // Find and remove from hashmap
    Segment* segment = hashmap_get(handler->allocated, name);
    if (!segment) {
        printf("Segment not found.\n");
        return 0;
    }
    hashmap_remove(handler->allocated, name);

    Segment* prev = NULL;
    Segment* curr = handler->free_list;

    // Find insertion point in sorted free list
    while (curr && curr->start < segment->start) {
        prev = curr;
        curr = curr->next;
    }

    // Try to merge with next
    if (curr && segment->start + segment->size == curr->start) {
        segment->size += curr->size;
        segment->next = curr->next;
        free(curr);
    } else {
        segment->next = curr;
    }

    // Try to merge with prev
    if (prev && prev->start + prev->size == segment->start) {
        prev->size += segment->size;
        prev->next = segment->next;
        free(segment);
    } else {
        if (prev) {
            prev->next = segment;
        } else {
            handler->free_list = segment;
        }
    }

    return 1;
}


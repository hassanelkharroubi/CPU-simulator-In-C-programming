#include "cpu.h"
#include <limits.h>

// Q 8.1
// [SEG:REG] => load from segment SEG at address stored in REG
void* segment_override_addressing(CPU *cpu, const char *operand) {
    // Regex for “[XX:YY]” where XX and YY are two uppercase letters
    const char *pattern = "^\\[([A-Z]{2}):([A-Z]{2})\\]$";
    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) return NULL;

    regmatch_t m[3];
    int ok = regexec(&regex, operand, 3, m, 0);
    regfree(&regex);
    if (ok) return NULL;

    // Extract segment name
    char seg[3] = { operand[m[1].rm_so], operand[m[1].rm_so + 1], '\0' };
    // Extract register name
    char reg[3] = { operand[m[2].rm_so], operand[m[2].rm_so + 1], '\0' };

    // Get register value = base address
    int *reg_val = hashmap_get(cpu->context, reg);
    if (!reg_val) return NULL;
    int address = *reg_val;

    // Load from specified segment
    Segment *seg_obj = hashmap_get(cpu->memory_handler->allocated, seg);
    if (!seg_obj) return NULL;

    // Check bounds
    if (address < 0 || address >= seg_obj->size) return NULL;

    return load(cpu->memory_handler, seg, address);
}


// Return start address of a free block of >= size, by strategy:
// 0=First Fit, 1=Best Fit, 2=Worst Fit
int find_free_address_strategy(MemoryHandler *handler, int size, int strategy) {
    Segment *curr = handler->free_list;
    int best_addr = -1;
    int best_size = (strategy==2 ? 0 : INT_MAX);

    while (curr) {
        if (curr->size >= size) {
            if (strategy == 0) {
                return curr->start;
            }
            if (strategy == 1 && curr->size < best_size) {
                best_size = curr->size;
                best_addr = curr->start;
            }
            if (strategy == 2 && curr->size > best_size) {
                best_size = curr->size;
                best_addr = curr->start;
            }
        }
        curr = curr->next;
    }
    return (strategy == 0 ? -1 : best_addr);
}


// Q 8.5
// Allocate ES segment based on AX=size, BX=strategy
int alloc_es_segment(CPU *cpu) {
    int *ax = hashmap_get(cpu->context, "AX");
    int *bx = hashmap_get(cpu->context, "BX");
    int *zf = hashmap_get(cpu->context, "ZF");
    int *es = hashmap_get(cpu->context, "ES");

    int size     = *ax;
    int strategy = *bx;

    int addr = find_free_address_strategy(cpu->memory_handler, size, strategy);
    if (addr < 0) {
        *zf = 1;  // failure
        return -1;
    }

    // Create ES segment and zero it
    assert(create_segment(cpu->memory_handler, "ES", addr, size));
    for (int i = 0; i < size; i++) {
        int *zero = malloc(sizeof(int));
        *zero = 0;
        store(cpu->memory_handler, "ES", i, zero);
    }

    *es = addr;
    *zf = 0;  // success
    return addr;
}
// Q 8.6
// Free ES segment
int free_es_segment(CPU *cpu) {
    int *es = hashmap_get(cpu->context, "ES");
    if (!es || *es < 0) {
        return -1;
    }

    // Remove ES segment: free each cell, then remove segment
    Segment *seg = hashmap_get(cpu->memory_handler->allocated, "ES");
    if (!seg) return -1;

    for (int i = 0; i < seg->size; i++) {
        int *val = load(cpu->memory_handler, "ES", i);
        free(val);
    }

    // Remove and merge
    remove_segment(cpu->memory_handler, "ES");

    *es = -1;
    return 0;
}

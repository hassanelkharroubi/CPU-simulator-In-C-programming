
#include "cpu.h"
#include <stdio.h>

// Q 7.2
//  Push an integer onto the stack
int push_value(CPU *cpu, int value) {
    int *sp = hashmap_get(cpu->context, "SP");
    Segment *ss = hashmap_get(cpu->memory_handler->allocated, "SS");
    if (!ss || *sp < 0) {
        printf("Stack overflow!\n");
        return -1;
    }

    int *ptr = malloc(sizeof(int));
    *ptr = value;
    store(cpu->memory_handler, "SS", *sp, ptr);
    (*sp)--;  // Stack grows downward
    return 0;
}

// Q 7.2
// Pop an integer from the stack
int pop_value(CPU *cpu, int *dest) {
    int *sp = hashmap_get(cpu->context, "SP");
    Segment *ss = hashmap_get(cpu->memory_handler->allocated, "SS");
    if (!ss || *sp >= ss->size - 1) {
        printf("Stack underflow!\n");
        return -1;
    }

    (*sp)++;  // Move back up
    int *val = load(cpu->memory_handler, "SS", *sp);
    if (val) *dest = *val;
    return 0;
}


#ifndef EXTRA_SEGMENT_H
#define EXTRA_SEGMENT_H
#include "cpu.h"  
#include <assert.h>
void* segment_override_addressing(CPU *cpu, const char *operand);
int find_free_address_strategy(MemoryHandler *handler, int size, int strategy);
int alloc_es_segment(CPU *cpu);
int free_es_segment(CPU *cpu);

#endif // EXTRA_SEGMENT_H

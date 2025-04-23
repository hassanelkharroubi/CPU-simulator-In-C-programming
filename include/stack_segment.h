#ifndef STACK_SEGMENT_H
#define STACK_SEGMENT_H
#include "cpu.h"


int push_value(CPU *cpu, int value);
int pop_value(CPU *cpu, int *dest);
#endif // STACK_SEGMENT_H
#include "parser_result.h"
#include "memory_handler.h"
#include <regex.h>
#include <stdbool.h>
#include "extra_segment.h"
typedef struct {
MemoryHandler * memory_handler ; // Gestionnaire de memoire
HashMap * context ; // Registres (AX, BX, CX, DX)
HashMap * constant_pool ; // Table de hachage pour stocker les valeurs i m m d i a t e s

} CPU ;


CPU *cpu_init(int memory_size);
void cpu_destroy(CPU *cpu);
void* store(MemoryHandler *handler, const char *segment_name, int pos, void *data) ;
void* load(MemoryHandler *handler, const char *segment_name,int pos);
void allocate_variables(CPU *cpu, Instruction** data_instructions, int data_count);
void print_data_segment(CPU *cpu) ;

// EX5
int matches(const char *pattern, const char *string) ;
void* immediate_addressing(CPU* cpu, const char* operand);
void* register_addressing(CPU* cpu, const char* operand);
void* register_indirect_addressing(CPU* cpu, const char* operand);
void* memory_direct_addressing(CPU* cpu, const char* operand);
void handle_MOV(CPU* cpu, void* src, void* dest);
void *resolve_addressing(CPU *cpu, const char *operand);
CPU *setup_test_environment();


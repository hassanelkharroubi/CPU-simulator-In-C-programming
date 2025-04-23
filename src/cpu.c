#include "cpu.h"
#include <assert.h>

CPU* cpu_init(int memory_size) {
    CPU *cpu = malloc(sizeof(CPU));
    if (!cpu) {
        printf("Error: CPU allocation failed\n");
        return NULL;
    }

    cpu->memory_handler = memory_init(memory_size);
    cpu->context        = hashmap_create();
    cpu->constant_pool  = hashmap_create();

    // General-purpose + special registers
    const char *regs[] = {
        "AX","BX","CX","DX",
        "IP","ZF","SF",
        "SP","BP","ES"
    };
    for (int i = 0; i < 10; i++) {
        int *val = malloc(sizeof(int));
        *val = (strcmp(regs[i], "ES")==0 ? -1 : 0);  // ES starts at -1
        hashmap_insert(cpu->context, regs[i], val);
    }

    // Stack segment "SS" (128 units at top of memory)
    int stack_size  = 128;
    int stack_start = memory_size - stack_size;
    create_segment(cpu->memory_handler, "SS", stack_start, stack_size);

    // Initialize SP and BP to top of stack (relative)
    int *sp = hashmap_get(cpu->context, "SP");
    int *bp = hashmap_get(cpu->context, "BP");
    *sp = stack_size - 1;
    *bp = stack_size - 1;

    return cpu;
}




void cpu_destroy(CPU *cpu)
{
    if (!cpu)
        return;

    hashmap_destroy(cpu->context);
    hashmap_destroy(cpu->constant_pool);
    free(cpu->memory_handler->memory);

    Segment *curr = cpu->memory_handler->free_list;
    while (curr)
    {
        Segment *next = curr->next;
        free(curr);
        curr = next;
    }

    free(cpu->memory_handler);
    free(cpu);
}

void *store(MemoryHandler *handler, const char *segment_name, int pos, void *data)
{

    Segment *segment = (Segment *)hashmap_get((HashMap *)handler->allocated, segment_name);
    assert(segment);
    
    if (!segment || pos < 0 || pos >= segment->size)
    {
        printf("Error: segment not found or position invalid.\n");
        return NULL;
    }

    int mem_index = segment->start + pos;
    handler->memory[mem_index] = data;

    return handler->memory[mem_index];
}

void *load(MemoryHandler *handler, const char *segment_name, int pos)
{
    if (!handler || !segment_name)
        return NULL;

    Segment *segment = (Segment *)hashmap_get(handler->allocated, segment_name);
    if (!segment)
    {
        printf("Error : segment '%s' not found.\n", segment_name);
        return NULL;
    }

    if (pos < 0 || pos >= segment->size)
    {
        printf("Error : position %d out of range of segment '%s'.\n", pos, segment_name);
        return NULL;
    }

    int real_index = segment->start + pos;
    return handler->memory[real_index];
}

void allocate_variables(CPU *cpu, Instruction **data_instructions, int data_count)
{
    if (!cpu || !data_instructions)
        return;

    int total_size = 1;
    for (int i = 0; i < data_count; i++)
    {
        Instruction *instr = data_instructions[i];
        

        if (strchr(instr->operand2, ',') != NULL)
        {
            // It's an array-like declaration
            
            int count = 1;
            for (char *p = instr->operand2; *p != '\0'; p++)
            {
                if (*p == ',')
                    count++;
            }
            total_size += count;
        }
        else
        {
            total_size += 1;
        }
    }


    // 2. "DS" segment creation 
    // TODO! check for existing segment
    if (create_segment(cpu->memory_handler, "DS", 0, total_size) == 0)
    {
        printf("Error : impossible to create a segment DS.\n");
        return;
    }



    // 3. fill the segment
    int offset = 0;
    for (int i = 0; i < data_count; i++)
    {
        Instruction *instr = data_instructions[i];

        if (strchr(instr->operand2, ',') != NULL)
        {
            // It's an array (comma-separated values)
            char *values = strdup(instr->operand2); // Duplicate to safely tokenize
            char *token = strtok(values, ",");
            while (token != NULL)
            {
                int *value = malloc(sizeof(int));
                *value = atoi(token); // Convert token to int
                store(cpu->memory_handler, "DS", offset, value);
                cpu->memory_handler->memory[i]=value;
                offset++;
                token = strtok(NULL, ",");
            }
            
            free(values);
        }
        else
        {
            // It's a single variable
            int *value = malloc(sizeof(int));
            *value = atoi(instr->operand2);
            cpu->memory_handler->memory[i]=value;
            store(cpu->memory_handler, "DS", offset, value);
            offset++;
        }
    }
}

void print_data_segment(CPU *cpu)
{
    if (!cpu)
        return;

    Segment *segment = (Segment *)hashmap_get(cpu->memory_handler->allocated, "DS");
    if (!segment)
    {
        printf("Segment 'DS' not found.\n");
        return;
    }

    printf("Content of DS segment :\n");
    for (int i = 0; i < segment->size; i++)
    {
        int mem_index = segment->start + i;
        int *val_ptr = (int *)cpu->memory_handler->memory[mem_index];

        if (val_ptr)
            printf("Addresse %d (pos %d): %d\n", mem_index, i, *val_ptr);
        else
            printf("Addresse %d (pos %d): NULL\n", mem_index, i);
    }
}


int matches(const char *pattern, const char *string) {
    regex_t regex;
    int result = regcomp(&regex, pattern, REG_EXTENDED);
    if (result) {
        fprintf(stderr, "Regex compilation failed for pattern: %s\n", pattern);
        return 0;
    }
    result = regexec(&regex, string, 0, NULL, 0);
    regfree(&regex);
    return result == 0;
}

// Q 5.2

void* immediate_addressing(CPU* cpu, const char* operand) {
    if (!matches("^[0-9]+$", operand)) return NULL;

    void* existing = hashmap_get(cpu->constant_pool, operand);
    if (existing) return existing;

    int* value = malloc(sizeof(int));
    *value = atoi(operand);
    hashmap_insert(cpu->constant_pool, operand, value);
    return value;
}

//Q 5.3

void* register_addressing(CPU* cpu, const char* operand) {
    if (!matches("^(AX|BX|CX|DX)$", operand)) return NULL;
    return hashmap_get(cpu->context, operand);
}

//Q 5.4 

void* memory_direct_addressing(CPU* cpu, const char* operand) {
    if (!matches("^\[[0-9]+\]$", operand)) return NULL;

    char copy[strlen(operand) - 1];
    strncpy(copy, operand + 1, strlen(operand) - 2);
    copy[strlen(operand) - 2] = '\0';

    int index = atoi(copy);
    if (index < 0 || index >= cpu->memory_handler->total_size) return NULL;
   return load(cpu->memory_handler, "DS", index);
}

// Q 5.5

void* register_indirect_addressing(CPU* cpu, const char* operand) {
    if (!matches("^\[(AX|BX|CX|DX)\]$", operand)) return NULL;

    char reg[3];
    strncpy(reg, operand + 1, 2);
    reg[2] = '\0';

    int* reg_value = hashmap_get(cpu->context, reg);
    if (!reg_value) return NULL;

    int address = *reg_value;
    if (address < 0 || address >= cpu->memory_handler->total_size) return NULL;
    return load(cpu->memory_handler, "DS", address);
}

void handle_MOV(CPU* cpu, void* src, void* dest) {
    if (!src || !dest) return;
    *((int*)dest) = *((int*)src);
}
// Q 5/7

CPU *setup_test_environment() {
    // Initialiser le CPU
    CPU *cpu = cpu_init(1024);
    if (!cpu) {
        printf("Error: CPU initialization failed\n");
        return NULL;
    }

    // Initialiser les registres avec des valeurs spécifiques
    int *ax = (int *) hashmap_get(cpu->context, "AX");
    int *bx = (int *) hashmap_get(cpu->context, "BX");
    int *cx = (int *) hashmap_get(cpu->context, "CX");
    int *dx = (int *) hashmap_get(cpu->context, "DX");

    *ax = 3;
    *bx = 6;
    *cx = 100;
    *dx = 0;

    // Créer et initialiser le segment de données
    if (!hashmap_get(cpu->memory_handler->allocated, "DS")) {
        create_segment(cpu->memory_handler, "DS", 0, 20);

        // Initialiser le segment de données avec des valeurs de test
        for (int i = 0; i < 10; i++) {
            int *value = (int *) malloc(sizeof(int));
            *value = i * 10 + 5; // Valeurs : 5, 15, 25, ...
            store(cpu->memory_handler, "DS", i, value);
        }
    }

    printf("Test environment initialized.\n");
    return cpu;
}



// Q 5.8
// Attempts each addressing mode in turn and returns the first non-NULL result
void* resolve_addressing(CPU *cpu, const char *operand) {
    void *result;

    // 1) Explicit segment override: [SEG:REG]
    result = segment_override_addressing(cpu, operand);
    if (result) return result;

    // 2) Immediate literal (e.g. "42")
    result = immediate_addressing(cpu, operand);
    if (result) return result;

    // 3) Register direct (e.g. "AX")
    result = register_addressing(cpu, operand);
    if (result) return result;

    // 4) Memory direct (e.g. "[5]")
    result = memory_direct_addressing(cpu, operand);
    if (result) return result;

    // 5) Register indirect (e.g. "[AX]")
    result = register_indirect_addressing(cpu, operand);
    if (result) return result;

     // uknown
    printf("Erreur : mode d'adressage non reconnu pour l'opérande '%s'\n", operand);
    return NULL;

}


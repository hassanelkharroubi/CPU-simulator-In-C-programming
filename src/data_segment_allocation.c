#include "data_segment_allocation.h"
#include <assert.h>

CPU *cpu_init(int memory_size)
{
    CPU *cpu = malloc(sizeof(CPU));
    if (!cpu)
        return NULL;

    cpu->memory_handler = memory_init(memory_size);
    cpu->context = hashmap_create();

    hashmap_insert(cpu->context, "AX", 0);
    hashmap_insert(cpu->context, "BX", 0);
    hashmap_insert(cpu->context, "CX", 0);
    hashmap_insert(cpu->context, "DX", 0);

    return cpu;
}

void cpu_destroy(CPU *cpu)
{
    if (!cpu)
        return;

    hashmap_destroy(cpu->context);
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

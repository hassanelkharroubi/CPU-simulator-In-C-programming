#include "code_segment.h"
#include "cpu.h"

char *trim(char *str) {
    while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r') str++;

    char *end = str + strlen(str) - 1;
    while (end > str && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
        *end = '\0';
        end--;
    }

    return str;
}

int search_and_replace(char **str, HashMap *values) {
    if (!str || !*str || !values) return 0;

    int replaced = 0;
    char *input = *str;

    // Iterate through all keys in the hashmap
    for (int i = 0; i < values->size; i++) {
        if (values->table[i].key && values->table[i].key != (void *)-1) {
            char *key = values->table[i].key;
            int value = (int)(long)values->table[i].value;

            // Find potential substring match
            char *substr = strstr(input, key);
            if (substr) {
                // Construct replacement buffer
                char replacement[64];
                snprintf(replacement, sizeof(replacement), "%d", value);

                // Calculate lengths
                int key_len = strlen(key);
                int repl_len = strlen(replacement);
                int remain_len = strlen(substr + key_len);

                // Create new string
                char *new_str = (char *)malloc(strlen(input) - key_len + repl_len + 1);
                strncpy(new_str, input, substr - input);
                new_str[substr - input] = '\0';
                strcat(new_str, replacement);
                strcat(new_str, substr + key_len);

                // Free and update original string
                free(input);
                *str = new_str;
                input = new_str;

                replaced = 1;
            }
        }
    }

    // Trim the final string
    if (replaced) {
        char *trimmed = trim(input);
        if (trimmed != input) {
            memmove(input, trimmed, strlen(trimmed) + 1);
        }
    }

    return replaced;
}


// Replace labels and variable names by numeric values in the code section
int resolve_constants(ParserResult *result) {
    if (!result || !result->code_instructions) return 0;

    int replaced = 0;

    for (int i = 0; i < result->code_count; i++) {
        Instruction *instr = result->code_instructions[i];

        replaced |= search_and_replace(&instr->operand1, result->memory_locations);
        replaced |= search_and_replace(&instr->operand1, result->labels);

        replaced |= search_and_replace(&instr->operand2, result->memory_locations);
        replaced |= search_and_replace(&instr->operand2, result->labels);
    }

    return replaced;
}

// Q 6.3
// Create "CS" segment and store code instructions
void allocate_code_segment(CPU *cpu, Instruction **code_instructions, int code_count) {
    create_segment(cpu->memory_handler, "CS", 0, code_count);

    for (int i = 0; i < code_count; i++) {
        store(cpu->memory_handler, "CS", i, code_instructions[i]);
    }

    int *ip = malloc(sizeof(int));
    *ip = 0;
    hashmap_insert(cpu->context, "IP", ip);
}

//Q 6.4
// Execute the logic of one instruction
int handle_instruction(CPU *cpu, Instruction *instr, void *src, void *dest) {
    int *zf = hashmap_get(cpu->context, "ZF");
    int *sf = hashmap_get(cpu->context, "SF");
    int *ip = hashmap_get(cpu->context, "IP");

    // MOV dest, src
    if (strcmp(instr->mnemonic, "MOV") == 0) {
        if (src && dest) *(int *)dest = *(int *)src;
    }

    // ADD dest, src
    else if (strcmp(instr->mnemonic, "ADD") == 0) {
        if (src && dest) *(int *)dest += *(int *)src;
    }

    // CMP dest, src — Updates ZF and SF
    else if (strcmp(instr->mnemonic, "CMP") == 0) {
        if (src && dest) {
            int result = *(int *)dest - *(int *)src;
            *zf = (result == 0);
            *sf = (result < 0);
        }
    }

    // JMP addr — Unconditional jump
    else if (strcmp(instr->mnemonic, "JMP") == 0) {
        if (instr->operand1) *ip = atoi(instr->operand1);
        return 1; // force jump
    }

    // JZ addr — Jump if zero flag
    else if (strcmp(instr->mnemonic, "JZ") == 0) {
        if (*zf == 1 && instr->operand1) {
            *ip = atoi(instr->operand1);
            return 1;
        }
    }

    // JNZ addr — Jump if not zero flag
    else if (strcmp(instr->mnemonic, "JNZ") == 0) {
        if (*zf == 0 && instr->operand1) {
            *ip = atoi(instr->operand1);
            return 1;
        }
    }

    // HALT — Stop program
    else if (strcmp(instr->mnemonic, "HALT") == 0) {
        *ip = cpu->memory_handler->total_size; // force termination
        return 1;
    }

    // PUSH src — push value onto stack
    else if (strcmp(instr->mnemonic, "PUSH") == 0) {
        void *val = resolve_addressing(cpu, instr->operand1 ? instr->operand1 : "AX");
        if (val) push_value(cpu, *(int *)val);
    }

    // POP dest — pop value from stack into register/memory
    else if (strcmp(instr->mnemonic, "POP") == 0) {
        int popped = 0;
        if (pop_value(cpu, &popped) == 0) {
            void *target = resolve_addressing(cpu, instr->operand1 ? instr->operand1 : "AX");
            if (target) *(int *)target = popped;
        }
    }

    return 0;
}



//Q 6.5
//  Execute one instruction after resolving operands
int execute_instruction(CPU *cpu, Instruction *instr) {
    void *src = resolve_addressing(cpu, instr->operand2);
    void *dst = resolve_addressing(cpu, instr->operand1);
    return handle_instruction(cpu, instr, src, dst);
}

// Q 6.6

//  Load next instruction based on IP
Instruction *fetch_next_instruction(CPU *cpu) {
    int *ip = hashmap_get(cpu->context, "IP");
    if (!ip) return NULL;

    Segment *cs = hashmap_get(cpu->memory_handler->allocated, "CS");
    if (!cs || *ip >= cs->size) return NULL;

    Instruction *instr = load(cpu->memory_handler, "CS", *ip);
    (*ip)++;
    return instr;
}

// Q 6.7
//  Instruction execution loop
int run_program(CPU *cpu) {
    printf("Starting program execution...\n");

    while (1) {
        Instruction *instr = fetch_next_instruction(cpu);
        if (!instr) break;

        printf("Executing: %s %s %s\n",
               instr->mnemonic,
               instr->operand1 ? instr->operand1 : "",
               instr->operand2 ? instr->operand2 : "");

        int jumped = execute_instruction(cpu, instr);

        if (!jumped) {
            int *ip = hashmap_get(cpu->context, "IP");
            if (*ip >= cpu->memory_handler->total_size) break;
        }
    }

    printf("Program finished.\n");
    return 0;
}

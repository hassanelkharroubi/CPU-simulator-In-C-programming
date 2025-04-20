#include "parser_result.h"
#define INITIAL_CAPACITY 8

ParserResult *parse(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    ParserResult *result = calloc(1, sizeof(ParserResult));
    // create hash map for labels
    result->labels = hashmap_create();
    // create hash map for memory locations
    result->memory_locations = hashmap_create();
    result->data_instructions = malloc(INITIAL_CAPACITY * sizeof(Instruction *));
    result->code_instructions = malloc(INITIAL_CAPACITY * sizeof(Instruction *));
    result->code_count=result->data_count=0;
    
    int data_capacity = INITIAL_CAPACITY;
    int code_capacity = INITIAL_CAPACITY;

    char line[256];
    enum { NONE, DATA, CODE } section = NONE;

    while (fgets(line, sizeof(line), file)) {
        // delete break line
        line[strcspn(line, "\r\n")] = 0;

        // ignore empty line
        char *trimmed = line;
        while (isspace(*trimmed)) trimmed++;
        if (*trimmed == '\0') continue;

        // section detection
        if (strcasecmp(trimmed, ".DATA") == 0) {
            section = DATA;
            continue;
        } else if (strcasecmp(trimmed, ".CODE") == 0) {
            section = CODE;
            continue;
        }

        if (section == DATA) {
            if (result->data_count >= data_capacity) {
                data_capacity *= 2;
                result->data_instructions = realloc(result->data_instructions, data_capacity * sizeof(Instruction *));
            }
            Instruction *instr = parse_data_instruction(trimmed, result->memory_locations);
            if (instr) {
                result->data_instructions[result->data_count++] = instr;
            }
        } else if (section == CODE) {
            if (result->code_count >= code_capacity) {
                code_capacity *= 2;
                result->code_instructions = realloc(result->code_instructions, code_capacity * sizeof(Instruction *));
            }
            Instruction *instr = parse_code_instruction(trimmed, result->labels, result->code_count);
            if (instr) {
                result->code_instructions[result->code_count++] = instr;
            }
        }
    }

    fclose(file);
    return result;
}


void free_parser_result(ParserResult *result) {
    if (!result) return;

    // free .DATA
    for (int i = 0; i < result->data_count; i++) {
        Instruction *instr = result->data_instructions[i];
        if (instr) {
            free(instr->mnemonic);
            free(instr->operand1);
            free(instr->operand2);
            free(instr);
        }
    }
    free(result->data_instructions);

    // free .CODE
    for (int i = 0; i < result->code_count; i++) {
        Instruction *instr = result->code_instructions[i];
        if (instr) {
            free(instr->mnemonic);
            free(instr->operand1);
            free(instr->operand2);
            free(instr);
        }
    }
    free(result->code_instructions);

    // free hash table
    hashmap_destroy(result->labels);
    hashmap_destroy(result->memory_locations);

    // free result
    free(result);
}

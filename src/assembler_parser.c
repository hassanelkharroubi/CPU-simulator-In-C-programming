#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assembler_parser.h"
#include "hashmap.h"
#include <assert.h>
#include <ctype.h>

char* tim_v1(char* str) {
    // Skip leading spaces
    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0') return str;  // all spaces

    // Remove trailing spaces
    char* end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    return str;
}

Instruction *parse_data_instruction(const char *line, HashMap *memory_locations) {
    static int current_address = 0;

    char *copy = strdup(line);
    char *token = strtok(copy, " ");
    assert(token);

    // create instruction
    Instruction *instr = malloc(sizeof(Instruction));
    instr->mnemonic = strdup(token); // variable name

    token = strtok(NULL, " ");
    assert(token);
    instr->operand1 = strdup(token); // type (DW ou DB)

    // reste of data is operand2
    char *value_start = strstr(line, token) + strlen(token);
    while (*value_start == ' ') value_start++; // skip spaces
    instr->operand2 = strdup(value_start); // value like "5", ou "5,6,7,8"

    // count how many element seperated by comma
    int count = 1;
    for (char *p = instr->operand2; *p; ++p) {
        if (*p == ',') count++;
    }

    // insert into hash map 
    hashmap_insert(memory_locations, instr->mnemonic, (void *)(long)current_address);

    // Mise à jour de l'adresse actuelle
    current_address += count;


    free(copy);
    return instr;
}

Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count) {
    char *copy = strdup(line);
    Instruction *instr = calloc(1,sizeof(Instruction));
    // Pointer to track our position in the string
    char *cursor = copy;
    
    // Skip leading whitespace
    while (isspace(*cursor)) cursor++;
    
    // Check for label (contains ':')
    char *colon = strchr(cursor, ':');
    if (colon != NULL) {
        *colon = '\0';
        while (isspace(*cursor)) cursor++;
        hashmap_insert(labels,strdup(tim_v1(cursor)),(void *)(long)code_count);
        // Skip past the label and colon
        cursor = colon + 1;
        // Skip whitespace after colon
        while (isspace(*cursor)) cursor++;
    }
    
    // Now parse the instruction part
    char *mnemonic_end = cursor;
    while (*mnemonic_end && !isspace(*mnemonic_end)) mnemonic_end++;
    if (mnemonic_end > cursor) {
        instr->mnemonic = strndup(cursor, mnemonic_end - cursor);
        cursor = mnemonic_end;
    }
    
    // Skip to operands
    while (isspace(*cursor)) cursor++;
    
    // Find comma separating operands
    char *comma = strchr(cursor, ',');
    if (comma != NULL) {
        // Extract first operand (before comma)
        char *op1_end = comma;
        while (op1_end > cursor && isspace(*(op1_end-1))) op1_end--;
        if (op1_end > cursor) {
            instr->operand1 = tim_v1(strndup(cursor, op1_end - cursor));
        }
        
        // Extract second operand (after comma)
        cursor = comma + 1;
        while (isspace(*cursor)) cursor++;
        if (*cursor) {
            instr->operand2 = strdup(tim_v1(cursor));
        }
    } else if (*cursor) {
        // Single operand case
        instr->operand1 = strdup(tim_v1(cursor));
    }
    free(copy);
    return instr;
}

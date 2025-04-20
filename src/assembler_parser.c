#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assembler_parser.h"
#include "hashmap.h"
#include <assert.h>
#include <ctype.h>


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
    printf("\n current_address before %d \n",current_address);
    hashmap_insert(memory_locations, instr->mnemonic, (void *)(long)current_address);

    // Mise à jour de l'adresse actuelle
    current_address += count;
    printf("\n current_address after %d and count %d\n",current_address, count);

    free(copy);
    return instr;
}

Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count) {
    // Create a modifiable copy of the input line
    char *copy = strdup(line);
    Instruction *instr = malloc(sizeof(Instruction));
    assert(instr != NULL);
    instr->mnemonic = NULL;
    instr->operand1 = NULL;
    instr->operand2 = NULL;

    // 1. Handle label (if present) - look for ':' character
    char *label_end = strchr(copy, ':');
    if (label_end != NULL) {
        *label_end = '\0';  // Terminate the label string
        char *label = copy;
        
        // Trim whitespace from the label
        while (isspace(*label)) label++;
        char *end = label + strlen(label) - 1;
        while (end > label && isspace(*end)) *end-- = '\0';
        
        // Store label in hashmap if not empty
        if (*label != '\0') {
            hashmap_insert(labels, strdup(label), (void*)(long)code_count);
        }
        copy = label_end + 1;  // Move past the ':'
    }

    // 2. Extract mnemonic 
    char *token = strtok(copy, " ");  // Split on spaces
    if (token == NULL) {
        free(copy);
        free(instr);
        return NULL;  // Empty line after label
    }
    instr->mnemonic = strdup(token);

    // 3. Handle operands
    token = strtok(NULL, ",");  // Get first operand (split on comma)
    if (token != NULL) {
        // Trim whitespace from first operand
        char *op = token;
        while (isspace(*op)) op++;
        char *end = op + strlen(op) - 1;
        while (end > op && isspace(*end)) *end-- = '\0';
        instr->operand1 = strdup(op);

        // Get second operand
        token = strtok(NULL, "");  // Get the rest of the string
        if (token != NULL) {
            while (isspace(*token)) token++;
            end = token + strlen(token) - 1;
            while (end > token && isspace(*end)) *end-- = '\0';
            if (*token != '\0') {
                instr->operand2 = strdup(token);
            }
        }
    }

    free(copy);
    return instr;
}

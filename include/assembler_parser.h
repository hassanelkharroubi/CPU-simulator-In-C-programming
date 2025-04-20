#include "hashmap.h"
typedef struct {
    char *mnemonic;  // Instruction mnemonic (ou nom de variable pour .DATA
    char *operand1;  // Premier operande (ou type pour .DATA)
    char *operand2;   // Second operande (ou initialisation pour .DATA)
} Instruction;

typedef struct {
    Instruction **data_instructions; // Tableau d’instructions .DATA
    int data_count; // Nombre d’instructions .DATA

    Instruction **code_instructions; // Tableau d’instructions .CODE
    int code_count; // Nombre d’instructions .CODE
    HashMap *labels;            // labels -> indices dans code_instructions
    HashMap *memory_locations;  // noms de variables -> adresse memoire
} ParserResult;

// Q 3.1
Instruction *parse_data_instruction(const char *line, HashMap *memory_locations);
// Q3.2
Instruction *parse_code_instruction(const char *line, HashMap *labels, int code_count);
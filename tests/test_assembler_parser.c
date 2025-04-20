#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "assembler_parser.h"
#include "hashmap.h"

// Helper function to print an Instruction
void print_instruction(Instruction *instr) {
    printf("Mnemonic: %s\n", instr->mnemonic);
    printf("Operand1: %s\n", instr->operand1);
    printf("Operand2: %s\n\n", instr->operand2);
}

// Test function
void test_parse_data_instruction() {
    HashMap *memory_locations = hashmap_create();

    Instruction *i1 = parse_data_instruction("X DW 3", memory_locations);

    Instruction *i2 = parse_data_instruction("Y DB 4", memory_locations);

    Instruction *i3 = parse_data_instruction("arr DB 5,6,7,8", memory_locations);

    Instruction *i4 = parse_data_instruction("z DB 9", memory_locations);


    printf("Parsed Instructions:\n");
    print_instruction(i1);
    print_instruction(i2);
    print_instruction(i3);
    print_instruction(i4);
    printf("\n\t\u2705 parsed Instruction  test passed 🥳 !\n");

    printf("Memory Locations:\n");
    printf("X   -> %ld\n", (long)hashmap_get(memory_locations, "X"));
    printf("Y   -> %ld\n", (long)hashmap_get(memory_locations, "Y"));
    printf("arr -> %ld\n", (long)hashmap_get(memory_locations, "arr"));
    printf("z   -> %ld\n", (long)hashmap_get(memory_locations, "z"));
    printf("\n\t\u2705 Memory Locations  test passed 🥳 !\n");

    // Clean up
    free(i1->mnemonic); free(i1->operand1); free(i1->operand2); free(i1);
    free(i2->mnemonic); free(i2->operand1); free(i2->operand2); free(i2);
    free(i3->mnemonic); free(i3->operand1); free(i3->operand2); free(i3);
    free(i4->mnemonic); free(i4->operand1); free(i4->operand2); free(i4);
    hashmap_destroy(memory_locations);
}


void test_parse_code_instruction() {
    const char *lines[] = {
        "loop : MOV AX ,6",
        "ADD BX ,10",
        "end : JMP loop"
    };

    int num_lines = sizeof(lines) / sizeof(lines[0]);
    Instruction **instructions = malloc(num_lines * sizeof(Instruction *));
    assert(instructions != NULL);

    HashMap *labels = hashmap_create();
    assert(labels);

    for (int i = 0; i < num_lines; i++) {
        instructions[i] = parse_code_instruction(lines[i], labels, i);
    }
    assert(strcmp(instructions[0]->mnemonic,"MOV")==0);
    assert(strcmp(instructions[1]->mnemonic,"ADD")==0);
    assert(strcmp(instructions[2]->mnemonic,"JMP")==0);

    assert(strcmp(instructions[0]->operand1,"AX")==0);
    assert(strcmp(instructions[1]->operand1,"BX")==0);
    assert(strcmp(instructions[2]->operand1,"loop")==0);

    assert( instructions[0]->operand2 && strcmp(instructions[0]->operand2,"6")==0);
    assert(instructions[1]->operand2 && strcmp(instructions[1]->operand2,"10")==0);
    assert(!instructions[2]->operand2);


    // printf("=== Instructions ===\n");
    // for (int i = 0; i < num_lines; i++) {
    //     Instruction *inst = instructions[i];
    //     printf("Instruction %d:\n", i);
    //     printf("  Mnemonic: %s\n", inst->mnemonic ? inst->mnemonic : "NULL");
    //     printf("  Operand1: %s\n", inst->operand1 ? inst->operand1 : "NULL");
    //     printf("  Operand2: %s\n", inst->operand2 ? inst->operand2 : "NULL");
    //     printf("\n");
    // }

    // Free memory
    for (int i = 0; i < num_lines; i++) {
        free(instructions[i]->mnemonic);
        free(instructions[i]->operand1);
        free(instructions[i]->operand2);
        free(instructions[i]);
    }
    free(instructions);
    hashmap_destroy(labels);
    printf("\n\t\u2705 test passed 🥳 !\n");
}


int main()
{
    test_parse_data_instruction();
    
    test_parse_code_instruction();
    printf("\n\t\u2705 All tests passed 🥳 !\n");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main()
{
    test_parse_data_instruction();
    printf("\n\t\u2705 All tests passed 🥳 !\n");
    return 0;
}

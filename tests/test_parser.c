#include "parser_result.h"  
#include <assert.h>  
void test_parser_result(ParserResult *result) {
    printf("testing DATA INSTRUCTIONS .....\n");
    assert(result->data_count==3);
    assert(strcmp(result->data_instructions[0]->mnemonic,"x")==0);
    assert(strcmp(result->data_instructions[1]->mnemonic,"arr")==0);
    assert(strcmp(result->data_instructions[2]->mnemonic,"y")==0);
    printf("\u2705 DATA INSTRUCTIONS mnemonic passed .....\n");

    assert(strcmp(result->data_instructions[0]->operand1,"DW")==0);
    assert(strcmp(result->data_instructions[1]->operand1,"DB")==0);
    assert(strcmp(result->data_instructions[2]->operand1,"DB")==0);
    printf("\u2705 DATA INSTRUCTIONS operand1 passed .....\n");

    assert(strcmp(result->data_instructions[0]->operand2,"42")==0);
    assert(strcmp(result->data_instructions[1]->operand2,"20,21,22,23")==0);
    assert(strcmp(result->data_instructions[2]->operand2,"10")==0);
    printf("\u2705 DATA INSTRUCTIONS operand2 passed .....\n");

    printf("\nTesting  CODE INSTRUCTIONS.....\n");
    assert(result->code_count==3);

    assert(strcmp(result->code_instructions[0]->mnemonic,"MOV")==0);
    assert(strcmp(result->code_instructions[1]->mnemonic,"ADD")==0);
    assert(strcmp(result->code_instructions[2]->mnemonic,"JMP")==0);
    printf("\u2705 CODE INSTRUCTIONS mnemonic passed .....\n");


    assert(strcmp(result->code_instructions[0]->operand1,"AX")==0);
    assert(strcmp(result->code_instructions[1]->operand1,"AX")==0);
    assert(strcmp(result->code_instructions[2]->operand1,"loop")==0);
    printf("\u2705 CODE INSTRUCTIONS operand1 passed .....\n");

    assert(strcmp(result->code_instructions[0]->operand2,"x")==0);
    assert(strcmp(result->code_instructions[1]->operand2,"y")==0);
    assert(!result->code_instructions[2]->operand2);
    printf("\u2705 CODE INSTRUCTIONS  operand2 passed .....\n");

    printf("\ntesting labels......\n");
    assert((long)hashmap_get(result->labels,"start")==0);
    assert((long)hashmap_get(result->labels,"loop")==1);
    printf("\u2705 test for labels passed .....\n");



    printf("\nTesting MEMORY LOCATIONS ......\n");
    assert((long)hashmap_get(result->memory_locations,"x")==0);
    assert((long)hashmap_get(result->memory_locations,"arr")==1);
    assert((long)hashmap_get(result->memory_locations,"y")==5);
    printf("\u2705 test for MEMORY LOCATIONS.....\n");

    
}

int main() {
    ParserResult *result = parse("tests/example.asm");
    test_parser_result(result);
    printf("\n\u2705 All tests passed 🥳 !\n");
    return 0;
}

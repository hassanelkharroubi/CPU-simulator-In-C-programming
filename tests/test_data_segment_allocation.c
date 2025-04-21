#include "data_segment_allocation.h"
#include <assert.h>

// Test for store
void test_store(CPU *cpu) {
    MemoryHandler *handler = cpu->memory_handler;
    // Create a segment
    assert(create_segment(handler, "DS", 0,10));
    int *val = malloc(sizeof(int));
    *val = 99;

    assert(store(handler, "DS", 0, val));
    printf("\n\u2705 Store test passed 🥳 !\n");
}

// Test for load
void test_load(CPU *cpu) {

    MemoryHandler *handler = cpu->memory_handler;
    // Create a segment
    assert(create_segment(handler, "DS", 0,10));

    int *val = malloc(sizeof(int));
    *val = 123;

    assert(store(handler, "DS", 5, val));  // Store at position 5
    int *retrieved = load(handler, "DS", 5);

    assert(*retrieved == 123);
    printf("\n\u2705 Load test passed 🥳 !\n");

}



// Test for allocate_variables
void test_allocate_variables() {

    CPU *cpu = cpu_init(1024);
    ParserResult *parser= parse("tests/example.asm") ;
    assert(parser);
    allocate_variables(cpu, parser->data_instructions, parser->data_count);
    printf("\n\u2705 Allocate variables test passed 🥳 !\n");

}


// Test for print_data_segment
void test_print_data_segment() {

}







void test_cpu(CPU *cpu)
{
    assert(cpu);
    assert((long)hashmap_get(cpu->context,"AX")==0);
    assert((long)hashmap_get(cpu->context,"BX")==0);
    assert((long)hashmap_get(cpu->context,"CX")==0);
    printf("\n\u2705 CPU   test passed 🥳 !\n");
}

int main()
{

    CPU *cpu = cpu_init(1024);
    test_cpu(cpu);
    test_store(cpu);
    test_load(cpu);
    test_allocate_variables(cpu);
    cpu_destroy(cpu);
    printf("\n\u2705 All   tests passed 🥳 !\n");


    return 0;
}

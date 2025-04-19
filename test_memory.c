#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memory_handler.h"
#include "hashmap.h"

void test_memory_init() {
    MemoryHandler* handler = memory_init(1024);
    assert(handler != NULL);
    assert(handler->total_size == 1024);
    assert(handler->free_list != NULL);
    assert(handler->free_list->start == 0);
    assert(handler->free_list->size == 1024);
    printf("\u2714 test_memory_init passed.\n");
}

void test_find_free_segment() {
    MemoryHandler* handler = memory_init(1024);
    Segment* prev = NULL;
    Segment* found = find_free_segment(handler, 100, 200, &prev);
    assert(found != NULL);
    assert(found->start == 0 && found->size == 1024);
    printf("\u2714 test_find_free_segment passed.\n");
}

void test_create_segment() {
    MemoryHandler* handler = memory_init(1024);
    int success = create_segment(handler, "data", 100, 200);
    assert(success == 1);
    Segment* seg = (Segment*) hashmap_get(handler->allocated, "data");
    assert(seg != NULL);
    assert(seg->start == 100 && seg->size == 200);
    printf("\u2714 test_create_segment passed.\n");
}

void test_remove_segment() {
    MemoryHandler* handler = memory_init(1024);
    create_segment(handler, "data", 100, 200);
    int removed = remove_segment(handler, "data");
    assert(removed == 1);
    Segment* seg = (Segment*) hashmap_get(handler->allocated, "data");
    assert(seg == NULL);
    printf("\u2714 test_remove_segment passed.\n");
}

int main() {
    test_memory_init();
    test_find_free_segment();
    test_create_segment();
    test_remove_segment();
    printf("\n\u2705 All tests passed!\n");
    return 0;
}

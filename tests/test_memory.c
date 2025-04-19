#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "memory_handler.h"
#include "hashmap.h"

void test_memory_init()
{
    MemoryHandler *handler = memory_init(1024);
    assert(handler != NULL);
    assert(handler->total_size == 1024);
    assert(handler->free_list != NULL);
    assert(handler->free_list->start == 0);
    assert(handler->free_list->size == 1024);
    printf("\u2705 test_memory_init passed.\n");
}

void test_find_free_segment()
{
    MemoryHandler *handler = memory_init(1024);
    Segment *prev = NULL;
    Segment *found = find_free_segment(handler, 100, 200, &prev);
    assert(found != NULL);
    assert(found->start == 0 && found->size == 1024);
    printf("\u2705 test_find_free_segment passed.\n");
}

void test_create_segment()
{
    printf("🔍 test_create_segment...\n");
    MemoryHandler *handler = memory_init(1024);

    // Test 1
    int success = create_segment(handler, "data", 100, 100);
    assert(success == 1);
    printf("✅ test_create_segment passed for TEST 1.\n");

    Segment *seg = (Segment *)hashmap_get(handler->allocated, "data");
    assert(seg != NULL);
    assert(seg->start == 100 && seg->size == 100);

    Segment *free = handler->free_list;
    int found_before = 0, found_after = 0;
    while (free)
    {
        if (free->start == 0 && free->size == 100)
            found_before = 1;
        if (free->start == 200 && free->size == 824)
            found_after = 1;
        free = free->next;
    }
    assert(found_before && found_after);

    // Test 2
    success = create_segment(handler, "cache", 300, 100);
    assert(success == 1);

    seg = (Segment *)hashmap_get(handler->allocated, "cache");
    assert(seg != NULL);
    assert(seg->start == 300 && seg->size == 100);

    free = handler->free_list;
    int found_gap1 = 0, found_gap2 = 0, found_gap3 = 0;
    while (free)
    {
        if (free->start == 0 && free->size == 100)
            found_gap1 = 1;
        if (free->start == 200 && free->size == 100)
            found_gap2 = 1;
        if (free->start == 400 && free->size == 624)
            found_gap3 = 1;
        free = free->next;
    }
    assert(found_gap1 && found_gap2 && found_gap3);
    printf("✅ test_create_segment passed for TEST 2.\n");

    printf("✅ test_create_segment passed for All Tests.\n");
}

void test_remove_segment() {
    MemoryHandler* handler = memory_init(1024);

    assert(create_segment(handler, "A", 100, 100)); // free: [0,100],[200,924]
    assert(create_segment(handler, "B", 200, 100)); // free: [0,100],[300,824]

    assert(remove_segment(handler, "A"));          // free: [0,100],[100,100],[300,824] → should merge [0,200]
    assert(remove_segment(handler, "B"));          // free: [0,200],[200,100],[300,824] → full merge to [0,1024]

    Segment* seg = handler->free_list;
    assert(seg->start == 0);
    assert(seg->size == 1024);
    assert(seg->next == NULL);
    printf("✅ test_remove_segment passed\n");
}


int main()
{
    test_memory_init();
    test_find_free_segment();
    test_create_segment();
    test_remove_segment();
    printf("\n\t\u2705 All tests passed 🥳 !\n");
    return 0;
}

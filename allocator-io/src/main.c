#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "allocator.h"
#include "tester.h"

static void *
buf_alloc(size_t size) {
    char *buf;
    size_t i;

    buf = mem_alloc(size);
    if (buf != NULL)
        for (i = 0; i < size; ++i)
            buf[i] = (char)rand();
    return buf;
}

int main(void) {

    void *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;
//
//    buf_alloc(SIZE_MAX);
//    mem_show("Initial");
//
//    ptr1 = buf_alloc(1);
//    mem_show("\nalloc(1)");
//    ptr2 = buf_alloc(100);
//    mem_show("\nalloc(100)");
//    ptr3 = buf_alloc(30);
//    mem_show("\nalloc(30)");
//    ptr4 = buf_alloc(20000);
//    mem_show("\nalloc(20000)");
//    ptr5 = buf_alloc(SIZE_MAX-1);
//    mem_show("\nalloc(SIZE_MAX-1)");
//
//    ptr2 = mem_realloc(ptr2, 200);
//    if (ptr2 != NULL) mem_show("\nrealloc(ptr2, 200)");
//    else printf("\nFailed to reallocate memory for ptr2\n");
//
//    ptr4 = mem_realloc(ptr4, SIZE_MAX);
//    if (ptr4 != NULL) mem_show("\nrealloc(ptr4, SIZE_MAX)");
//    else printf("\nFailed to reallocate memory for ptr4\n");
//
//    mem_free(ptr2);
//    mem_show("\nfree(ptr2)");
//    mem_free(ptr4);
//    mem_show("\nfree(ptr4)");
//    mem_free(ptr1);
//    mem_show("\nfree(ptr1)");
//    mem_free(ptr3);
//    mem_show("\nfree(ptr3)");

    srand(100);
    tester(true);
}

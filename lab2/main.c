#include <stdio.h>
#include "slab.h"
#include "tester.h"
#include "config.h"

int main() {
    void* obj1 = mem_alloc(sizeof(int));
    void* obj2 = mem_alloc(sizeof(int));

    void* obj3 = mem_alloc(100);
    mem_show("State of the memory after creation obj1-3:");
    getchar();

    obj2 = mem_realloc(obj2, 450);
    mem_show("Memory status after reallocation obj2:");
    getchar();

    mem_free(obj1);
    mem_show("Memory state after release obj1:");
    getchar();

    mem_free(obj2);
    mem_show("Memory state after release obj2:");
    getchar();

    const int N = PAGE_NUM * 4 * 16;
    void* array[PAGE_NUM * 4 * 16];
    for (int i = 0; i < N; i++) {
        array[i] = mem_alloc(1024);
    }
    mem_show("Full memory status:\n");
    getchar();

    void* obj4 = mem_alloc(4);
    mem_show("Full memory state + 1 object:");
//    tester(true);

    return 0;
}

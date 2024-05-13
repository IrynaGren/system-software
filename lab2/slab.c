#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "kernel.h"
#include "config.h"

typedef struct slab_header {
    size_t size;
    size_t used;
    void* free;
} slab_header_t;

typedef struct slab {
    slab_header_t header;
    char data[PAGE_NUM * PAGE_SIZE];
} slab_t;

typedef struct cache {
    slab_t* slabs[CACHE_SIZE];
    size_t count;
} cache_t;

cache_t cache;

slab_t* alloc_slab(size_t size) {
    slab_t* slab = kernel_alloc(sizeof(slab_t));
    if (!slab) return NULL;

    slab->header.size = size;
    slab->header.used = 0;
    slab->header.free = slab->data;

    return slab;
}

void free_slab(slab_t* slab) {
    kernel_free(slab);
}

void* mem_alloc(size_t size) {
    if (size > MAX_OBJ_SIZE) {
        fprintf(stderr, "Error: object size exceeds the maximum object size\n");
        return NULL;
    }

    for (int i = 0; i < cache.count; i++) {
        slab_t* slab = cache.slabs[i];

        if (slab->header.size == size && slab->header.free && slab->header.used < PAGE_SIZE * PAGE_NUM / slab->header.size) {
            slab->header.used++;
            void* ptr = slab->header.free;
            slab->header.free = (char*)slab->header.free + size;
            return ptr;
        }
    }

    slab_t* slab = alloc_slab(size);
    if (!slab) return NULL;

    slab->header.used = 1;
    slab->header.free = slab->data + size;

    if (cache.count < CACHE_SIZE) {
        cache.slabs[cache.count++] = slab;
    } else {
        int victim_index = -1;
        size_t min_used = (size_t)-1;
        for (int i = 0; i < cache.count; i++) {
            slab_t* curr_slab = cache.slabs[i];
            if (curr_slab->header.used < min_used) {
                victim_index = i;
                min_used = curr_slab->header.used;
            }
        }

        if (victim_index == -1) {
            fprintf(stderr, "Error: cache is full, but the least used one is not found slab\n");
            return NULL;
        }

        free_slab(cache.slabs[victim_index]);
        cache.slabs[victim_index] = slab;
    }

    return slab->data;
}

void mem_free(void* ptr) {
    for (int i = 0; i < cache.count; i++) {
        slab_t* slab = cache.slabs[i];

        if (ptr >= (void*)slab->data && ptr < (void*)(slab->data + PAGE_NUM * PAGE_SIZE)) {
            slab->header.used--;

            if (slab->header.used == 0) {
                for (int j = i; j < cache.count - 1; j++) {
                    cache.slabs[j] = cache.slabs[j + 1];
                }
                cache.count--;

                free_slab(slab);
            }
            return;
        }
    }

    fprintf(stderr, "Error: attempting to delete an unknown object\n");
}

void* mem_realloc(void* ptr, size_t new_size) {
    for (int i = 0; i < cache.count; i++) {
        slab_t* slab = cache.slabs[i];

        if (ptr >= (void*)slab->data && ptr < (void*)(slab->data + PAGE_NUM * PAGE_SIZE)) {
            size_t current_size = slab->header.size;

            if (new_size <= current_size) return ptr;

            if (new_size > MAX_OBJ_SIZE) {
                fprintf(stderr, "Error: new size exceeds the maximum size of the object\n\n");
                return ptr;
            }

            void* new_ptr = mem_alloc(new_size);
            if (!new_ptr) {
                fprintf(stderr, "Error: Unable to allocate memory for a new object\n\n");
                return ptr;
            }

            memcpy(new_ptr, ptr, current_size);

            mem_free(ptr);

            return new_ptr;
        }
    }

    fprintf(stderr, "Error: attempting to resize an unknown object\n\n");
    return NULL;
}

void slab_info(slab_t* slab, int slab_num) {
    printf("\nSlab number %d (%p)\n", slab_num, (void*)slab);
    printf("   Size of the object:\t%zu Byte\n", slab->header.size);
    printf("   Used by.:\t\t%zu/%zu\n", slab->header.used, PAGE_SIZE * PAGE_NUM / slab->header.size);
    printf("   Condition:\t\t%s\n\n", slab->header.used == 0 ? "empty" : (slab->header.used == PAGE_SIZE * PAGE_NUM / slab->header.size ? "full" : "partial"));
}

void mem_show(const char* message) {
    printf(message);
    for (int i = 0; i < cache.count; i++) {
        slab_info(cache.slabs[i], i + 1);
    }
    printf("=======================================\n");
}

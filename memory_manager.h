#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stddef.h> // For size_t

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


// Helps C++ compilers to handle C header files
/*#ifdef __cplusplus
extern "C"
{
#endif*/

#define POOL_SIZE 81920000 
#define MAX_BLOCKS 100000      


typedef struct Block {
    void* address; // 
    size_t size;   //
    bool is_free;  // 
    struct Block* next; // 
} Block;




void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit(void);
#ifdef __cplusplus
}
#endif

#endif // MEMORY_MANAGER_H

#include "memory_manager.h"
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static pthread_mutex_t memory_mutex = PTHREAD_MUTEX_INITIALIZER;

void* memory_pool; // 
Block* head_pool;  // 

void mem_init(size_t size) {
    pthread_mutex_init(&memory_mutex, NULL);  //  
    pthread_mutex_lock(&memory_mutex);
    memory_pool = malloc(size);
    if (memory_pool == NULL) {
        printf("Failed to allocate memory pool.\n");
        pthread_mutex_unlock(&memory_mutex);
        return;
    }

    head_pool = (Block*)malloc(sizeof(Block));
    if (head_pool == NULL) {
        printf("Failed to allocate head block.\n");
        pthread_mutex_unlock(&memory_mutex);
        return;
    }

    head_pool->address = memory_pool;
    head_pool->size = size;
    head_pool->is_free = true;
    head_pool->next = NULL;
    pthread_mutex_unlock(&memory_mutex);
}

void* mem_alloc(size_t size) {
    pthread_mutex_lock(&memory_mutex);
    Block* current = head_pool;

    while (current != NULL) {
        if (current->is_free && current->size >= size) {
            current->is_free = false;

            if (current->size > size) {
                Block* new_block = (Block*)malloc(sizeof(Block));
                new_block->address = current->address + size;
                new_block->size = current->size - size;
                new_block->is_free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            pthread_mutex_unlock(&memory_mutex);
            return current->address;
        }
        current = current->next;
    }

    printf("No suitable block found.\n");
    pthread_mutex_unlock(&memory_mutex);
    return NULL;
}

void mem_free(void* block) {
    pthread_mutex_lock(&memory_mutex);
    Block* current = head_pool;
    Block* prev = NULL;

    while (current != NULL) {
        if (current->address == block) {
            current->is_free = true;

            //         
            if (current->next != NULL && current->next->is_free) {
                Block* next_block = current->next;
                current->size += next_block->size;
                current->next = next_block->next;
                free(next_block);
            }

            
            if (prev != NULL && prev->is_free) {
                prev->size += current->size;
                prev->next = current->next;
                free(current);
                current = prev;
            }

            pthread_mutex_unlock(&memory_mutex);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Block not found.\n");
    pthread_mutex_unlock(&memory_mutex);
}

void* mem_resize(void* block, size_t size) {
    pthread_mutex_lock(&memory_mutex);
    Block* current = head_pool;

    while (current != NULL) {
        if (current->address == block) {
            //  
            if (current->size >= size) {
                pthread_mutex_unlock(&memory_mutex);
                return block;
            }

            //    
            pthread_mutex_unlock(&memory_mutex);  // 
            void* new_block = mem_alloc(size);

            if (new_block == NULL) {
                return NULL;
            }

            // 
            pthread_mutex_lock(&memory_mutex);
            memcpy(new_block, block, current->size);
            pthread_mutex_unlock(&memory_mutex);

            mem_free(block);
            return new_block;
        }
        current = current->next;
    }

    printf("Block not found for resizing.\n");
    pthread_mutex_unlock(&memory_mutex);
    return NULL;
}

void mem_deinit() {
    pthread_mutex_lock(&memory_mutex);
    free(memory_pool);
    memory_pool = NULL;

    Block* current = head_pool;
    while (current != NULL) {
        Block* temp = current;
        current = current->next;
        free(temp);
    }
    head_pool = NULL;
    pthread_mutex_unlock(&memory_mutex);
    pthread_mutex_destroy(&memory_mutex);  //  
}

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "memory_manager.h"

// Definition of a singly linked list node.
typedef struct Node {
    uint16_t data;        // The data stored in the node.
    struct Node* next;    // Pointer to the next node in the list.
    pthread_mutex_t lock; 
} Node;

void list_init(Node** head, size_t size);               
void list_insert(Node** head, uint16_t data);                  
void list_insert_after(Node* prev_node, uint16_t data);        
void list_insert_before(Node** head, Node* next_node, uint16_t data);  
void list_delete(Node** head, uint16_t data);                  
Node* list_search(Node** head, uint16_t data);                
void list_display(Node** head);                         
void list_display_range(Node** head, Node* start_node, Node* end_node); 
int list_count_nodes(Node** head);                        
void list_cleanup(Node** head);                          

#endif

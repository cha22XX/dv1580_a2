#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include "memory_manager.h"
#include "memory_manager.c"
#include "linked_list.h"

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  

void list_init(Node** head, size_t size) {
    *head = NULL;
    mem_init(size);
}

void list_insert(Node** head, uint16_t data) {
    pthread_mutex_lock(&mutex); 

    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed\n");
        pthread_mutex_unlock(&mutex); 
        return;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    pthread_mutex_unlock(&mutex); 
}

void list_insert_after(Node* prev_node, uint16_t data) {
    if (prev_node == NULL) {
        printf("Previous node cannot be NULL\n");
        return;
    }

    pthread_mutex_lock(&mutex); 

    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed\n");
        pthread_mutex_unlock(&mutex); 
        return;
    }

    new_node->data = data;
    new_node->next = prev_node->next;
    prev_node->next = new_node;

    pthread_mutex_unlock(&mutex); 
}

void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    if (next_node == NULL) {
        printf("Cannot insert before NULL node.\n");
        return;
    }

    pthread_mutex_lock(&mutex); 

    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed\n");
        pthread_mutex_unlock(&mutex); 
        return;
    }
    new_node->data = data;

    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node* temp = *head;
        while (temp != NULL && temp->next != next_node) {
            temp = temp->next;
        }

        if (temp == NULL) {
            printf("Node not found in the list.\n");
            mem_free(new_node);
            pthread_mutex_unlock(&mutex); 
            return;
        }

        new_node->next = next_node;
        temp->next = new_node;
    }

    pthread_mutex_unlock(&mutex); 
}

void list_delete(Node** head, uint16_t data) {
    pthread_mutex_lock(&mutex);

    if (*head == NULL) {
        printf("List is empty\n");
        pthread_mutex_unlock(&mutex); 
        return;
    }

    Node* temp = *head;
    Node* previous = NULL;

    while (temp != NULL && temp->data != data) {
        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Data not found in the list\n");
        pthread_mutex_unlock(&mutex); 
        return;
    }

    if (previous == NULL) {
        *head = temp->next;
    } else {
        previous->next = temp->next;
    }

    mem_free(temp);

    pthread_mutex_unlock(&mutex); 
}

Node* list_search(Node** head, uint16_t data) {
    pthread_rwlock_rdlock(&rwlock); 

    Node* temp = *head;
    while (temp != NULL) {
        if (temp->data == data) {
            pthread_rwlock_unlock(&rwlock); 
            return temp;
        }
        temp = temp->next;
    }

    pthread_rwlock_unlock(&rwlock); 
    return NULL;
}

void list_display(Node** head) {
    pthread_rwlock_rdlock(&rwlock); 

    Node* temp = *head;
    printf("[");
    while (temp != NULL) {
        printf("%u", temp->data);
        if (temp->next != NULL) {
            printf(", ");
        }
        temp = temp->next;
    }
    printf("]");

    pthread_rwlock_unlock(&rwlock); 
}

void list_display_range(Node** head, Node* start_node, Node* end_node) {
    pthread_rwlock_rdlock(&rwlock); 

    Node* temp = start_node ? start_node : *head;
    printf("[");
    while (temp != NULL && (end_node == NULL || temp != end_node->next)) {
        printf("%u", temp->data);
        if (temp->next != NULL && temp != end_node) {
            printf(", ");
        }
        temp = temp->next;
    }
    printf("]");

    pthread_rwlock_unlock(&rwlock); 
}

int list_count_nodes(Node** head) {
    pthread_rwlock_rdlock(&rwlock); 

    int count = 0;
    Node* temp = *head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    pthread_rwlock_unlock(&rwlock); 
    return count;
}

void list_cleanup(Node** head) {
    pthread_mutex_lock(&mutex);

    Node* temp = *head;
    while (temp != NULL) {
        Node* next_node = temp->next;
        mem_free(temp);
        temp = next_node;
    }
    *head = NULL;
    mem_deinit();

    pthread_mutex_unlock(&mutex);
}

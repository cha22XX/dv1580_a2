/*#include "memory_manager.c"
#include "linked_list.h"
#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void list_init(Node **head, size_t size) {
    mem_init(size);
    *head = NULL;
}

void list_insert(Node** head, uint16_t data) {
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_node->data = data;
    new_node->next = NULL;
    pthread_mutex_init(&new_node->lock, NULL); //

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        pthread_mutex_lock(&temp->lock); //

        while (temp->next != NULL) {
            Node* next = temp->next;
            pthread_mutex_lock(&next->lock); // 
            pthread_mutex_unlock(&temp->lock); // 
            temp = next;
        }

        temp->next = new_node;
        pthread_mutex_unlock(&temp->lock); // 
    }
}

void list_insert_after(Node *prev_node, uint16_t data) {
    if (prev_node == NULL) {
        printf("Previous node cannot be NULL.\n");
        return;
    }

    Node *new_node = (Node *)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_node->data = data;
    new_node->next = prev_node->next;
    pthread_mutex_init(&new_node->lock, NULL);

    pthread_mutex_lock(&prev_node->lock);
    prev_node->next = new_node;
    pthread_mutex_unlock(&prev_node->lock);
}

void list_insert_before(Node **head, Node *next_node, uint16_t data) {
    if (*head == NULL || next_node == NULL) {
        printf("Cannot insert before NULL node.\n");
        return;
    }

    Node *new_node = (Node *)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_node->data = data;
    pthread_mutex_init(&new_node->lock, NULL);

    pthread_mutex_lock(&new_node->lock);

    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node *temp = *head;
        pthread_mutex_lock(&temp->lock);

        while (temp != NULL && temp->next != next_node) {
            pthread_mutex_unlock(&temp->lock);
            temp = temp->next;
            pthread_mutex_lock(&temp->lock);
        }

        if (temp == NULL) {
            printf("Node not found in the list.\n");
            mem_free(new_node);
            pthread_mutex_unlock(&new_node->lock);
            return;
        }

        new_node->next = temp->next;
        temp->next = new_node;

        pthread_mutex_unlock(&temp->lock);
    }

    pthread_mutex_unlock(&new_node->lock);
}

void list_delete(Node **head, uint16_t data) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *temp = *head;
    Node *prev = NULL;

    pthread_mutex_lock(&temp->lock);

    if (temp->data == data) {
        *head = temp->next;
        pthread_mutex_unlock(&temp->lock);
        pthread_mutex_destroy(&temp->lock);
        mem_free(temp);
        return;
    }

    while (temp != NULL && temp->data != data) {
        if (prev != NULL) {
            pthread_mutex_unlock(&prev->lock);
        }
        prev = temp;
        temp = temp->next;
        if (temp != NULL) {
            pthread_mutex_lock(&temp->lock);
        }
    }

    if (temp == NULL) {
        printf("Data not found in the list.\n");
        if (prev != NULL) {
            pthread_mutex_unlock(&prev->lock);
        }
        return;
    }

    prev->next = temp->next;
    pthread_mutex_unlock(&prev->lock);
    pthread_mutex_unlock(&temp->lock);
    pthread_mutex_destroy(&temp->lock);
    mem_free(temp);
}

Node *list_search(Node **head, uint16_t data) {
    Node *temp = *head;
    while (temp != NULL) {
        pthread_mutex_lock(&temp->lock);
        if (temp->data == data) {
            pthread_mutex_unlock(&temp->lock);
            return temp;
        }
        pthread_mutex_unlock(&temp->lock);
        temp = temp->next;
    }
    return NULL;
}

void list_display(Node **head) {
    Node *temp = *head;
    if (temp == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");
    while (temp != NULL) {
        pthread_mutex_lock(&temp->lock);
        printf("%d", temp->data);
        temp = temp->next;
        if (temp != NULL) {
            printf(", ");
        }
        pthread_mutex_unlock(&temp->lock);
    }
    printf("]\n");
}

void list_display_range(Node **head, Node *start_node, Node *end_node) {
    Node *temp = (start_node != NULL) ? start_node : *head;
    if (temp == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");
    while (temp != NULL && (end_node == NULL || temp != end_node->next)) {
        pthread_mutex_lock(&temp->lock);
        printf("%d", temp->data);
        temp = temp->next;
        if (temp != NULL && (end_node == NULL || temp != end_node->next)) {
            printf(", ");
        }
        pthread_mutex_unlock(&temp->lock);
    }
    printf("]\n");
}

int list_count_nodes(Node **head) {
    int count = 0;
    Node *temp = *head;
    while (temp != NULL) {
        pthread_mutex_lock(&temp->lock);
        count++;
        temp = temp->next;
        pthread_mutex_unlock(&temp->lock);
    }
    return count;
}

void list_cleanup(Node **head) {
    Node *temp = *head;
    while (temp != NULL) {
        Node *next = temp->next;
        pthread_mutex_destroy(&temp->lock);
        mem_free(temp);
        temp = next;
    }
    *head = NULL;
}*/

/*
#include "memory_manager.c"
#include "linked_list.h"
#include "memory_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void list_init(Node **head, size_t size) {
    mem_init(size);
    *head = NULL;
}

void list_insert(Node** head, uint16_t data) {
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_node->data = data;
    new_node->next = NULL;
    pthread_mutex_init(&new_node->lock, NULL); // 

    if (*head == NULL) {
        *head = new_node;
    } else {
        Node* temp = *head;
        pthread_mutex_lock(&temp->lock); // 

        while (temp->next != NULL) {
            Node* next = temp->next;
            pthread_mutex_lock(&next->lock); // 
            pthread_mutex_unlock(&temp->lock); //
            temp = next;
        }

        temp->next = new_node;
        pthread_mutex_unlock(&temp->lock); //
    }
}

void list_insert_after(Node *prev_node, uint16_t data) {
    if (prev_node == NULL) {
        printf("Previous node cannot be NULL.\n");
        return;
    }

    Node *new_node = (Node *)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_node->data = data;
    new_node->next = prev_node->next;
    pthread_mutex_init(&new_node->lock, NULL);

    pthread_mutex_lock(&prev_node->lock);
    prev_node->next = new_node;
    pthread_mutex_unlock(&prev_node->lock);
}

void list_insert_before(Node **head, Node *next_node, uint16_t data) {
    if (*head == NULL || next_node == NULL) {
        printf("Cannot insert before NULL node.\n");
        return;
    }

    Node *new_node = (Node *)mem_alloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_node->data = data;
    pthread_mutex_init(&new_node->lock, NULL);

    pthread_mutex_lock(&new_node->lock);

    if (*head == next_node) {
        new_node->next = *head;
        *head = new_node;
    } else {
        Node *temp = *head;
        pthread_mutex_lock(&temp->lock);

        while (temp != NULL && temp->next != next_node) {
            pthread_mutex_unlock(&temp->lock);
            temp = temp->next;
            pthread_mutex_lock(&temp->lock);
        }

        if (temp == NULL) {
            printf("Node not found in the list.\n");
            mem_free(new_node);
            pthread_mutex_unlock(&new_node->lock);
            return;
        }

        new_node->next = temp->next;
        temp->next = new_node;

        pthread_mutex_unlock(&temp->lock);
    }

    pthread_mutex_unlock(&new_node->lock);
}

void list_delete(Node **head, uint16_t data) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }

    Node *temp = *head;
    Node *prev = NULL;

    pthread_mutex_lock(&temp->lock);

    if (temp->data == data) {
        *head = temp->next;
        pthread_mutex_unlock(&temp->lock);
        pthread_mutex_destroy(&temp->lock);
        mem_free(temp);
        return;
    }

    while (temp != NULL && temp->data != data) {
        if (prev != NULL) {
            pthread_mutex_unlock(&prev->lock);
        }
        prev = temp;
        temp = temp->next;
        if (temp != NULL) {
            pthread_mutex_lock(&temp->lock);
        }
    }

    if (temp == NULL) {
        printf("Data not found in the list.\n");
        if (prev != NULL) {
            pthread_mutex_unlock(&prev->lock);
        }
        return;
    }

    prev->next = temp->next;
    pthread_mutex_unlock(&prev->lock);
    pthread_mutex_unlock(&temp->lock);
    pthread_mutex_destroy(&temp->lock);
    mem_free(temp);
}

Node *list_search(Node **head, uint16_t data) {
    Node *temp = *head;
    while (temp != NULL) {
        pthread_mutex_lock(&temp->lock);
        if (temp->data == data) {
            pthread_mutex_unlock(&temp->lock);
            return temp;
        }
        pthread_mutex_unlock(&temp->lock);
        temp = temp->next;
    }
    return NULL;
}

void list_display(Node **head) {
    Node *temp = *head;
    if (temp == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");
    while (temp != NULL) {
        pthread_mutex_lock(&temp->lock);
        printf("%d", temp->data);
        temp = temp->next;
        if (temp != NULL) {
            printf(", ");
        }
        pthread_mutex_unlock(&temp->lock);
    }
    printf("]\n");
}

void list_display_range(Node **head, Node *start_node, Node *end_node) {
    Node *temp = (start_node != NULL) ? start_node : *head;
    if (temp == NULL) {
        printf("[]\n");
        return;
    }

    printf("[");
    while (temp != NULL && (end_node == NULL || temp != end_node->next)) {
        pthread_mutex_lock(&temp->lock);
        printf("%d", temp->data);
        temp = temp->next;
        if (temp != NULL && (end_node == NULL || temp != end_node->next)) {
            printf(", ");
        }
        pthread_mutex_unlock(&temp->lock);
    }
    printf("]\n");
}

int list_count_nodes(Node **head) {
    int count = 0;
    Node *temp = *head;
    while (temp != NULL) {
        pthread_mutex_lock(&temp->lock);
        count++;
        temp = temp->next;
        pthread_mutex_unlock(&temp->lock);
    }
    return count;
}

void list_cleanup(Node **head) {
    Node *temp = *head;
    while (temp != NULL) {
        Node *next = temp->next;
        pthread_mutex_destroy(&temp->lock);
        mem_free(temp);
        temp = next;
    }
    *head = NULL;
}*/


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



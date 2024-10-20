

#include "linked_list.h"
#include "memory_manager.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>




void list_init(Node** head, size_t size) {
    mem_init(size); // 
    *head = NULL;
}

// 
Node* create_node(uint16_t data) {
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (!new_node) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    pthread_mutex_init(&new_node->node_mutex, NULL); // 
    return new_node;
}

// 
void list_insert(Node** head, uint16_t data) {
    Node* new_node = create_node(data);
    if (!new_node) {
        return;
    }

    if (*head == NULL) {
        // 
        *head = new_node;
    } else {
        Node* temp = *head;
        pthread_mutex_lock(&temp->node_mutex); // 

        while (temp->next != NULL) {
            Node* next = temp->next;
            pthread_mutex_lock(&next->node_mutex); // 
            pthread_mutex_unlock(&temp->node_mutex); // 
            temp = next;
        }

        temp->next = new_node;
        pthread_mutex_unlock(&temp->node_mutex); // 
    }
}

// 
void list_insert_after(Node* prev_node, uint16_t data) {
    if (prev_node == NULL) {
        printf("Previous node cannot be NULL.\n");
        return;
    }

    pthread_mutex_lock(&prev_node->node_mutex); // 
    Node* new_node = create_node(data);
    if (!new_node) {
        pthread_mutex_unlock(&prev_node->node_mutex);
        return;
    }
    
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    pthread_mutex_unlock(&prev_node->node_mutex); // 
}

// 
void list_insert_before(Node** head, Node* next_node, uint16_t data) {
    if (*head == NULL || next_node == NULL) {
        printf("Cannot insert before NULL node.\n");
        return;
    }

    pthread_mutex_lock(&(*head)->node_mutex); // 

    Node* new_node = create_node(data);
    if (!new_node) {
        pthread_mutex_unlock(&(*head)->node_mutex);
        return;
    }

    if (*head == next_node) {
        // 
        new_node->next = *head;
        *head = new_node;
        pthread_mutex_unlock(&new_node->node_mutex);
        return;
    }

    Node* temp = *head;
    while (temp != NULL && temp->next != next_node) {
        Node* next = temp->next;
        pthread_mutex_lock(&next->node_mutex); // 
        pthread_mutex_unlock(&temp->node_mutex); // 
        temp = next;
    }

    if (temp == NULL) {
        printf("Node not found in the list.\n");
        pthread_mutex_unlock(&temp->node_mutex);
        return;
    }

    new_node->next = temp->next;
    temp->next = new_node;
    pthread_mutex_unlock(&temp->node_mutex); // 
}

// 
void list_delete(Node** head, uint16_t data) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }

    pthread_mutex_lock(&(*head)->node_mutex); // 

    Node* temp = *head;
    Node* prev = NULL;

    if (temp != NULL && temp->data == data) {
        //
        *head = temp->next;
        pthread_mutex_unlock(&temp->node_mutex);
        pthread_mutex_destroy(&temp->node_mutex);
        mem_free(temp);
        return;
    }

    while (temp != NULL && temp->data != data) {
        prev = temp;
        temp = temp->next;
        if (temp != NULL) {
            pthread_mutex_lock(&temp->node_mutex); // 
        }
        pthread_mutex_unlock(&prev->node_mutex); // 
    }

    if (temp == NULL) {
        printf("Data not found in the list.\n");
        return;
    }

    prev->next = temp->next;
    pthread_mutex_unlock(&temp->node_mutex); // 
    pthread_mutex_destroy(&temp->node_mutex); // 
    mem_free(temp);
}

// 
Node* list_search(Node** head, uint16_t data) {
    Node* temp = *head;
    while (temp != NULL) {
        pthread_mutex_lock(&temp->node_mutex); // 
        if (temp->data == data) {
            pthread_mutex_unlock(&temp->node_mutex);
            return temp;
        }
        Node* next = temp->next;
        pthread_mutex_lock(&next->node_mutex); // 
        pthread_mutex_unlock(&temp->node_mutex); // 
        temp = next;
    }
    return NULL;
}

// 
int list_count_nodes(Node** head) {
    int count = 0;
    Node* temp = *head;
    while (temp != NULL) {
        pthread_mutex_lock(&temp->node_mutex); // 
        count++;
        Node* next = temp->next;
        pthread_mutex_lock(&next->node_mutex); // 
        pthread_mutex_unlock(&temp->node_mutex); // 
        temp = next;
    }
    return count;
}

// 
void list_cleanup(Node** head) {
    Node* temp = *head;
    while (temp != NULL) {
        Node* next = temp->next;
        pthread_mutex_destroy(&temp->node_mutex); // 
        mem_free(temp); // 
        temp = next;
    }
    *head = NULL;
}

// 
void list_display(Node** head) {
    Node* temp = *head;
    if (temp == NULL) {
        printf("[]"); // 
        return;
    }

    printf("[");
    while (temp != NULL) {
        pthread_mutex_lock(&temp->node_mutex); // 
        printf("%d", temp->data); // 
        Node* next = temp->next;
        if (next != NULL) {
            printf(", "); // 
        }
        pthread_mutex_unlock(&temp->node_mutex); // 
        temp = next;
    }
    printf("]\n");
}

// 
void list_display_range(Node** head, Node* start_node, Node* end_node) {
    Node* temp = (start_node != NULL) ? start_node : *head;

    if (temp == NULL) {
        printf("[]");
        return;
    }

    printf("[");
    while (temp != NULL && (end_node == NULL || temp != end_node->next)) {
        pthread_mutex_lock(&temp->node_mutex); // 
        printf("%d", temp->data); // 
        Node* next = temp->next;
        if (next != NULL && (end_node == NULL || next != end_node->next)) {
            printf(", "); // 
        }
        pthread_mutex_unlock(&temp->node_mutex); // 
        temp = next;
    }
    printf("]\n");
}

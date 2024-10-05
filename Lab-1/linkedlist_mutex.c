#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

static Node* head = NULL;
static pthread_mutex_t list_mutex;

void init_mutex() {
    pthread_mutex_init(&list_mutex, NULL);
}

void insert_mutex(int value) {
    pthread_mutex_lock(&list_mutex);
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
    pthread_mutex_unlock(&list_mutex);
}

void delete_mutex(int value) {
    pthread_mutex_lock(&list_mutex);
    Node* curr = head;
    Node* prev = NULL;

    while (curr != NULL && curr->data != value) {
        prev = curr;
        curr = curr->next;
    }

    if (curr != NULL) {
        if (prev == NULL) {
            head = curr->next;
        } else {
            prev->next = curr->next;
        }
        free(curr);
    }
    pthread_mutex_unlock(&list_mutex);
}

int member_mutex(int value) {
    pthread_mutex_lock(&list_mutex);
    Node* curr = head;
    while (curr != NULL) {
        if (curr->data == value) {
            pthread_mutex_unlock(&list_mutex);
            return 1;
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&list_mutex);
    return 0;
}

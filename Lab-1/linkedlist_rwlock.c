#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

static Node* head = NULL;
static pthread_rwlock_t list_rwlock;

void init_rwlock() {
    pthread_rwlock_init(&list_rwlock, NULL);
}

void insert_rwlock(int value) {
    pthread_rwlock_wrlock(&list_rwlock);
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
    pthread_rwlock_unlock(&list_rwlock);
}

void delete_rwlock(int value) {
    pthread_rwlock_wrlock(&list_rwlock);
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
    pthread_rwlock_unlock(&list_rwlock);
}

int member_rwlock(int value) {
    pthread_rwlock_rdlock(&list_rwlock);
    Node* curr = head;
    while (curr != NULL) {
        if (curr->data == value) {
            pthread_rwlock_unlock(&list_rwlock);
            return 1;
        }
        curr = curr->next;
    }
    pthread_rwlock_unlock(&list_rwlock);
    return 0;
}

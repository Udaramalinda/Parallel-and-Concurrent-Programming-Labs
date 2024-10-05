#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <pthread.h>

// Linked List Node structure
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Function declarations for Serial Linked List
void insert_serial(int value);
void delete_serial(int value);
int member_serial(int value);

// Function declarations for Mutex Linked List
void init_mutex();
void insert_mutex(int value);
void delete_mutex(int value);
int member_mutex(int value);

// Function declarations for Read-Write Lock Linked List
void init_rwlock();
void insert_rwlock(int value);
void delete_rwlock(int value);
int member_rwlock(int value);

#endif // LINKEDLIST_H

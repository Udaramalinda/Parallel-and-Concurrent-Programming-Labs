#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000  // Initial size of the linked list
#define M 10000 // Number of operations
#define THREAD_COUNT 4 // Number of threads to run concurrently

void* thread_work(void* arg) {
    int ops = M / THREAD_COUNT;
    for (int i = 0; i < ops; i++) {
        int op_type = rand() % 3;
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (op_type == 0) {
            member_mutex(value); // Perform member operation
        } else if (op_type == 1) {
            insert_mutex(value); // Perform insert operation
        } else {
            delete_mutex(value); // Perform delete operation
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    // Initialize the mutex and rwlock linked lists
    init_mutex();
    init_rwlock();

    // Create threads
    pthread_t threads[THREAD_COUNT];

    // Mutex-based linked list operations with multiple threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, thread_work, NULL);
    }

    // Join threads
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Completed operations on the linked list with Mutex locks\n");

    return 0;
}

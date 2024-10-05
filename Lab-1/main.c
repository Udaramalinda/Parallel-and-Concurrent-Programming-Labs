#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000  // Initial size of the linked list
#define M 10000 // Number of operations
#define THREAD_COUNT 4 // Number of threads to run concurrently

// Function for timing
double measure_time(void (*func)(void)) {
    clock_t start = clock();
    func();
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Example thread work function for mutex and read-write lock
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

// Function to run linked list operations with mutex
void run_mutex_operations() {
    init_mutex(); // Initialize mutex linked list
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, thread_work, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Function to run linked list operations with read-write locks
void run_rwlock_operations() {
    init_rwlock(); // Initialize read-write lock linked list
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, thread_work, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Function to run linked list operations serially
void run_serial_operations() {
    for (int i = 0; i < M; i++) {
        int op_type = rand() % 3;
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (op_type == 0) {
            member_serial(value); // Perform member operation
        } else if (op_type == 1) {
            insert_serial(value); // Perform insert operation
        } else {
            delete_serial(value); // Perform delete operation
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    // Measure time for serial operations
    double serial_time = measure_time(run_serial_operations);
    printf("Time taken for Serial operations: %.6f seconds\n", serial_time);

    // Measure time for mutex operations
    double mutex_time = measure_time(run_mutex_operations);
    printf("Time taken for Mutex operations: %.6f seconds\n", mutex_time);

    // Measure time for read-write lock operations
    double rwlock_time = measure_time(run_rwlock_operations);
    printf("Time taken for Read-Write Lock operations: %.6f seconds\n", rwlock_time);

    return 0;
}

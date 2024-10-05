#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000         // Initial size of the linked list
#define M 10000         // Number of operations
#define THREAD_COUNT 1 // Number of threads to run concurrently
#define SAMPLE_SIZE 385  // Number of times each experiment is run

// Function for timing
double measure_time(void (*func)(void))
{
    clock_t start = clock();
    func();
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Example thread work function for mutex and read-write lock
void *thread_work(void *arg)
{
    int ops = M / THREAD_COUNT;
    for (int i = 0; i < ops; i++)
    {
        int op_type = rand() % 3;
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (op_type == 0)
        {
            member_mutex(value); // Perform member operation
        }
        else if (op_type == 1)
        {
            insert_mutex(value); // Perform insert operation
        }
        else
        {
            delete_mutex(value); // Perform delete operation
        }
    }
    return NULL;
}

// Function to run linked list operations with mutex
double run_mutex_operations()
{
    init_mutex(); // Initialize mutex linked list
    pthread_t threads[THREAD_COUNT];

    clock_t start = clock();

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, thread_work, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Function to run linked list operations with read-write locks
double run_rwlock_operations()
{
    init_rwlock(); // Initialize read-write lock linked list
    pthread_t threads[THREAD_COUNT];

    clock_t start = clock();

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, thread_work, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Function to run linked list operations serially
double run_serial_operations()
{
    clock_t start = clock();

    for (int i = 0; i < M; i++)
    {
        int op_type = rand() % 3;
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (op_type == 0)
        {
            member_serial(value); // Perform member operation
        }
        else if (op_type == 1)
        {
            insert_serial(value); // Perform insert operation
        }
        else
        {
            delete_serial(value); // Perform delete operation
        }
    }
    clock_t end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    double total_serial_time = 0.0;
    double total_mutex_time = 0.0;
    double total_rwlock_time = 0.0;

    // Serial operations loop
    printf("Running Serial operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        total_serial_time += run_serial_operations();
        printf("\rProgress: %d/%d", i + 1, SAMPLE_SIZE);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // // Mutex operations loop
    // printf("Running Mutex operations...\n");
    // for (int i = 0; i < SAMPLE_SIZE; i++)
    // {
    //     total_mutex_time += run_mutex_operations();
    //     printf("\rProgress: %d/%d", i + 1, SAMPLE_SIZE);
    //     fflush(stdout); // Force the output to be updated
    // }
    // printf("\n");

    // // Read-Write Lock operations loop
    // printf("Running Read-Write Lock operations...\n");
    // for (int i = 0; i < SAMPLE_SIZE; i++)
    // {
    //     total_rwlock_time += run_rwlock_operations();
    //     printf("\rProgress: %d/%d", i + 1, SAMPLE_SIZE);
    //     fflush(stdout); // Force the output to be updated
    // }
    // printf("\n");

    // Calculate and print average times
    double avg_serial_time = total_serial_time / SAMPLE_SIZE;
    double avg_mutex_time = total_mutex_time / SAMPLE_SIZE;
    double avg_rwlock_time = total_rwlock_time / SAMPLE_SIZE;

    printf("Average time taken for Serial operations: %.6f seconds\n", avg_serial_time);
    printf("Average time taken for Mutex operations: %.6f seconds\n", avg_mutex_time);
    printf("Average time taken for Read-Write Lock operations: %.6f seconds\n", avg_rwlock_time);

    return 0;
}

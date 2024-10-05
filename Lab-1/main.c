#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 1000         // Initial size of the linked list
#define M 10000        // Number of operations
#define M_member 0.005 // Member operation probability
#define M_insert 0.8   // Insert operation probability
#define M_delete 0.195 // Delete operation probability
#define THREAD_COUNT 2 // Number of threads to run concurrently
#define SAMPLE_SIZE 50 // Number of times each experiment is run

// TODO: prevent duplication of random numbers in the linked list at any given point of time

// Example thread work function for mutex and read-write lock
void *thread_work_mutex(void *arg)
{
    // Calculate number of operations for each type
    int member_ops = M * 0.99;  // 99% of the total operations
    int insert_ops = M * 0.005; // 0.5% of the total operations
    int delete_ops = M * 0.005; // 0.5% of the total operations

    // Total operations for this thread
    int ops_per_thread = M / THREAD_COUNT;

    // Create an array to hold all operations (0 = member, 1 = insert, 2 = delete)
    int *operations = malloc(ops_per_thread * sizeof(int));
    int index = 0;

    // Fill array with exact number of each operation
    for (int i = 0; i < member_ops / THREAD_COUNT; i++)
        operations[index++] = 0; // 0 represents member

    for (int i = 0; i < insert_ops / THREAD_COUNT; i++)
        operations[index++] = 1; // 1 represents insert

    for (int i = 0; i < delete_ops / THREAD_COUNT; i++)
        operations[index++] = 2; // 2 represents delete

    // Shuffle the array to randomize the order of operations
    for (int i = 0; i < ops_per_thread; i++)
    {
        int j = rand() % ops_per_thread;
        int temp = operations[i];
        operations[i] = operations[j];
        operations[j] = temp;
    }

    // Perform the operations based on the shuffled array
    for (int i = 0; i < ops_per_thread; i++)
    {
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (operations[i] == 0)
        {
            member_mutex(value);
        }
        else if (operations[i] == 1)
        {
            insert_mutex(value);
        }
        else
        {
            delete_mutex(value);
        }
    }

    free(operations);
    return NULL;
}

// Example thread work function for read-write lock
void *thread_work_rwlock(void *arg)
{
    // Calculate number of operations for each type
    int member_ops = M * 0.99;  // 99% of the total operations
    int insert_ops = M * 0.005; // 0.5% of the total operations
    int delete_ops = M * 0.005; // 0.5% of the total operations

    // Total operations for this thread
    int ops_per_thread = M / THREAD_COUNT;

    // Create an array to hold all operations (0 = member, 1 = insert, 2 = delete)
    int *operations = malloc(ops_per_thread * sizeof(int));
    int index = 0;

    // Fill array with exact number of each operation
    for (int i = 0; i < member_ops / THREAD_COUNT; i++)
        operations[index++] = 0; // 0 represents member

    for (int i = 0; i < insert_ops / THREAD_COUNT; i++)
        operations[index++] = 1; // 1 represents insert

    for (int i = 0; i < delete_ops / THREAD_COUNT; i++)
        operations[index++] = 2; // 2 represents delete

    // Shuffle the array to randomize the order of operations
    for (int i = 0; i < ops_per_thread; i++)
    {
        int j = rand() % ops_per_thread;
        int temp = operations[i];
        operations[i] = operations[j];
        operations[j] = temp;
    }

    // Perform the operations based on the shuffled array
    for (int i = 0; i < ops_per_thread; i++)
    {
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (operations[i] == 0)
        {
            member_rwlock(value);
        }
        else if (operations[i] == 1)
        {
            insert_rwlock(value);
        }
        else
        {
            delete_rwlock(value);
        }
    }

    free(operations);
    return NULL;
}

// Function to run linked list operations with mutex
double run_mutex_operations()
{
    init_mutex(); // Initialize mutex linked list
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < N; i++)
    {
        int value = rand() % 65536; // Value between 0 and 2^16-1
        insert_mutex(value);        // Insert N elements into the linked list
    }

    clock_t start = clock();

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, thread_work_mutex, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    free_list_mutex(); // Free the linked list
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Function to run linked list operations with read-write locks
double run_rwlock_operations()
{
    init_rwlock(); // Initialize read-write lock linked list
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < N; i++)
    {
        int value = rand() % 65536; // Value between 0 and 2^16-1
        insert_rwlock(value);       // Insert N elements into the linked list
    }

    clock_t start = clock();

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, thread_work_rwlock, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    free_list_rwlock(); // Free the linked list
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Function to run linked list operations serially
double run_serial_operations()
{
    for (int i = 0; i < N; i++)
    {
        int value = rand() % 65536; // Value between 0 and 2^16-1
        insert_serial(value);       // Insert N elements into the linked list
    }

    // Calculate number of operations for each type
    int member_ops = M * 0.99;  // 99% of the total operations
    int insert_ops = M * 0.005; // 0.5% of the total operations
    int delete_ops = M * 0.005; // 0.5% of the total operations

    // Create an array to hold all operations (0 = member, 1 = insert, 2 = delete)
    int *operations = malloc(M * sizeof(int));
    int index = 0;

    // Fill array with exact number of each operation
    for (int i = 0; i < member_ops; i++)
        operations[index++] = 0; // 0 represents member

    for (int i = 0; i < insert_ops; i++)
        operations[index++] = 1; // 1 represents insert

    for (int i = 0; i < delete_ops; i++)
        operations[index++] = 2; // 2 represents delete

    // Shuffle the array to randomize the order of operations
    for (int i = 0; i < M; i++)
    {
        int j = rand() % M;
        int temp = operations[i];
        operations[i] = operations[j];
        operations[j] = temp;
    }

    clock_t start = clock();
    
    // Perform the operations based on the shuffled array
    for (int i = 0; i < M; i++)
    {
        int value = rand() % 65536; // Value between 0 and 2^16-1

        if (operations[i] == 0)
        {
            member_serial(value); // Perform member operation
        }
        else if (operations[i] == 1)
        {
            insert_serial(value); // Perform insert operation
        }
        else
        {
            delete_serial(value); // Perform delete operation
        }
    }

    clock_t end = clock();
    free(operations); // Free the allocated memory

    free_list_serial(); // Free the linked list
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    double total_serial_time = 0.0;
    double total_mutex_time = 0.0;
    double total_rwlock_time = 0.0;
    double temp = 0.0;

    // Serial operations loop
    printf("Running Serial operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        temp = run_serial_operations();
        total_serial_time += temp;
        printf("\rProgress: %d/%d -- %.6f seconds", i + 1, SAMPLE_SIZE, temp);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // Mutex operations loop
    printf("Running Mutex operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        temp = run_mutex_operations();
        total_mutex_time += temp;
        printf("\rProgress: %d/%d -- %.6f seconds", i + 1, SAMPLE_SIZE, temp);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // Read-Write Lock operations loop
    printf("Running Read-Write Lock operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        temp = run_rwlock_operations();
        total_rwlock_time += temp;
        printf("\rProgress: %d/%d -- %.6f seconds", i + 1, SAMPLE_SIZE, temp);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // Calculate and print average times
    double avg_serial_time = total_serial_time / SAMPLE_SIZE;
    double avg_mutex_time = total_mutex_time / SAMPLE_SIZE;
    double avg_rwlock_time = total_rwlock_time / SAMPLE_SIZE;

    printf("Average time taken for Serial operations: %.6f seconds\n", avg_serial_time);
    printf("Average time taken for Mutex operations: %.6f seconds\n", avg_mutex_time);
    printf("Average time taken for Read-Write Lock operations: %.6f seconds\n", avg_rwlock_time);

    return 0;
}

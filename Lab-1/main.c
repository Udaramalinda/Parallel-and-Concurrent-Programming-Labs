#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define N 1000              // Initial size of the linked list
#define M 10000             // Number of operations
#define SAMPLE_SIZE 40      // Number of times each experiment is run

// TODO: prevent duplication of random numbers in the linked list at any given point of time

// Function prototypes
void generate_unique_values(int *values, int *initial_values);
void generate_operation_order(int *operations, double M_member, double M_insert, double M_delete);
double calculate_standard_deviation(double times[], int sample_size, double mean);

typedef struct
{
    int *operations;
    int *values;        // Pre-generated random values
    int ops_per_thread; // Number of operations per thread
} thread_args_t;

// Example thread work function for mutex
void *thread_work_mutex(void *arg)
{
    // Cast the argument to thread_args_t*
    thread_args_t *args = (thread_args_t *)arg;
    int *operations = args->operations;
    int *values = args->values;
    int ops_per_thread = args->ops_per_thread;

    // Perform the operations based on the pre-made operations array and random values array
    for (int i = 0; i < ops_per_thread; i++)
    {
         // Use the pre-generated random value

        if (operations[i] == 0)
        {
            int value = rand() % 65536;
            member_mutex(value);
        }
        else if (operations[i] == 1)
        {
            int value = values[i];
            insert_mutex(value);
        }
        else
        {
            int value = rand() % 65536;
            delete_mutex(value);
        }
    }

    return NULL;
}

// Function to run linked list operations with mutex
double run_mutex_operations(double M_member, double M_insert, double M_delete, int PNC_THREAD_COUNT)
{
    init_mutex(); // Initialize mutex linked list
    pthread_t threads[PNC_THREAD_COUNT];
    thread_args_t thread_args[PNC_THREAD_COUNT];

    int total_ops = M; // Total number of operations (M)
    int ops_per_thread = total_ops / PNC_THREAD_COUNT;

    // Create an array to hold all operations (0 = member, 1 = insert, 2 = delete)
    int *operations = malloc(M * sizeof(int));
    int *values = malloc(M * sizeof(int)); // Array to hold random values
    int *initial_values = malloc(N * sizeof(int)); // Array to hold initial values

    generate_unique_values(values, initial_values);
    generate_operation_order(operations, M_member, M_insert, M_delete);

    // Prepopulate the linked list with N elements
    for (int i = 0; i < N; i++)
    {
        int value = initial_values[i]; // Value between 0 and 2^16-1
        insert_mutex(value);        // Insert N elements into the linked list
    }

    clock_t start = clock();

    // Create threads and pass a subset of operations and values to each
    for (int i = 0; i < PNC_THREAD_COUNT; i++)
    {
        // Set the operations and operations per thread for each thread
        thread_args[i].operations = operations + i * ops_per_thread;
        thread_args[i].values = values + i * ops_per_thread; // Pass the random values
        thread_args[i].ops_per_thread = ops_per_thread;

        pthread_create(&threads[i], NULL, thread_work_mutex, (void *)&thread_args[i]);
    }

    // Join threads
    for (int i = 0; i < PNC_THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    free(operations);  // Free the operations array
    free(values);      // Free the random values array
    free(initial_values); // Free the initial values array
    free_list_mutex(); // Free the linked list
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

void *thread_work_rwlock(void *arg)
{
    // Cast the argument to thread_args_t*
    thread_args_t *args = (thread_args_t *)arg;
    int *operations = args->operations;
    int *values = args->values;
    int ops_per_thread = args->ops_per_thread;

    // Perform the operations based on the pre-made operations array and random values array
    for (int i = 0; i < ops_per_thread; i++)
    {

        if (operations[i] == 0)
        {
            int value = rand() % 65536; // Random value between 0 and 2^16-1
            member_rwlock(value);
        }
        else if (operations[i] == 1)
        {
            int value = values[i]; // Use the pre-generated random value
            insert_rwlock(value);
        }
        else
        {
            int value = rand() % 65536; // Random value between 0 and 2^16-1
            delete_rwlock(value);
        }
    }

    return NULL;
}

// Function to run linked list operations with read-write locks
double run_rwlock_operations(double M_member, double M_insert, double M_delete, int PNC_THREAD_COUNT)
{
    init_rwlock(); // Initialize read-write lock linked list
    pthread_t threads[PNC_THREAD_COUNT];
    thread_args_t thread_args[PNC_THREAD_COUNT];

    int total_ops = M; // Total number of operations (M)
    int ops_per_thread = total_ops / PNC_THREAD_COUNT;

    // Create an array to hold all operations (0 = member, 1 = insert, 2 = delete)
    int *operations = malloc(M * sizeof(int));
    int *values = malloc(M * sizeof(int)); // Array to hold random values
    int *initial_values = malloc(N * sizeof(int)); // Array to hold initial values
    
    generate_unique_values(values, initial_values);
    generate_operation_order(operations, M_member, M_insert, M_delete);

    // Prepopulate the linked list with N elements
    for (int i = 0; i < N; i++)
    {
        int value = initial_values[i]; // Value between 0 and 2^16-1
        insert_rwlock(value);       // Insert N elements into the linked list
    }

    clock_t start = clock();

    // Create threads and pass a subset of operations and values to each
    for (int i = 0; i < PNC_THREAD_COUNT; i++)
    {
        // Set the operations and operations per thread for each thread
        thread_args[i].operations = operations + i * ops_per_thread;
        thread_args[i].values = values + i * ops_per_thread; // Pass the random values
        thread_args[i].ops_per_thread = ops_per_thread;

        pthread_create(&threads[i], NULL, thread_work_rwlock, (void *)&thread_args[i]);
    }

    // Join threads
    for (int i = 0; i < PNC_THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();

    free(operations);   // Free the operations array
    free(values);       // Free the random values array
    free(initial_values); // Free the initial values array
    free_list_rwlock(); // Free the linked list
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Function to run linked list operations serially
double run_serial_operations(double M_member, double M_insert, double M_delete)
{
    int *operations = malloc(M * sizeof(int)); // Array to hold all operations (0 = member, 1 = insert, 2 = delete)
    int *values = malloc(M * sizeof(int));     // Array to hold random values
    int *initial_values = malloc(N * sizeof(int)); // Array to hold initial values

    generate_unique_values(values, initial_values);
    generate_operation_order(operations, M_member, M_insert, M_delete);

    for (int i = 0; i < N; i++)
    {
        int value = initial_values[i];
        insert_serial(value);
    }

    clock_t start = clock();

    // Perform the operations based on the shuffled array
    for (int i = 0; i < M; i++)
    {

        if (operations[i] == 0)
        {
            int value = rand() % 65536; // Random value between 0 and 2^16-1
            member_serial(value); // Perform member operation
        }
        else if (operations[i] == 1)
        {
            int value = values[i];
            insert_serial(value); // Perform insert operation
        }
        else
        {
            int value = rand() % 65536;
            delete_serial(value); // Perform delete operation
        }
    }

    clock_t end = clock();

    free(operations); // Free the allocated memory
    free(values);     // Free the allocated memory
    free(initial_values); // Free the allocated memory

    free_list_serial(); // Free the linked list
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

void generate_unique_values(int *values, int *initial_values)
{
    bool used[65536] = {false};
    for (int i = 0; i < M; i++)
    {
        int value = rand() % 65536;
        while (used[value])
        {
            value = rand() % 65536;
        }
        values[i] = value;
        used[value] = true;
    }

    for (int i = 0; i < N; i++)
    {
        int value = rand() % 65536;
        while (used[value])
        {
            value = rand() % 65536;
        }
        initial_values[i] = value;
        used[value] = true;
    }
}

void generate_operation_order(int *operations, double M_member, double M_insert, double M_delete)
{
    // Calculate number of operations for each type
    int member_ops = M * M_member;
    int insert_ops = M * M_insert;
    int delete_ops = M * M_delete;

    // Fill array with exact number of each operation
    int index = 0;
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
}


double calculate_standard_deviation(double times[], int sample_size, double mean)
{
    double sum_squared_diff = 0.0;
    for (int i = 0; i < sample_size; i++)
    {
        double diff = times[i] - mean;
        sum_squared_diff += diff * diff;
    }
    return sqrt(sum_squared_diff / sample_size);
}


int main(int argc, char *argv[])
{
    if (argc < 5) {
        printf("Usage: %s <M_member> <M_insert> <M_delete> <THREAD_COUNT>\n", argv[0]);
        return 1;
    }

    double M_member = atof(argv[1]);
    double M_insert = atof(argv[2]);
    double M_delete = atof(argv[3]);
    int PNC_THREAD_COUNT = atoi(argv[4]);

    srand(time(NULL));

    double serial_times[SAMPLE_SIZE];
    double mutex_times[SAMPLE_SIZE];
    double rwlock_times[SAMPLE_SIZE];
    double temp = 0.0;

    double total_serial_time = 0.0;
    double total_mutex_time = 0.0;
    double total_rwlock_time = 0.0;

    // Serial operations loop
    printf("Running Serial operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        temp = run_serial_operations(M_member, M_insert, M_delete);
        serial_times[i] = temp;
        total_serial_time += temp;
        printf("\rProgress: %d/%d -- %.6f seconds", i + 1, SAMPLE_SIZE, temp);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // Mutex operations loop
    printf("Running Mutex operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        temp = run_mutex_operations(M_member, M_insert, M_delete, PNC_THREAD_COUNT);
        mutex_times[i] = temp;
        total_mutex_time += temp;
        printf("\rProgress: %d/%d -- %.6f seconds", i + 1, SAMPLE_SIZE, temp);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // Read-Write Lock operations loop
    printf("Running Read-Write Lock operations...\n");
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        temp = run_rwlock_operations(M_member, M_insert, M_delete, PNC_THREAD_COUNT);
        rwlock_times[i] = temp;
        total_rwlock_time += temp;
        printf("\rProgress: %d/%d -- %.6f seconds", i + 1, SAMPLE_SIZE, temp);
        fflush(stdout); // Force the output to be updated
    }
    printf("\n");

    // Calculate average times
    double avg_serial_time = total_serial_time / SAMPLE_SIZE;
    double avg_mutex_time = total_mutex_time / SAMPLE_SIZE;
    double avg_rwlock_time = total_rwlock_time / SAMPLE_SIZE;

    // Calculate standard deviations
    double stddev_serial_time = calculate_standard_deviation(serial_times, SAMPLE_SIZE, avg_serial_time);
    double stddev_mutex_time = calculate_standard_deviation(mutex_times, SAMPLE_SIZE, avg_mutex_time);
    double stddev_rwlock_time = calculate_standard_deviation(rwlock_times, SAMPLE_SIZE, avg_rwlock_time);

    // Print results
    printf("Average time taken for Serial operations: %.6f seconds\n", avg_serial_time);
    printf("Standard deviation for Serial operations: %.6f seconds\n", stddev_serial_time);

    printf("Average time taken for Mutex operations: %.6f seconds\n", avg_mutex_time);
    printf("Standard deviation for Mutex operations: %.6f seconds\n", stddev_mutex_time);

    printf("Average time taken for Read-Write Lock operations: %.6f seconds\n", avg_rwlock_time);
    printf("Standard deviation for Read-Write Lock operations: %.6f seconds\n", stddev_rwlock_time);

    return 0;
}
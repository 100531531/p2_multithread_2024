//SSOO-P3 23/24

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <pthread.h>
#include "queue.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>



// struct for the operation data thats in the ARRAY
// typedef struct Operation {
//     int product_id;
//     char operation_type[9]; // PURCHASE or SALE
//     int units;
// } Operation;

// put the producer args in a struct to reduce complexity
typedef struct {
    queue *q;
    element *operations;
    int start_proc_prod;
    int end_proc_prod;
    int start_proc_cons;
    int end_proc_cons;
    int *product_stock;
    int *profits;
} ThreadArgs;


// the func exec by each 
void *producer_thread(void *args) {
    ThreadArgs *arguments = (ThreadArgs *)args;
    queue *q = arguments->q;
    element *operations = arguments->operations;
    int start_proc_prod = arguments->start_proc_prod;
    int end_proc_prod = arguments->end_proc_prod;

    for (int i = start_proc_prod; i < end_proc_prod; i++) {
        queue_put(q, &operations[i]);
    }
    
    return NULL;
}

void *consumer_thread(void *args) {
    ThreadArgs *arguments = (ThreadArgs *)args;
    queue *q = arguments->q;
    element *operations = arguments->operations;
    int start_proc_cons = arguments->start_proc_cons;
    int end_proc_cons = arguments->end_proc_cons;
    int *product_stock = arguments->product_stock;
    int *profits = arguments->profits; // Get the pointer to the profits variable
    int product_cost [5] = {2,5,15,25,100};
    int sale_price [5] = {3,10,20,40,125};

    for (int i = start_proc_cons; i < end_proc_cons; i++) {
        queue_get(q);

        if (strcmp(operations[i].op, "PURCHASE") == 0) {
            product_stock[(operations[i].product_id) - 1] += operations[i].units;
            *profits -= ((product_cost[(operations[i].product_id) - 1]) * operations[i].units);
        }
        else if (strcmp(operations[i].op, "SALE") == 0) {
            product_stock[(operations[i].product_id) - 1] -= operations[i].units;
            *profits += ((sale_price[(operations[i].product_id) - 1]) * operations[i].units);
            // printf("%ls", profits);
        }
    }

    return NULL;
}




int main (int argc, const char * argv[])
{
    int profits = 0;
    int product_stock [5] = {0,0,0,0,0};

    // Create ThreadArgs struct for consumer thread
        

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <file name> <num producers> <num consumers> <buff size>\n", argv[0]);
        return 1;
    }

    // get the command line arguments
    const char *file_name = argv[1];
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    int buff_size = atoi(argv[4]);

    // creating the queue of designated size
    queue *q = queue_init(buff_size);


    // Open the file
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // gets the num of operations to read from the file
    int num_operations;
    if (fscanf(file, "%d", &num_operations) != 1) {
        fprintf(stderr, "Error reading number of operations from file\n");
        fclose(file);
        return 1;
    }

    // ************************************************************ //
    // should there be a check for the number of operations?
    // ************************************************************ //

    // creates an array of num_operations that will contain struct operations
    element *operations = malloc(num_operations * sizeof(element));
    if (operations == NULL) {
        perror("Error allocating memory for operations");
        fclose(file);
        return 1;
    }



    // for (int i = 0; i < num_operations; i++) {
    //     printf("Operation %d: product_id = %d, op = %d, units = %d\n", i + 1, operations[i].product_id, operations[i].op, operations[i].units);
    // }


    // reads the operations from file and stores them in the operations array
    for (int i = 0; i < num_operations; i++) {
        if (fscanf(file, "%d %8s %d", &operations[i].product_id, operations[i].op, &operations[i].units) != 3) {
            fprintf(stderr, "Error reading operation %d from file\n", i + 1);
            free(operations);
            fclose(file);
            return 1;
        }
    }

    // each producer will do num_operations/num_producers(floor division), 
    // but last producer will do num_ops - the floor division number
    int ops_per_prod_thread = num_operations / num_producers;

    // Create arguments for each producer thread
    ThreadArgs producer_args[num_producers];
    pthread_t producer_threads[num_producers];

    // execute the producers threads
    for (int i = 0; i < num_producers; i++) {
        // start idx for each prod thread
        int start_idx = i * ops_per_prod_thread;
        // end idx for each prod thread and incls the last thread to handle to the end
        // 2nd ternary logic, just uses the supposed next startidx as the end
        int end_idx = (i == num_producers - 1) ? num_operations : (i + 1) * ops_per_prod_thread;
        ThreadArgs args = {q, operations, start_idx, end_idx, 0, 0};
        producer_args[i] = args;
        pthread_create(&producer_threads[i], NULL, producer_thread, &producer_args[i]);
    }


    
    // each consumer will do num_operations/num_consumers(floor division), 
    // but last producer will do num_ops - the floor division number
    int ops_per_con_thread = num_operations / num_consumers;

    // Create arguments for each producer thread
    ThreadArgs consumer_args[num_consumers];
    pthread_t consumer_threads[num_consumers];

    // execute the producers threads
    for (int i = 0; i < num_consumers; i++) {
        // start idx for each prod thread
        int start_idx = i * ops_per_con_thread;
        // end idx for each prod thread and incls the last thread to handle to the end
        // 2nd ternary logic, just uses the supposed next startidx as the end
        int end_idx = (i == num_consumers - 1) ? num_operations : (i + 1) * ops_per_con_thread;
        
        ThreadArgs args = {q, operations, 0, 0, start_idx, end_idx}; 
        consumer_args[i] = args;
        consumer_args[i].product_stock = product_stock; // Pass a pointer to product_stock
        consumer_args[i].profits = &profits; // Pass the address of profits

        pthread_create(&consumer_threads[i], NULL, consumer_thread, (void *)&consumer_args[i]);
    }


    // Join producer threads
    for (int i = 0; i < num_producers; i++) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < num_consumers; i++) {
        pthread_join(consumer_threads[i], NULL);
    }


    // pthread_exit to ensure main thread waits for all other threads to finish
    

    // Output
    printf("Total: %d euros\n", profits);
    printf("Stock:\n");
    printf("  Product 1: %d\n", product_stock[0]);
    printf("  Product 2: %d\n", product_stock[1]);
    printf("  Product 3: %d\n", product_stock[2]);
    printf("  Product 4: %d\n", product_stock[3]);
    printf("  Product 5: %d\n", product_stock[4]);

    pthread_exit(NULL);
    return 0;
}

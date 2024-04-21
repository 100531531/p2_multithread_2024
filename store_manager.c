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

#define MAX_OPERATIONS 1000 // Maximum number of operations to process
#define MAX_PRODUCTS 5 // Maximum number of products

// Structure to store operation data
struct Operation {
    int product_id;
    char operation_type[9]; // PURCHASE or SALE
    int units;
};

int main(int argc, const char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <file name> <num producers> <num consumers> <buff size>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    const char *file_name = argv[1];
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    int buff_size = atoi(argv[4]);

    // Validate buffer size
    if (buff_size < 1 || buff_size > MAX_OPERATIONS) {
        fprintf(stderr, "Buffer size must be between 1 and %d\n", MAX_OPERATIONS);
        return 1;
    }

    // Open the file
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read number of operations from the file
    int num_operations;
    if (fscanf(file, "%d", &num_operations) != 1) {
        fprintf(stderr, "Error reading number of operations from file\n");
        fclose(file);
        return 1;
    }

    // Validate number of operations
    if (num_operations < 1 || num_operations > MAX_OPERATIONS) {
        fprintf(stderr, "Number of operations in file must be between 1 and %d\n", MAX_OPERATIONS);
        fclose(file);
        return 1;
    }

    // Allocate memory to store operations
    struct Operation *operations = malloc(num_operations * sizeof(struct Operation));
    if (operations == NULL) {
        perror("Error allocating memory for operations");
        fclose(file);
        return 1;
    }

    // Read operations from file
    for (int i = 0; i < num_operations; i++) {
        if (fscanf(file, "%d %8s %d", &operations[i].product_id, operations[i].operation_type, &operations[i].units) != 3) {
            fprintf(stderr, "Error reading operation %d from file\n", i + 1);
            free(operations);
            fclose(file);
            return 1;
        }
    }

    // Close the file
    fclose(file);

    // Output for testing
    printf("File: %s\n", file_name);
    printf("Num producers: %d\n", num_producers);
    printf("Num consumers: %d\n", num_consumers);
    printf("Buffer size: %d\n", buff_size);
    printf("Num operations: %d\n", num_operations);
    for (int i = 0; i < num_operations; i++) {
        printf("Operation %d: Product ID: %d, Type: %s, Units: %d\n", i + 1, operations[i].product_id, operations[i].operation_type, operations[i].units);
    }

    // Free allocated memory
    free(operations);

    return 0;
}


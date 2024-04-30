// queue.c

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

// To create a queue
queue* queue_init(int num_elements) {
    queue *q = (queue *)malloc(sizeof(queue));

    if (q == NULL) {
        // in the case it fails
        return NULL;
    }

    q->size = 0;
    q->cap = num_elements; // Set the capacity of the queue
    q->head = 0;       // Initialize the front index
    q->tail = -1;       // Initialize the rear index to -1 (no elements)
    q->array = (struct element *)malloc(num_elements * sizeof(struct element));
    if (q->array == NULL) {
        // in the case of mem alloc fails
        free(q); // free any mem
        return NULL;
    }

    // init the mutex as this is a shared resource
    if (pthread_mutex_init(&q->mutex, NULL) != 0) {
        free(q->array);
        free(q);
        return NULL;
    }

    // init the cond vars
    if (pthread_cond_init(&q->not_full, NULL) != 0 || pthread_cond_init(&q->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&q->mutex);
        free(q);
        perror("Failed to initialize condition variables");
        return NULL;
    }


    return q;
}

// To Enqueue an element
int queue_put(queue *q, struct element *x) {

    pthread_mutex_lock(&q->mutex); // lock the mutex, q is being added to

    // must wait until there is space available in the queue
    while (queue_full(q)) {
        // Wait on the condition variable until notified that space is available
        pthread_cond_wait(&q->not_full, &q->mutex);
    }

    // Increment tail index and wrap around if necessary
    q->tail = (q->tail + 1) % q->cap;
    q->array[q->tail] = *x;
    q->size++;

    // signal the queue is not empty
    pthread_cond_signal(&q->not_empty);

    // unlock the mutex
    pthread_mutex_unlock(&q->mutex);

    return 0; // Success
}

// To Dequeue an element.
struct element* queue_get(queue *q) {
    if (queue_empty(q)) {
        return NULL; // Queue is empty
    }

    struct element *element = &(q->array[q->head]);
    q->head = (q->head + 1) % q->cap;
    q->size--;

    return element;
}

// To check if the queue is empty
int queue_empty(queue *q) {
    return (q->size == 0);
}


// To check if the queue is full
int queue_full(queue *q) {
    return (q->size == q->cap);
}

int queue_not_full(queue *q) {
    return !(q->size == q->cap);
}

// To destroy the queue and free the resources
int queue_destroy(queue *q) {
    if (q == NULL) {
        return -1; // Queue is already destroyed
    }

    free(q->array);
    free(q);
    return 0;
}


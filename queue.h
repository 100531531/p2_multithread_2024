// queue.h
#include <pthread.h>

#ifndef QUEUE_H
#define QUEUE_H


// Structure representing an operation element
typedef struct element {
    int product_id; // Product identifier
    int op;         // Operation
    int units;      // Product units
} element;

typedef struct queue {
    int size;            // Current size of the queue
    int cap;             // Maximum capacity of the queue
    int head;            // Index of the first element
    int tail;            // Index of the last element
    struct element *array; // The array holding queue elements
    pthread_mutex_t mutex; // Mutex for thread safety
    pthread_cond_t not_full;     // Condition variable to signal when the queue is not full
    pthread_cond_t not_empty;    // Condition variable to signal when the queue is not empty
} queue;

queue* queue_init(int num_elements);
int queue_destroy(queue *q);
int queue_put(queue *q, struct element *elem);
struct element* queue_get(queue *q);
int queue_empty(queue *q);
int queue_full(queue *q);

#endif

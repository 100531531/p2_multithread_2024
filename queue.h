// queue.h

#ifndef QUEUE_H
#define QUEUE_H

struct element {
    int product_id; // Product identifier
    int op;         // Operation
    int units;      // Product units
};

typedef struct queue {
    int size;            // Current size of the queue
    int cap;             // Maximum capacity of the queue
    int head;            // Index of the first element
    int tail;            // Index of the last element
    struct element *array; // The array holding queue elements
} queue;

queue* queue_init(int num_elements);
int queue_destroy(queue *q);
int queue_put(queue *q, struct element *elem);
struct element* queue_get(queue *q);
int queue_empty(queue *q);
int queue_full(queue *q);

#endif

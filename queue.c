//SSOO-P3 23/24

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "queue.h"

//To create a queue
queue* queue_init(int  num_elements)
{
  queue * q = (queue *)malloc(sizeof(queue));

    if (q == NULL) {
        // in the case it fails
        return NULL;
    }

    q->size = 0;
    q->array = (struct element *)malloc(num_elements * sizeof(struct element));

  if (q->array == NULL) {
        // in the case of mem alloc fails
        free(q); // free any mem
        return NULL;
    }

    q->head = 0;       // Initialize the front index
    q->tail = -1;       // Initialize the rear index to -1 (no elements)

  return q;
}


// To Enqueue an element
int queue_put(queue *q, struct element* x)
{
  
  
  return 0;
}


// To Dequeue an element.
struct element* queue_get(queue *q)
{
  struct element* element;
  
  return element;
}




//To check queue state
int queue_empty(queue *q)
{
  
  return 0;
}

int queue_full(queue *q)
{
  
  return q->size
}

//To destroy the queue and free the resources
int queue_destroy(queue *q)
{

  return 0;
}

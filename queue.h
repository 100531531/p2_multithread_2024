//SSOO-P3 23/24

#ifndef HEADER_FILE
#define HEADER_FILE


struct element {
  int product_id; //Product identifier
  int op;         //Operation
  int units;      //Product units
};

typedef struct queue {
  int size; // curr size of the queue
  int cap; // max cap of the queue
  int head; // index of first elem
  int tail; // index of last elem
  struct element *array;   // the array holding queue elems
}queue;

queue* queue_init (int  num_elements);
int queue_destroy (queue *q);
int queue_put (queue *q, struct element* elem);
struct element * queue_get(queue *q);
int queue_empty (queue *q);
int queue_full(queue *q);

#endif

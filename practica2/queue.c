#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define MAX_QUEUE 4096

struct _Queue {
  void *data[MAX_QUEUE];
  void **front;
  void **rear;
};

/*----------------------------------------------------------------------------------------*/
/*
Private functions:
*/
static void **queue_nextPosition(const Queue *q, void **position);
static Bool queue_isFull(const Queue *q);

static void **queue_nextPosition(const Queue *q, void **position) {
  if (!q || !position) return NULL;

  if (position == (void **)&q->data[MAX_QUEUE - 1]) return (void **)&q->data[0];

  return position + 1;
}

static Bool queue_isFull(const Queue *q) {
  void **next;

  if (!q || queue_isEmpty(q) == TRUE) return FALSE;

  next = queue_nextPosition(q, q->rear);
  if (!next) return FALSE;

  return (next == q->front) ? TRUE : FALSE;
}

/*----------------------------------------------------------------------------------------*/
Queue *queue_new() {
  Queue *q;
  int i;

  q = (Queue *)malloc(sizeof(Queue));
  if (!q) return NULL;

  for (i = 0; i < MAX_QUEUE; i++) {
    q->data[i] = NULL;
  }

  q->front = NULL;
  q->rear = NULL;

  return q;
}

void queue_free(Queue *q) {
  if (!q) return;

  free(q);
}

Bool queue_isEmpty(const Queue *q) {
  if (!q || !q->front || !q->rear) return TRUE;

  return FALSE;
}

Status queue_push(Queue *q, void *ele) {
  void **next;

  if (!q || !ele) return ERROR;
  if (queue_isFull(q) == TRUE) return ERROR;

  if (queue_isEmpty(q) == TRUE) {
    q->front = (void **)&q->data[0];
    q->rear = (void **)&q->data[0];
    *(q->rear) = ele;
    return OK;
  }

  next = queue_nextPosition(q, q->rear);
  if (!next) return ERROR;

  q->rear = next;
  *(q->rear) = ele;

  return OK;
}

void *queue_pop(Queue *q) {
  void *ele;
  void **next;

  if (!q || queue_isEmpty(q) == TRUE) return NULL;

  ele = *(q->front);
  *(q->front) = NULL;

  if (q->front == q->rear) {
    q->front = NULL;
    q->rear = NULL;
    return ele;
  }

  next = queue_nextPosition(q, q->front);
  if (!next) return NULL;

  q->front = next;

  return ele;
}

void *queue_getFront(const Queue *q) {
  if (!q || queue_isEmpty(q) == TRUE) return NULL;

  return *(q->front);
}

void *queue_getBack(const Queue *q) {
  if (!q || queue_isEmpty(q) == TRUE) return NULL;

  return *(q->rear);
}

size_t queue_size(const Queue *q) {
  if (!q || queue_isEmpty(q) == TRUE) return 0;

  if (q->rear >= q->front) {
    return (size_t)(q->rear - q->front + 1);
  }

  return (size_t)(((void **)&q->data[MAX_QUEUE] - q->front) +
                  (q->rear - (void **)&q->data[0] + 1));
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f) {
  void **current;
  int chars = 0;
  int written;

  if (!fp || !q || !f) return -1;
  if (queue_isEmpty(q) == TRUE) return 0;

  current = q->front;
  while (1) {
    written = f(fp, *current);
    if (written < 0) return -1;
    chars += written;

    written = fprintf(fp, "\n");
    if (written < 0) return -1;
    chars += written;

    if (current == q->rear) break;

    current = queue_nextPosition(q, current);
    if (!current) return -1;
  }

  return chars;
}

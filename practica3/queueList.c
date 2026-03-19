#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "queue.h"

struct _Queue {
  List *pl;
};

Queue *queue_new() {
  Queue *q;

  q = (Queue *)malloc(sizeof(Queue));
  if (!q) return NULL;

  q->pl = list_new();
  if (!q->pl) {
    free(q);
    return NULL;
  }

  return q;
}

void queue_free(Queue *q) {
  if (!q) return;

  list_free(q->pl);
  free(q);
}

Bool queue_isEmpty(const Queue *q) {
  if (!q) return TRUE;

  return list_isEmpty(q->pl);
}

Status queue_push(Queue *q, void *ele) {
  if (!q || !ele) return ERROR;

  return list_pushBack(q->pl, ele);
}

void *queue_pop(Queue *q) {
  if (!q) return NULL;

  return list_popFront(q->pl);
}

void *queue_getFront(const Queue *q) {
  if (!q) return NULL;

  return list_getFront(q->pl);
}

void *queue_getBack(const Queue *q) {
  if (!q) return NULL;

  return list_getBack(q->pl);
}

size_t queue_size(const Queue *q) {
  if (!q) return 0;

  return (size_t)list_size(q->pl);
}

int queue_print(FILE *fp, const Queue *q, p_queue_ele_print f) {
  Queue *aux;
  void *ele;
  size_t i;
  int written;
  int chars = 0;

  if (!fp || !q || !f) return -1;
  if (queue_isEmpty(q) == TRUE) return 0;

  aux = (Queue *)q;
  for (i = 0; i < queue_size(q); i++) {
    ele = queue_pop(aux);
    if (!ele) return -1;

    written = f(fp, ele);
    if (written < 0) return -1;
    chars += written;

    written = fprintf(fp, "\n");
    if (written < 0) return -1;
    chars += written;

    if (queue_push(aux, ele) == ERROR) return -1;
  }

  return chars;
}

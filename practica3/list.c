#include <stdio.h>
#include <stdlib.h>

#include "list.h"

typedef struct _Node Node;

struct _Node {
  void *info;
  struct _Node *next;
};

struct _List {
  Node *first;  /* First node of the list. */
  Node *last;   /* Last node of the list. */
  int size;
};

/*----------------------------------------------------------------------------------------*/
/*
Private functions:
*/
static Node *node_new(const void *e);

static Node *node_new(const void *e) {
  Node *n;

  if (!e) return NULL;

  n = (Node *)malloc(sizeof(Node));
  if (!n) return NULL;

  n->info = (void *)e;
  n->next = NULL;

  return n;
}

/*----------------------------------------------------------------------------------------*/
List *list_new() {
  List *pl;

  pl = (List *)malloc(sizeof(List));
  if (!pl) return NULL;

  pl->first = NULL;
  pl->last = NULL;
  pl->size = 0;

  return pl;
}

Bool list_isEmpty(const List *pl) {
  if (!pl || pl->size == 0) return TRUE;

  return FALSE;
}

Status list_pushFront(List *pl, const void *e) {
  Node *n;

  if (!pl || !e) return ERROR;

  n = node_new(e);
  if (!n) return ERROR;

  n->next = pl->first;
  pl->first = n;

  if (!pl->last) pl->last = n;

  pl->size++;
  return OK;
}

Status list_pushBack(List *pl, const void *e) {
  Node *n;

  if (!pl || !e) return ERROR;

  n = node_new(e);
  if (!n) return ERROR;

  if (pl->last) {
    pl->last->next = n;
  } else {
    pl->first = n;
  }

  pl->last = n;
  pl->size++;

  return OK;
}

void *list_popFront(List *pl) {
  Node *aux;
  void *info;

  if (!pl || list_isEmpty(pl) == TRUE) return NULL;

  aux = pl->first;
  info = aux->info;
  pl->first = aux->next;

  if (!pl->first) pl->last = NULL;

  free(aux);
  pl->size--;

  return info;
}

void *list_popBack(List *pl) {
  Node *aux;
  Node *previous;
  void *info;

  if (!pl || list_isEmpty(pl) == TRUE) return NULL;

  if (pl->first == pl->last) {
    info = pl->first->info;
    free(pl->first);
    pl->first = NULL;
    pl->last = NULL;
    pl->size = 0;
    return info;
  }

  previous = pl->first;
  while (previous->next != pl->last) {
    previous = previous->next;
  }

  aux = pl->last;
  info = aux->info;
  previous->next = NULL;
  pl->last = previous;

  free(aux);
  pl->size--;

  return info;
}

void *list_getFront(List *pl) {
  if (!pl || list_isEmpty(pl) == TRUE) return NULL;

  return pl->first->info;
}

void *list_getBack(List *pl) {
  if (!pl || list_isEmpty(pl) == TRUE) return NULL;

  return pl->last->info;
}

void list_free(List *pl) {
  if (!pl) return;

  while (list_isEmpty(pl) == FALSE) {
    list_popFront(pl);
  }

  free(pl);
}

int list_size(const List *pl) {
  if (!pl) return -1;

  return pl->size;
}

int list_print(FILE *fp, const List *pl, p_list_ele_print f) {
  Node *current;
  int chars = 0;
  int written;

  if (!fp || !pl || !f) return -1;

  current = pl->first;
  while (current) {
    written = f(fp, current->info);
    if (written < 0) return written;
    chars += written;
    current = current->next;
  }

  return chars;
}

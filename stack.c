#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

#define INIT_CAPACITY 2
#define FCT_CAPACITY 2

struct _Stack {
  void **item;   /*!< Dynamic array of elements */
  int top;       /*!< Index of the top element in the stack */
  int capacity;  /*!< Stack capacity */
};

static Bool stack_isFull(const Stack *s);
static Status stack_incrCapacity(Stack *s);

static Bool stack_isFull(const Stack *s) {
  if (!s) return FALSE;

  return (s->top + 1 == s->capacity) ? TRUE : FALSE;
}

static Status stack_incrCapacity(Stack *s) {
  void **tmp;
  int new_capacity;

  if (!s) return ERROR;

  new_capacity = s->capacity * FCT_CAPACITY;
  tmp = (void **)realloc(s->item, sizeof(void *) * new_capacity);
  if (!tmp) return ERROR;

  s->item = tmp;
  s->capacity = new_capacity;

  return OK;
}

Stack *stack_init() {
  Stack *s;

  s = (Stack *)malloc(sizeof(Stack));
  if (!s) return NULL;

  s->item = (void **)malloc(sizeof(void *) * INIT_CAPACITY);
  if (!s->item) {
    free(s);
    return NULL;
  }

  s->top = -1;
  s->capacity = INIT_CAPACITY;

  return s;
}

void stack_free(Stack *s) {
  if (!s) return;

  free(s->item);
  free(s);
}

Status stack_push(Stack *s, const void *ele) {
  if (!s || !ele) return ERROR;

  if (stack_isFull(s) == TRUE && stack_incrCapacity(s) == ERROR) return ERROR;

  s->top++;
  s->item[s->top] = (void *)ele;

  return OK;
}

void *stack_pop(Stack *s) {
  void *ele;

  if (!s || stack_isEmpty(s) == TRUE) return NULL;

  ele = s->item[s->top];
  s->top--;

  return ele;
}

void *stack_top(const Stack *s) {
  if (!s || stack_isEmpty(s) == TRUE) return NULL;

  return s->item[s->top];
}

Bool stack_isEmpty(const Stack *s) {
  if (!s || s->top < 0) return TRUE;

  return FALSE;
}

size_t stack_size(const Stack *s) {
  if (!s) return 0;

  return (size_t)(s->top + 1);
}

int stack_print(FILE *fp, const Stack *s, P_stack_ele_print f) {
  int i;
  int chars = 0;
  int written;

  if (!fp || !s || !f) return -1;

  written = fprintf(fp, "SIZE: %lu\n", (unsigned long)stack_size(s));
  if (written < 0) return -1;
  chars += written;

  for (i = s->top; i >= 0; i--) {
    written = f(fp, s->item[i]);
    if (written < 0) return -1;
    chars += written;

    written = fprintf(fp, "\n");
    if (written < 0) return -1;
    chars += written;
  }

  return chars;
}

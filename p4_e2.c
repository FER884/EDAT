#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bstree.h"
#include "list.h"
#include "radio.h"
#include "types.h"

static void cleanUp(int ret_value, Radio *r, FILE *pf, BSTree *t, List *list) {
  if (list) {
    list_free(list);
  }

  if (t) {
    tree_destroy(t);
  }

  if (r) {
    radio_free(r);
  }

  if (pf) {
    fclose(pf);
  }

  exit(ret_value);
}

static Status parseLong(const char *str, long *value) {
  char *end;
  long number;

  if (!str || !value) {
    return ERROR;
  }

  number = strtol(str, &end, 10);
  if (end == str || *end != '\0' || number < 0) {
    return ERROR;
  }

  *value = number;

  return OK;
}

static FILE *openInputFile(const char *filename) {
  FILE *f;
  char alt_name[1024];
  size_t len;

  if (!filename) {
    return NULL;
  }

  f = fopen(filename, "r");
  if (f) {
    return f;
  }

  len = strlen(filename);
  if (len >= 4 && strcmp(filename + len - 4, ".txt") == 0) {
    return NULL;
  }

  if (len + 4 >= sizeof(alt_name)) {
    return NULL;
  }

  sprintf(alt_name, "%s.txt", filename);

  return fopen(alt_name, "r");
}

static Music *findMusicById(const Radio *r, long music_id) {
  Music *m;
  int i, total;

  if (!r || music_id < 0) {
    return NULL;
  }

  total = radio_getNumberOfMusic(r);
  for (i = 0; i < total; i++) {
    m = radio_getMusicAt(r, i);
    if (m && music_getId(m) == music_id) {
      return m;
    }
  }

  return NULL;
}

static BSTree *loadTreeFromRadio(const Radio *r) {
  BSTree *t;
  Music *m;
  int i, total;

  if (!r) {
    return NULL;
  }

  t = tree_init(music_plain_print, music_cmp);
  if (!t) {
    return NULL;
  }

  total = radio_getNumberOfMusic(r);
  for (i = 0; i < total; i++) {
    m = radio_getMusicAt(r, i);
    if (!m || tree_insert(t, m) == ERROR) {
      tree_destroy(t);
      return NULL;
    }
  }

  return t;
}

int main(int argc, char *argv[]) {
  FILE *f_in = NULL;
  Radio *r = NULL;
  BSTree *t = NULL;
  List *list = NULL;
  Music *min_music = NULL, *max_music = NULL;
  long min_id, max_id;

  if (argc != 4) {
    printf("Usage: %s music_file min_id max_id\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (parseLong(argv[2], &min_id) == ERROR ||
      parseLong(argv[3], &max_id) == ERROR || min_id > max_id) {
    printf("Incorrect id range\n");
    return EXIT_FAILURE;
  }

  f_in = openInputFile(argv[1]);
  if (!f_in) {
    printf("Error opening input file: %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  r = radio_init();
  if (!r) {
    cleanUp(EXIT_FAILURE, r, f_in, t, list);
  }

  if (radio_readFromFile(f_in, r) == ERROR) {
    printf("Not file or File format incorrect\n");
    cleanUp(EXIT_FAILURE, r, f_in, t, list);
  }

  min_music = findMusicById(r, min_id);
  max_music = findMusicById(r, max_id);
  if (!min_music || !max_music) {
    printf("Error finding music limits\n");
    cleanUp(EXIT_FAILURE, r, f_in, t, list);
  }

  t = loadTreeFromRadio(r);
  if (!t) {
    printf("Error creating tree\n");
    cleanUp(EXIT_FAILURE, r, f_in, t, list);
  }

  list = tree_rangeSearch(t, min_music, max_music);
  if (!list) {
    printf("Error filtering songs\n");
    cleanUp(EXIT_FAILURE, r, f_in, t, list);
  }

  printf("Lista de canciones desde id %ld hasta id %ld\n", min_id, max_id);
  if (list_print(stdout, list, music_line_print) < 0) {
    printf("Error printing list\n");
    cleanUp(EXIT_FAILURE, r, f_in, t, list);
  }

  cleanUp(EXIT_SUCCESS, r, f_in, t, list);

  return EXIT_SUCCESS;
}

/*
P2:
La lista resultante aparece ordenada de menor a mayor identificador e incluye
los extremos del rango. Esto ocurre porque la busqueda recorre el BST en inorden
y solo baja por las ramas que pueden contener canciones dentro del intervalo.
*/

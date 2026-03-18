#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "music.h"
#include "radio.h"

static int music_line_print(FILE *pf, const void *m);

static int music_line_print(FILE *pf, const void *m) {
  int written;

  if (!pf || !m) return -1;

  written = music_plain_print(pf, m);
  if (written < 0) return -1;

  return written + fprintf(pf, "\n");
}

int main(int argc, char **argv) {
  FILE *fin = NULL;
  Radio *radio = NULL;
  List *favorites = NULL;
  Music *m = NULL;
  int i;
  int total;
  int half;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s <radio_file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  fin = fopen(argv[1], "r");
  if (!fin) {
    fprintf(stderr, "Error: could not open file %s\n", argv[1]);
    return EXIT_FAILURE;
  }

  radio = radio_init();
  if (!radio) {
    fprintf(stderr, "Error: could not initialize radio\n");
    fclose(fin);
    return EXIT_FAILURE;
  }

  if (radio_readFromFile(fin, radio) == ERROR) {
    fprintf(stderr, "Error: could not read radio from file %s\n", argv[1]);
    fclose(fin);
    radio_free(radio);
    return EXIT_FAILURE;
  }

  fclose(fin);

  favorites = list_new();
  if (!favorites) {
    fprintf(stderr, "Error: could not initialize favorites list\n");
    radio_free(radio);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(radio); i++) {
    m = radio_getMusicAt(radio, i);
    if (!m) {
      fprintf(stderr, "Error: could not access radio music\n");
      list_free(favorites);
      radio_free(radio);
      return EXIT_FAILURE;
    }

    if (i % 2 == 0) {
      if (list_pushBack(favorites, m) == ERROR) {
        fprintf(stderr, "Error: could not push music to the back of the list\n");
        list_free(favorites);
        radio_free(radio);
        return EXIT_FAILURE;
      }
    } else {
      if (list_pushFront(favorites, m) == ERROR) {
        fprintf(stderr, "Error: could not push music to the front of the list\n");
        list_free(favorites);
        radio_free(radio);
        return EXIT_FAILURE;
      }
    }
  }

  if (list_print(stdout, favorites, music_line_print) < 0) {
    fprintf(stderr, "Error: could not print favorites list\n");
    list_free(favorites);
    radio_free(radio);
    return EXIT_FAILURE;
  }

  total = list_size(favorites);
  half = total / 2;

  printf("\nFinished inserting. Now we extract from the beginning:\n");
  for (i = 0; i < half; i++) {
    m = (Music *)list_popFront(favorites);
    if (!m) {
      fprintf(stderr, "Error: could not pop from the front of the list\n");
      list_free(favorites);
      radio_free(radio);
      return EXIT_FAILURE;
    }

    if (music_plain_print(stdout, m) < 0) {
      fprintf(stderr, "Error: could not print extracted front music\n");
      list_free(favorites);
      radio_free(radio);
      return EXIT_FAILURE;
    }

    if (i < half - 1) printf(" ");
  }

  printf("\nNow we extract from the end:\n");
  while (list_isEmpty(favorites) == FALSE) {
    m = (Music *)list_popBack(favorites);
    if (!m) {
      fprintf(stderr, "Error: could not pop from the back of the list\n");
      list_free(favorites);
      radio_free(radio);
      return EXIT_FAILURE;
    }

    if (music_plain_print(stdout, m) < 0) {
      fprintf(stderr, "Error: could not print extracted back music\n");
      list_free(favorites);
      radio_free(radio);
      return EXIT_FAILURE;
    }

    if (list_isEmpty(favorites) == FALSE) printf(" ");
  }
  printf("\n");

  list_free(favorites);
  radio_free(radio);

  return EXIT_SUCCESS;
}

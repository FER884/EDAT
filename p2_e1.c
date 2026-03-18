#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "music.h"
#include "radio.h"
#include "stack.h"

#define MENU_LINE_SIZE 64

static int show_player_menu(Stack *history);
static int read_menu_option(void);
static int music_history_print(FILE *pf, const void *m);

static int read_menu_option(void) {
  char line[MENU_LINE_SIZE];
  char *endptr;
  long option;

  if (!fgets(line, sizeof(line), stdin)) return 2;

  option = strtol(line, &endptr, 10);
  if (endptr == line) return -1;

  return (int)option;
}

static int music_history_print(FILE *pf, const void *m) {
  const Music *music = (const Music *)m;

  if (!pf || !music) return -1;

  return fprintf(pf, "[%ld, %s, %s, %hu, %d]", music_getId(music),
                 music_getTitle(music), music_getArtist(music),
                 music_getDuration(music), (int)music_getState(music));
}

static int show_player_menu(Stack *history) {
  Music *m = NULL;

  if (!history) return 2;

  m = (Music *)stack_top(history);
  if (m != NULL) {
    music_formatted_print(stdout, m);
  } else {
    printf("\nNo song currently playing.\n");
  }

  printf("\nRecently Played:\n");
  stack_print(stdout, history, music_history_print);

  printf("\n1. Back to previous song\n");
  printf("2. Exit\n");
  printf("Choose an option: ");

  return read_menu_option();
}

int main(int argc, char **argv) {
  FILE *fin = NULL;
  Radio *radio = NULL;
  Stack *history = NULL;
  Music *m = NULL;
  int i;
  int option;

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

  history = stack_init();
  if (!history) {
    fprintf(stderr, "Error: could not initialize history stack\n");
    radio_free(radio);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(radio); i++) {
    m = radio_getMusicAt(radio, i);
    if (!m || stack_push(history, m) == ERROR) {
      fprintf(stderr, "Error: could not push music into history\n");
      stack_free(history);
      radio_free(radio);
      return EXIT_FAILURE;
    }
  }

  do {
    option = show_player_menu(history);

    if (option == 1) {
      if (stack_isEmpty(history) == FALSE) stack_pop(history);
    } else if (option != 2) {
      printf("Invalid option\n");
    }
  } while (option != 2);

  stack_free(history);
  radio_free(radio);

  return EXIT_SUCCESS;
}

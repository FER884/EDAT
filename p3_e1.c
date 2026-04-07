#include <stdio.h>
#include <stdlib.h>

#include "music.h"
#include "queue.h"
#include "radio.h"

#define MENU_LINE_SIZE 64

static Music *current_music = NULL;

static int now_playing_menu(Queue *q);
static int read_menu_option(void);

static int read_menu_option(void) {
  char line[MENU_LINE_SIZE];
  char *endptr;
  long option;

  if (!fgets(line, sizeof(line), stdin)) return 2;

  option = strtol(line, &endptr, 10);
  if (endptr == line) return -1;

  return (int)option;
}

static int now_playing_menu(Queue *q) {
  if (current_music) {
    music_formatted_print(stdout, current_music);
  } else {
    printf("\nNo song currently playing.\n");
  }

  printf("\nUpcoming:\n");
  if (queue_print(stdout, q, music_plain_print) < 0) return 2;

  printf("\n1. Next song\n");
  printf("2. Exit\n");
  printf("Choose an option: ");

  return read_menu_option();
}

int main(int argc, char **argv) {
  FILE *fin = NULL;
  Radio *radio = NULL;
  Queue *play_queue = NULL;
  Music *m = NULL;
  int option;
  int i;

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

  play_queue = queue_new();
  if (!play_queue) {
    fprintf(stderr, "Error: could not initialize playback queue\n");
    radio_free(radio);
    return EXIT_FAILURE;
  }

  for (i = 0; i < radio_getNumberOfMusic(radio); i++) {
    m = radio_getMusicAt(radio, i);
    if (!m || queue_push(play_queue, m) == ERROR) {
      fprintf(stderr, "Error: could not enqueue music\n");
      queue_free(play_queue);
      radio_free(radio);
      return EXIT_FAILURE;
    }
  }

  current_music = (Music *)queue_pop(play_queue);

  do {
    option = now_playing_menu(play_queue);

    if (option == 1) {
      current_music = (Music *)queue_pop(play_queue);
    } else if (option != 2) {
      printf("Invalid option\n");
    }
  } while (option != 2);

  queue_free(play_queue);
  radio_free(radio);

  return EXIT_SUCCESS;
}

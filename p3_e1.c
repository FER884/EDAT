#include <stdio.h>
#include <stdlib.h>

#include "music.h"
#include "queue.h"
#include "radio.h"

int main(int argc, char **argv) {
  FILE *fin = NULL;
  Radio *radio = NULL;
  Queue *play_queue = NULL;
  Music *current_music = NULL;
  Music *m = NULL;
  char line[64];
  char *endptr;
  long option_value;
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
    if (current_music) {
      if (music_formatted_print(stdout, current_music) < 0) {
        fprintf(stderr, "Error: could not print current music\n");
        queue_free(play_queue);
        radio_free(radio);
        return EXIT_FAILURE;
      }
    } else {
      printf("\nNo song currently playing.\n");
    }

    printf("\nUpcoming:\n");
    if (queue_print(stdout, play_queue, music_plain_print) < 0) {
      fprintf(stderr, "Error: could not print playback queue\n");
      queue_free(play_queue);
      radio_free(radio);
      return EXIT_FAILURE;
    }

    printf("\n1. Next song\n");
    printf("2. Exit\n");
    printf("Choose an option: ");

    if (!fgets(line, sizeof(line), stdin)) {
      option = 2;
    } else {
      option_value = strtol(line, &endptr, 10);
      option = (endptr == line) ? -1 : (int)option_value;
    }

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

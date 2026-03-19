#include <stdio.h>
#include <stdlib.h>

#include "music.h"
#include "radio.h"
#include "stack.h"

static int music_duration_cmp(const void *m1, const void *m2);
static int music_playlist_print(FILE *pf, const void *m);
static Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout);
static Status loadPlaylist(const char *filename, Radio **radio, Stack **stack);

static int music_duration_cmp(const void *m1, const void *m2) {
  const Music *music1 = (const Music *)m1;
  const Music *music2 = (const Music *)m2;

  if (!music1 || !music2) return 0;

  return (int)music_getDuration(music1) - (int)music_getDuration(music2);
}

static int music_playlist_print(FILE *pf, const void *m) {
  const Music *music = (const Music *)m;

  if (!pf || !music) return -1;

  return fprintf(pf, "[%ld, %s, %s, %hu, %d]",
                 music_getId(music), music_getTitle(music), music_getArtist(music),
                 music_getDuration(music), (int)music_getState(music));
}

static Status mergeStacks(Stack *sin1, Stack *sin2, Stack *sout) {
  void *e;
  Stack *remaining;

  if (!sin1 || !sin2 || !sout) return ERROR;

  while (stack_isEmpty(sin1) == FALSE && stack_isEmpty(sin2) == FALSE) {
    if (music_duration_cmp(stack_top(sin1), stack_top(sin2)) > 0) {
      e = stack_pop(sin1);
    } else {
      e = stack_pop(sin2);
    }

    if (!e || stack_push(sout, e) == ERROR) return ERROR;
  }

  remaining = (stack_isEmpty(sin1) == TRUE) ? sin2 : sin1;
  while (stack_isEmpty(remaining) == FALSE) {
    e = stack_pop(remaining);
    if (!e || stack_push(sout, e) == ERROR) return ERROR;
  }

  return OK;
}

static Status loadPlaylist(const char *filename, Radio **radio, Stack **stack) {
  FILE *fin = NULL;
  int i;
  Music *m = NULL;

  if (!filename || !radio || !stack) return ERROR;

  fin = fopen(filename, "r");
  if (!fin) return ERROR;

  *radio = radio_init();
  if (!*radio) {
    fclose(fin);
    return ERROR;
  }

  if (radio_readFromFile(fin, *radio) == ERROR) {
    fclose(fin);
    radio_free(*radio);
    *radio = NULL;
    return ERROR;
  }

  fclose(fin);

  *stack = stack_init();
  if (!*stack) {
    radio_free(*radio);
    *radio = NULL;
    return ERROR;
  }

  for (i = 0; i < radio_getNumberOfMusic(*radio); i++) {
    m = radio_getMusicAt(*radio, i);
    if (!m || stack_push(*stack, m) == ERROR) {
      stack_free(*stack);
      radio_free(*radio);
      *stack = NULL;
      *radio = NULL;
      return ERROR;
    }
  }

  return OK;
}

int main(int argc, char **argv) {
  Radio *radio1 = NULL;
  Radio *radio2 = NULL;
  Stack *playlist1 = NULL;
  Stack *playlist2 = NULL;
  Stack *merged = NULL;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <playlist_file1> <playlist_file2>\n", argv[0]);
    return EXIT_FAILURE;
  }

  if (loadPlaylist(argv[1], &radio1, &playlist1) == ERROR ||
      loadPlaylist(argv[2], &radio2, &playlist2) == ERROR) {
    fprintf(stderr, "Error: could not load playlists\n");
    stack_free(playlist1);
    radio_free(radio1);
    stack_free(playlist2);
    radio_free(radio2);
    return EXIT_FAILURE;
  }

  merged = stack_init();
  if (!merged) {
    fprintf(stderr, "Error: could not initialize merged stack\n");
    stack_free(playlist1);
    radio_free(radio1);
    stack_free(playlist2);
    radio_free(radio2);
    return EXIT_FAILURE;
  }

  printf("Playlist 0:\n");
  stack_print(stdout, playlist1, music_playlist_print);
  printf("\nPlaylist 1:\n");
  stack_print(stdout, playlist2, music_playlist_print);

  if (mergeStacks(playlist1, playlist2, merged) == ERROR) {
    fprintf(stderr, "Error: could not merge playlists\n");
    stack_free(merged);
    stack_free(playlist1);
    radio_free(radio1);
    stack_free(playlist2);
    radio_free(radio2);
    return EXIT_FAILURE;
  }

  printf("\nPlaylist combined:\n");
  stack_print(stdout, merged, music_playlist_print);

  stack_free(merged);
  stack_free(playlist1);
  radio_free(radio1);
  stack_free(playlist2);
  radio_free(radio2);

  return EXIT_SUCCESS;
}

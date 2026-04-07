#include <stdio.h>
#include <stdlib.h>
#include "music.h"

static int setup_music(Music *m, long id, const char *title, const char *artist,
                       unsigned short duration) {
  if (!m) return EXIT_FAILURE;
  if (music_setId(m, id) == ERROR) return EXIT_FAILURE;
  if (music_setTitle(m, title) == ERROR) return EXIT_FAILURE;
  if (music_setArtist(m, artist) == ERROR) return EXIT_FAILURE;
  if (music_setDuration(m, duration) == ERROR) return EXIT_FAILURE;
  if (music_setState(m, NOT_LISTENED) == ERROR) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

static void print_cmp_message(int cmp, const char *name1, const char *name2) {
  if (cmp < 0) {
    printf("%s es menor que %s\n", name1, name2);
  } else if (cmp > 0) {
    printf("%s es mayor que %s\n", name1, name2);
  } else {
    printf("%s y %s son iguales\n", name1, name2);
  }
}

int main(void) {
  Music *blindingLights = NULL;
  Music *bohemianRhapsody = NULL;
  Music *thirdMusic = NULL;
  const char *title = NULL;

  blindingLights = music_init();
  if (!blindingLights) {
    fprintf(stderr, "Error creando blindingLights\n");
    return EXIT_FAILURE;
  }

  bohemianRhapsody = music_init();
  if (!bohemianRhapsody) {
    fprintf(stderr, "Error creando bohemianRhapsody\n");
    music_free(blindingLights);
    return EXIT_FAILURE;
  }

  if (setup_music(blindingLights, 10, "Blinding Lights", "The Weeknd", 200) == EXIT_FAILURE ||
      setup_music(bohemianRhapsody, 20, "Bohemian Rhapsody", "Queen", 355) == EXIT_FAILURE) {
    fprintf(stderr, "Error inicializando canciones\n");
    music_free(blindingLights);
    music_free(bohemianRhapsody);
    return EXIT_FAILURE;
  }

  music_formatted_print(stdout, blindingLights);
  music_formatted_print(stdout, bohemianRhapsody);

  print_cmp_message(music_cmp(blindingLights, bohemianRhapsody), "blindingLights",
                    "bohemianRhapsody");

  title = music_getTitle(bohemianRhapsody);
  if (!title) {
    fprintf(stderr, "Error obteniendo el titulo de bohemianRhapsody\n");
    music_free(blindingLights);
    music_free(bohemianRhapsody);
    return EXIT_FAILURE;
  }
  printf("Titulo de la segunda cancion: %s\n", title);

  thirdMusic = music_copy(blindingLights);
  if (!thirdMusic) {
    fprintf(stderr, "Error copiando blindingLights en thirdMusic\n");
    music_free(blindingLights);
    music_free(bohemianRhapsody);
    return EXIT_FAILURE;
  }

  printf("ID de la tercera cancion: %ld\n", music_getId(thirdMusic));
  music_formatted_print(stdout, blindingLights);
  music_formatted_print(stdout, thirdMusic);

  print_cmp_message(music_cmp(blindingLights, thirdMusic), "blindingLights", "thirdMusic");

  music_free(blindingLights);
  music_free(bohemianRhapsody);
  music_free(thirdMusic);

  return EXIT_SUCCESS;
}

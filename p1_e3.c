#include <stdio.h>
#include <stdlib.h>

#include "radio.h"

int main(int argc, char **argv) {
  FILE *fin = NULL;
  Radio *radio = NULL;

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

  printf("Radio recommendations:\n");
  if (radio_print(stdout, radio) < 0) {
    fprintf(stderr, "Error: could not print radio\n");
    radio_free(radio);
    return EXIT_FAILURE;
  }

  radio_free(radio);
  return EXIT_SUCCESS;
}

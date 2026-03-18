#include <stdio.h>
#include <stdlib.h>

#include "radio.h"

int main(int argc, char **argv) {
  FILE *fin = NULL;
  Radio *radio = NULL;
  long from_id;
  long to_id;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s <radio_file> <from_id> <to_id>\n", argv[0]);
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

  from_id = atol(argv[2]);
  to_id = atol(argv[3]);

  printf("Radio:\n");
  if (radio_print(stdout, radio) < 0) {
    fprintf(stderr, "Error: could not print radio\n");
    radio_free(radio);
    return EXIT_FAILURE;
  }

  printf("From music with id: %ld\n", from_id);
  printf("To music with id: %ld\n", to_id);
  printf("Music exploration path:\n");

  if (radio_depthSearch(radio, from_id, to_id) == ERROR) {
    fprintf(stderr, "Error: could not search through radio\n");
    radio_free(radio);
    return EXIT_FAILURE;
  }

  radio_free(radio);
  return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

#include "radio.h"

int main(void) {
  Radio *r;
  Status st;
  int from_111;

  r = radio_init();
  if (!r) {
    fprintf(stderr, "Error: no se pudo inicializar la radio\n");
    return EXIT_FAILURE;
  }

  st = radio_newMusic(r,
                      "id:\"111\" title:\"Paint It, Black\" artist:\"The Rolling Stones\" "
                      "duration:\"202\" state:\"NOT_LISTENED\"");
  printf("Inserting Paint It, Black... result...: %d\n", st);

  st = radio_newMusic(r,
                      "id:\"222\" title:\"Every Breath You Take\" artist:\"The Police\" "
                      "duration:\"253\" state:\"NOT_LISTENED\"");
  printf("Inserting Every Breath You Take... result...: %d\n", st);

  printf("\nInserting radio recommendation: 222 --> 111\n");
  if (radio_newRelation(r, 222, 111) == ERROR) {
    fprintf(stderr, "Error: no se pudo insertar la recomendacion\n");
    radio_free(r);
    return EXIT_FAILURE;
  }

  printf("\n111 --> 222? %s\n", radio_relationExists(r, 111, 222) == TRUE ? "Yes" : "No");
  printf("222 --> 111? %s\n", radio_relationExists(r, 222, 111) == TRUE ? "Yes" : "No");

  from_111 = radio_getNumberOfRelationsFromId(r, 111);
  printf("Number of connections from 111: %d\n", from_111);

  printf("\nAll radio recommendations:\n");
  if (radio_print(stdout, r) < 0) {
    fprintf(stderr, "Error: no se pudo imprimir la radio\n");
    radio_free(r);
    return EXIT_FAILURE;
  }

  radio_free(r);
  return EXIT_SUCCESS;
}

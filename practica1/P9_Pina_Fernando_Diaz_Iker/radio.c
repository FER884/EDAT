#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "radio.h"

#define MAX_MSC 4096
#define RADIO_LINE_SIZE 4096

struct _Radio {
  Music *songs[MAX_MSC];               /* Array with the radio music */
  Bool relations[MAX_MSC][MAX_MSC];    /* Adjacency matrix */
  int num_music;                       /* Total number of music */
  int num_relations;                   /* Total number of relations */
};

/*----------------------------------------------------------------------------------------*/
/*
Private functions:
*/
static int radio_getIndexById(const Radio *r, long id);

static int radio_getIndexById(const Radio *r, long id) {
  int i;

  if (!r || id < 0) return -1;

  for (i = 0; i < r->num_music; i++) {
    if (music_getId(r->songs[i]) == id) return i;
  }

  return -1;
}

/*----------------------------------------------------------------------------------------*/
/**
 * @brief Creates a new empty radio.
 *
 * Allocates memory for a new radio and initializes it to be empty
 * (no music and no relations).
 *
 * @return A pointer to the radio if it was correctly allocated,
 * NULL otherwise.
 **/
Radio *radio_init() {
  Radio *new_radio;
  int i, j;

  new_radio = (Radio *)malloc(sizeof(Radio));
  if (!new_radio) return NULL;

  new_radio->num_music = 0;
  new_radio->num_relations = 0;

  for (i = 0; i < MAX_MSC; i++) {
    new_radio->songs[i] = NULL;
    for (j = 0; j < MAX_MSC; j++) {
      new_radio->relations[i][j] = FALSE;
    }
  }

  return new_radio;
}

/**
 * @brief Frees a radio.
 *
 * Frees all the memory allocated for the radio.
 *
 * @param r Pointer to radio to be free.
 **/
void radio_free(Radio *r) {
  int i;

  if (!r) return;

  for (i = 0; i < r->num_music; i++) {
    music_free(r->songs[i]);
  }

  free(r);
}

/**
 * @brief Inserts a new music in a radio.
 *
 * Creates a music by calling music_initFromString and adds it to
 * a radio. If a music with the same id already exists in the radio,
 * it is not added.
 *
 * @param r Pointer to the radio.
 * @param desc Description of the music.
 *
 * @return Returns OK if the music could be created (or if it exists
 * already), ERROR otherwise.
 **/
Status radio_newMusic(Radio *r, char *desc) {
  Music *new_music;
  long new_id;

  if (!r || !desc) return ERROR;
  if (r->num_music >= MAX_MSC) return ERROR;

  new_music = music_initFromString(desc);
  if (!new_music) return ERROR;

  new_id = music_getId(new_music);
  if (radio_getIndexById(r, new_id) >= 0) {
    music_free(new_music);
    return OK;
  }

  r->songs[r->num_music] = new_music;
  r->num_music++;

  return OK;
}

/**
 * @brief Creates a relation between two music of a radio.
 *
 * If any of the two music does not exist in the radio the relation is
 * not created.
 *
 * @param r Pointer to the radio.
 * @param orig ID of the origin music.
 * @param dest ID of the destination music.
 *
 * @return OK if the relation could be added to the radio, ERROR otherwise.
 **/
Status radio_newRelation(Radio *r, long orig, long dest) {
  int i_orig, i_dest;

  if (!r) return ERROR;

  i_orig = radio_getIndexById(r, orig);
  i_dest = radio_getIndexById(r, dest);
  if (i_orig < 0 || i_dest < 0) return ERROR;

  if (r->relations[i_orig][i_dest] == TRUE) return OK;

  r->relations[i_orig][i_dest] = TRUE;
  r->num_relations++;

  return OK;
}

/**
 * @brief Checks if a radio contains a music.
 *
 * @param r Pointer to the radio.
 * @param id ID of the music.
 *
 * @return Returns TRUE if there is a music in the radio g with the
 * ID id, FALSE otherwise.
 **/
Bool radio_contains(const Radio *r, long id) {
  if (!r || id < 0) return FALSE;
  return (radio_getIndexById(r, id) >= 0) ? TRUE : FALSE;
}

/**
 * @brief Returns the total number of music in a radio.
 *
 * @param r Pointer to the radio.
 *
 * @return Returns The number of music in the radio, or -1 if
 * there is any error.
 **/
int radio_getNumberOfMusic(const Radio *r) {
  if (!r) return -1;
  return r->num_music;
}

/**
 * @brief Returns the total number of relations in the radio.
 *
 * @param r Pointer to the radio.
 *
 * @return Returns The number of relations in the radio, or -1 if
 * there is any error.
 **/
int radio_getNumberOfRelations(const Radio *r) {
  if (!r) return -1;
  return r->num_relations;
}

/**
 * @brief Determines if there is a relation between a pair of music.
 *
 * @param r Pointer to the radio.
 * @param orig ID of the origin music.
 * @param dest ID of the destination music.
 *
 * @return Returns TRUE if there is a relation in r from orig
 *  to dest, FALSE otherwise.
 **/
Bool radio_relationExists(const Radio *r, long orig, long dest) {
  int i_orig, i_dest;

  if (!r) return FALSE;

  i_orig = radio_getIndexById(r, orig);
  i_dest = radio_getIndexById(r, dest);
  if (i_orig < 0 || i_dest < 0) return FALSE;

  return r->relations[i_orig][i_dest];
}

/**
 * @brief Gets the number of relations starting at a given music.
 *
 * @param r Pointer to the radio.
 * @param id ID of the origin music.
 *
 * @return Returns the total number of relation starting at
 * music with ID id, or -1 if there is any error.
 **/
int radio_getNumberOfRelationsFromId(const Radio *r, long id) {
  int i_orig, i, total = 0;

  if (!r) return -1;

  i_orig = radio_getIndexById(r, id);
  if (i_orig < 0) return -1;

  for (i = 0; i < r->num_music; i++) {
    if (r->relations[i_orig][i] == TRUE) total++;
  }

  return total;
}

/**
 * @brief Returns an array with the ids of all the music which a
 * given music connects to.
 *
 * This function allocates memory for the array.
 *
 * @param r Pointer to the radio.
 * @param id ID of the origin music.
 *
 * @return Returns an array with the ids of all the music to which
 * the music with ID id is connected, or NULL if there is any error.
 */
long *radio_getRelationsFromId(const Radio *r, long id) {
  long *ids;
  int i_orig, i, count, index = 0;

  if (!r) return NULL;

  i_orig = radio_getIndexById(r, id);
  if (i_orig < 0) return NULL;

  count = radio_getNumberOfRelationsFromId(r, id);
  if (count < 0) return NULL;

  ids = (long *)malloc(sizeof(long) * (count > 0 ? count : 1));
  if (!ids) return NULL;

  for (i = 0; i < r->num_music; i++) {
    if (r->relations[i_orig][i] == TRUE) {
      ids[index] = music_getId(r->songs[i]);
      index++;
    }
  }

  return ids;
}

/**
 * @brief Prints a radio.
 *
 * Prints the radio g to the file pf.
 * The format to be followed is: print a line by music with the
 * information associated with the music and then their connections
 *
 * @param pf File descriptor.
 * @param r Pointer to the radio.
 *
 * @return The number of characters printed, or -1 if there is any error.
 */
int radio_print(FILE *pf, const Radio *r) {
  int i, j, chars = 0, written;

  if (!pf || !r) return -1;

  for (i = 0; i < r->num_music; i++) {
    written = music_plain_print(pf, r->songs[i]);
    if (written < 0) return -1;
    chars += written;

    written = fprintf(pf, ":");
    if (written < 0) return -1;
    chars += written;

    for (j = 0; j < r->num_music; j++) {
      if (r->relations[i][j] == TRUE) {
        written = fprintf(pf, " ");
        if (written < 0) return -1;
        chars += written;

        written = music_plain_print(pf, r->songs[j]);
        if (written < 0) return -1;
        chars += written;
      }
    }

    written = fprintf(pf, "\n");
    if (written < 0) return -1;
    chars += written;
  }

  return chars;
}

/**
 * @brief Reads a radio definition from a text file.
 *
 * Reads a radio description from the text file pointed to by fin,
 * and fills the radio r.
 *
 * @param fin Pointer to the input stream.
 * @param r Pointer to the radio.
 *
 * @return OK or ERROR
 */
Status radio_readFromFile(FILE *fin, Radio *r) {
  char line[RADIO_LINE_SIZE];
  char *token;
  long number_of_music, orig, dest;
  int i;

  if (!fin || !r) return ERROR;

  if (!fgets(line, sizeof(line), fin)) return ERROR;
  number_of_music = atol(line);
  if (number_of_music < 0 || number_of_music > MAX_MSC) return ERROR;

  for (i = 0; i < number_of_music; i++) {
    if (!fgets(line, sizeof(line), fin)) return ERROR;
    line[strcspn(line, "\r\n")] = '\0';
    if (radio_newMusic(r, line) == ERROR) return ERROR;
  }

  while (fgets(line, sizeof(line), fin)) {
    token = strtok(line, " \t\r\n");
    if (!token) continue;

    orig = atol(token);
    token = strtok(NULL, " \t\r\n");
    while (token) {
      dest = atol(token);
      if (radio_newRelation(r, orig, dest) == ERROR) return ERROR;
      token = strtok(NULL, " \t\r\n");
    }
  }

  return OK;
}

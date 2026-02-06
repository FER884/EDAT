/**
 * @file  music.c
 * @author Profesores EDAT
 * @date February 2026
 * @mersion 1.0
 * @brief Library to manage ADT Music
 *
 * @details
 *
 * @see
 */

#include <string.h>
#include "music.h"

/* START [STR_LENGTH] */
#define STR_LENGTH 64
/* END [STR_LENGTH] */

/* Music declaration goes here */
struct _Music {
  long id;
  char title[STR_LENGTH];
  char artist[STR_LENGTH];
  unsigned short duration;
  State state;
};

/*----------------------------------------------------------------------------------------*/
/*
Private function:
*/
static Status music_setField(Music *m, char *key, char *value);
static Status music_parseState(const char *value, State *state);

/**
 * @brief Parses a textual state and maps it to State enum.
 *
 * Accepted values are `NOT_LISTENED`/`0` and `LISTENED`/`1`.
 *
 * @param value String with the state value.
 * @param state Output state.
 *
 * @return Returns OK if parsing succeeds, ERROR otherwise.
 */
static Status music_parseState(const char *value, State *state) {
  if (!value || !state) return ERROR;

  if (strcmp(value, "NOT_LISTENED") == 0 || strcmp(value, "0") == 0) {
    *state = NOT_LISTENED;
    return OK;
  }

  if (strcmp(value, "LISTENED") == 0 || strcmp(value, "1") == 0) {
    *state = LISTENED;
    return OK;
  }

  return ERROR;
}

/**
 * @brief Sets one field of a Music from a key:value pair.
 *
 * Valid keys are `id`, `title`, `artist`, `duration` and `state`.
 *
 * @param m Music pointer.
 * @param key Field name.
 * @param value Field value.
 *
 * @return Returns OK if the field is set correctly, ERROR otherwise.
 */
static Status music_setField(Music *m, char *key, char *value) {
  long number;
  State state;

  if (!m || !key || !value) return ERROR;

  if (strcmp(key, "id") == 0) {
    number = atol(value);
    return music_setId(m, number);
  }

  if (strcmp(key, "title") == 0) {
    return music_setTitle(m, value);
  }

  if (strcmp(key, "artist") == 0) {
    return music_setArtist(m, value);
  }

  if (strcmp(key, "duration") == 0) {
    number = atol(value);
    if (number < 0) return ERROR;
    return music_setDuration(m, (unsigned short)number);
  }

  if (strcmp(key, "state") == 0) {
    if (music_parseState(value, &state) == ERROR) return ERROR;
    return music_setState(m, state);
  }

  return ERROR;
}

/**
 * @brief Constructor. Initialize a Music.
 *
 * This function allocates memory for a Music and sets its fields to
 * id to 0, title to "", artist to "", duration to 0 and state to NOT_LISTENED.
 *
 * @return Return the initialized Music if it was done correctly,
 * otherwise return NULL.
 */
Music *music_init() {
  Music *m;

  m = (Music *)malloc(sizeof(Music));
  if (!m) return NULL;

  m->id = 0;
  m->title[0] = '\0';
  m->artist[0] = '\0';
  m->duration = 0;
  m->state = NOT_LISTENED;

  return m;
}

/**
 * @brief Constructor. Initialize a Music from its description.
 *
 * The description is a list of key:value pairs separated by whitespace.
 *
 * @param descr String describing the Music.
 *
 * @return Return the initialized Music if it was done correctly,
 * otherwise return NULL.
 */
Music *music_initFromString(char *descr) {
  Music *m;
  char *pair;
  char *saveptr;
  char *separator;
  char *buffer;

  if (!descr) return NULL;

  buffer = strdup(descr);
  if (!buffer) return NULL;

  m = music_init();
  if (!m) {
    free(buffer);
    return NULL;
  }

  pair = strtok_r(buffer, " \t\n", &saveptr);
  while (pair) {
    separator = strchr(pair, ':');
    if (!separator) {
      music_free(m);
      free(buffer);
      return NULL;
    }

    *separator = '\0';
    if (music_setField(m, pair, separator + 1) == ERROR) {
      music_free(m);
      free(buffer);
      return NULL;
    }

    pair = strtok_r(NULL, " \t\n", &saveptr);
  }

  free(buffer);
  return m;
}

/**
 * @brief Destructor. Free the dynamic memory reserved for a Music.
 *
 * @param m Music to free.
 */
void music_free(void *m) {
  if (!m) return;
  free((Music *)m);
}

/**
 * @brief Gets the Music id.
 *
 * @param m Music pointer.
 *
 * @return Returns the id of the given Music, or -1 in case of error.
 */
long music_getId(const Music *m) {
  if (!m) return -1;
  return m->id;
}

/**
 * @brief Gets the Music title.
 *
 * @param m Music pointer.
 *
 * @return Returns a pointer to the title of the Music, or NULL in case of error.
 */
const char *music_getTitle(const Music *m) {
  if (!m) return NULL;
  return m->title;
}

/**
 * @brief Gets the Music artist.
 *
 * @param m Music pointer.
 *
 * @return Returns a pointer to the artist of the Music, or NULL in case of error.
 */
const char *music_getArtist(const Music *m) {
  if (!m) return NULL;
  return m->artist;
}

/**
 * @brief Gets the Music duration.
 *
 * @param m Music pointer.
 *
 * @return Returns the duration of the given Music, or -1 in case of error.
 */
unsigned short music_getDuration(const Music *m) {
  if (!m) return (unsigned short)-1;
  return m->duration;
}

/**
 * @brief Gets the state of a given Music.
 *
 * @param m Music pointer.
 *
 * @return Returns the state of a given Music, or ERROR_MUSIC in case of error.
 */
State music_getState(const Music *m) {
  if (!m) return ERROR_MUSIC;
  return m->state;
}

/**
 * @brief Modifies the id of a given Music.
 *
 * @param m Music pointer.
 * @param id New Music id, must be equal or greater than 0.
 *
 * @return Returns OK or ERROR in case of error.
 */
Status music_setId(Music *m, const long id) {
  if (m && id >= 0) {
    m->id = id;
    return OK;
  }
  return ERROR;
}

/**
 * @brief Modifies the title of a given Music.
 *
 * @param m Music pointer.
 * @param title New Music title, must fit in the Music.
 *
 * @return Returns OK or ERROR in case of error.
 */
Status music_setTitle(Music *m, const char *title) {
  if (m && title && strlen(title) < STR_LENGTH) {
    strncpy(m->title, title, STR_LENGTH);
    m->title[STR_LENGTH - 1] = '\0';
    return OK;
  }
  return ERROR;
}

/**
 * @brief Modifies the artist of a given Music.
 *
 * @param m Music pointer.
 * @param artist New Music artist, must fit in the Music.
 *
 * @return Returns OK or ERROR in case of error.
 */
Status music_setArtist(Music *m, const char *artist) {
  if (m && artist && strlen(artist) < STR_LENGTH) {
    strncpy(m->artist, artist, STR_LENGTH);
    m->artist[STR_LENGTH - 1] = '\0';
    return OK;
  }
  return ERROR;
}

/**
 * @brief Modifies the duration of a given Music.
 *
 * @param m Music pointer.
 * @param duration New Music duration, must fit in the Music.
 *
 * @return Returns OK or ERROR in case of error.
 */
Status music_setDuration(Music *m, const unsigned short duration) {
  if (m) {
    m->duration = duration;
    return OK;
  }
  return ERROR;
}

/**
 * @brief Modifies the state of a given Music.
 *
 * @param m Music pointer.
 * @param state New Music state.
 *
 * @return Returns OK or ERROR in case of error.
 */
Status music_setState(Music *m, const State state) {
  if (!m) return ERROR;
  if (state != NOT_LISTENED && state != LISTENED) return ERROR;

  m->state = state;
  return OK;
}

/**
 * @brief Compares two music.
 *
 * First it compares their ids. If they are equal, then compares
 * their title. If equal, compares artist.
 *
 * @param m1,m2 Music to compare.
 *
 * @return It returns an integer less than or greater than zero if
 * id of m1 is found, respectively, to be less than or be greater
 * than id of m2. If they are equal, then returns the result of
 * comparing their titles if they are different. If they are equal, then
 * returns the result of comparing their artists.
 */
int music_cmp(const void *m1, const void *m2) {
  const Music *a = (const Music *)m1;
  const Music *b = (const Music *)m2;
  int c;

  if (!a || !b) return 0;

  if (a->id < b->id) return -1;
  if (a->id > b->id) return 1;

  c = strcmp(a->title, b->title);
  if (c != 0) return c;

  return strcmp(a->artist, b->artist);
}

/**
 * @brief Reserves memory for a Music where it copies the data from src.
 *
 * @param src Original Music pointer.
 *
 * @return Returns the pointer of the copied Music if everything
 * went well, or NULL otherwise.
 */
void *music_copy(const void *src) {
  const Music *m = (const Music *)src;
  Music *copy;

  if (!m) return NULL;

  copy = music_init();
  if (!copy) return NULL;

  copy->id = m->id;
  strncpy(copy->title, m->title, STR_LENGTH);
  copy->title[STR_LENGTH - 1] = '\0';
  strncpy(copy->artist, m->artist, STR_LENGTH);
  copy->artist[STR_LENGTH - 1] = '\0';
  copy->duration = m->duration;
  copy->state = m->state;

  return copy;
}

/**
 * @brief Prints in pf the data of a Music.
 *
 * Prints without format: [id, title, artist, duration, state], without
 * line break at the end.
 *
 * @param pf File descriptor.
 * @param m Music to be printed.
 *
 * @return Returns the number of characters that have been written
 * successfully. If there have been errors returns -1.
 */
int music_plain_print(FILE *pf, const void *m) {
  const Music *aux;

  if (!pf || !m) return -1;
  aux = (const Music *)m;

  return fprintf(pf, "[%ld, %s, %s, %hu, %d]",
                 aux->id, aux->title, aux->artist, aux->duration, (int)aux->state);
}

/**
 * @brief Prints in pf the formatted data of a Music.
 *
 * @param pf File descriptor.
 * @param m Music to be printed.
 *
 * @return Returns the number of characters that have been written
 * successfully. If there have been errors returns -1.
 */
int music_formatted_print(FILE *pf, const void *m) {
  const Music *aux;
  int counter = 0;
  int minutes;
  int sec;

  if (!pf || !m) return -1;

  aux = (const Music *)m;
  minutes = aux->duration / 60;
  sec = aux->duration % 60;

  counter = fprintf(pf, "\t ɴᴏᴡ ᴘʟᴀʏɪɴɢ: %s\n", aux->title);
  counter += fprintf(pf, "\t • Artist %s •\n", aux->artist);
  counter += fprintf(pf, "\t──────────⚪──────────\n");
  counter += fprintf(pf, "\t\t◄◄⠀▐▐ ⠀►►\n");
  counter += fprintf(pf, "\t 0:00 / %02d:%02d ───○ 🔊⠀\n\n", minutes, sec);

  return counter;
}

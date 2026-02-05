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


/*----------------------------------------------------------------------------------------*/
/*
Private function:
*/
Status music_setField (Music *m, char *key, char *value);

Status music_setField (Music *m, char *key, char *value) {
  if (!key || !value) return ERROR;

  if (strcmp(key, "id") == 0) {
    return music_setId(m, atol(value));
  } else if (strcmp(key, "title") == 0) {
    return music_setTitle(m, value);
  } else if (strcmp(key, "artist") == 0) {
    return music_setArtist(m, value);
  } else if (strcmp(key, "duration") == 0) {
    return music_setDuration(m, atol(value));
  } else if (strcmp(key, "state") == 0) {
    return music_setState(m, (State)atoi(value));
  }

  return ERROR;
}

/*----------------------------------------------------------------------------------------*/
Music *music_initFromString(char *descr) {
  Music *m;
  char *p;
  char *key_start;
  char *value_start;
  char *buffer;

  if (!descr) return NULL;

  buffer = strdup(descr);          /* copiar entrada */
  if (!buffer) return NULL;

  m = music_init();
  if (!m) {
    free(buffer);
    return NULL;
  }

  p = buffer;

  while (*p) {
    /* Skip whitespace */
    while (*p == ' ' || *p == '\t' || *p == '\n')
      p++;

    if (!*p) break;

    /* Parse key */
    key_start = p;
    while (*p && *p != ':')
      p++;

    if (!*p) break;
    *p++ = '\0';   /* terminate key */

    /* Expect opening quote */
    if (*p != '"') break;
    p++;

    /* Parse value */
    value_start = p;
    while (*p && *p != '"')
      p++;

    if (!*p) break;
    *p++ = '\0';   /* terminate value */

    music_setField(m, key_start, value_start);
  }

  free(buffer);
  return m;
}


/**  Remaining functions of music.h to be implemented here **/


int music_formatted_print (FILE * pf, const void * m) {
	Music * aux;
	int counter = 0, minutes, sec;
	if (!pf || !m) return -1;

	aux = (Music*) m;
	
	if (!aux->duration || aux->duration <= 0) return -1;
	minutes = aux->duration / 60;
    sec = aux->duration % 60;
	
	counter = fprintf(pf, "\t ɴᴏᴡ ᴘʟᴀʏɪɴɢ: %s\n", aux->title);
	counter += fprintf(pf, "\t • Artist %s •\n", aux->artist);
	counter += fprintf(pf, "\t──────────⚪──────────\n");
	counter += fprintf(pf, "\t\t◄◄⠀▐▐ ⠀►►\n");
	counter += fprintf(pf, "\t 0:00 / %02d:%02d ───○ 🔊⠀\n\n", minutes, sec);
	
	return counter;
}




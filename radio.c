#include "types.h"
#include "radio.h"
#include "music.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSC 50
#define BUFFER_SIZE 1024

struct _Radio
{
    Music *songs[MAX_MSC];            /* Array with the radio music */
    Bool relations[MAX_MSC][MAX_MSC]; /* Adjacency matrix */
    int num_music;                    /* Total number of music */
    int num_relations;                /* Total number of relations */
};

static int radio_getIndexById(const Radio *r, long id)
{
    int i;

    if (!r || id < 0)
        return -1;

    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == id)
            return i;
    }

    return -1;
}

/**
 * @brief Creates a new empty radio.
 *
 * Allocates memory for a new radio and initializes it to be empty
 * (no music and no relations).
 *
 * @return A pointer to the radio if it was correctly allocated,
 * NULL otherwise.
 **/
Radio *radio_init()
{
    Radio *new_radio = NULL;
    int i;
    int j;

    new_radio = (Radio *)malloc(sizeof(Radio));
    if (!new_radio)
        return NULL;

    new_radio->num_music = 0;
    new_radio->num_relations = 0;

    for (i = 0; i < MAX_MSC; i++)
    {
        new_radio->songs[i] = NULL;
        for (j = 0; j < MAX_MSC; j++)
        {
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
void radio_free(Radio *r)
{
    int i;

    if (!r)
        return;

    for (i = 0; i < r->num_music; i++)
    {
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
Status radio_newMusic(Radio *r, char *desc)
{
    Music *new_music = NULL;
    long new_id;
    int i;

    if (!r || !desc)
        return ERROR;
    if (r->num_music >= MAX_MSC)
        return ERROR;

    new_music = music_initFromString(desc);
    if (!new_music)
        return ERROR;

    new_id = music_getId(new_music);
    for (i = 0; i < r->num_music; i++)
    {
        if (music_getId(r->songs[i]) == new_id)
        {
            music_free(new_music);
            return OK;
        }
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
Status radio_newRelation(Radio *r, long orig, long dest)
{
    int orig_idx;
    int dest_idx;

    if (!r)
    {
        return ERROR;
    }

    orig_idx = radio_getIndexById(r, orig);
    dest_idx = radio_getIndexById(r, dest);
    if (orig_idx < 0 || dest_idx < 0)
        return ERROR;

    if (r->relations[orig_idx][dest_idx] == FALSE)
    {
        r->relations[orig_idx][dest_idx] = TRUE;
        r->num_relations++;
    }

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
Bool radio_contains(const Radio *r, long id)
{
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
int radio_getNumberOfMusic(const Radio *r)
{
    if (!r)
        return -1;
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
int radio_getNumberOfRelations(const Radio *r)
{
    if (!r)
        return -1;
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
Bool radio_relationExists(const Radio *r, long orig, long dest)
{
    int orig_idx;
    int dest_idx;

    if (!r)
        return FALSE;

    orig_idx = radio_getIndexById(r, orig);
    dest_idx = radio_getIndexById(r, dest);
    if (orig_idx < 0 || dest_idx < 0)
        return FALSE;

    return r->relations[orig_idx][dest_idx];
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
int radio_getNumberOfRelationsFromId(const Radio *r, long id)
{
    int idx;
    int i;
    int count = 0;

    if (!r)
        return -1;

    idx = radio_getIndexById(r, id);
    if (idx < 0)
        return -1;

    for (i = 0; i < r->num_music; i++)
    {
        if (r->relations[idx][i] == TRUE)
            count++;
    }

    return count;
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
long *radio_getRelationsFromId(const Radio *r, long id)
{
    long *ids = NULL;
    int idx;
    int total;
    int i;
    int pos = 0;

    if (!r)
        return NULL;

    idx = radio_getIndexById(r, id);
    if (idx < 0)
        return NULL;

    total = radio_getNumberOfRelationsFromId(r, id);
    if (total < 0)
        return NULL;

    if (total == 0)
    {
        ids = (long *)calloc(1, sizeof(long));
        return ids;
    }

    ids = (long *)malloc(sizeof(long) * (size_t)total);
    if (!ids)
        return NULL;

    for (i = 0; i < r->num_music; i++)
    {
        if (r->relations[idx][i] == TRUE)
        {
            ids[pos] = music_getId(r->songs[i]);
            pos++;
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
int radio_print(FILE *pf, const Radio *r)
{
    int i;
    int j;
    int written;
    int total = 0;

    if (!pf || !r)
        return -1;

    for (i = 0; i < r->num_music; i++)
    {
        written = music_plain_print(pf, r->songs[i]);
        if (written < 0)
            return -1;
        total += written;

        written = fprintf(pf, ":");
        if (written < 0)
            return -1;
        total += written;

        for (j = 0; j < r->num_music; j++)
        {
            if (r->relations[i][j] == TRUE)
            {
                written = fprintf(pf, " ");
                if (written < 0)
                    return -1;
                total += written;

                written = music_plain_print(pf, r->songs[j]);
                if (written < 0)
                    return -1;
                total += written;
            }
        }

        written = fprintf(pf, "\n");
        if (written < 0)
            return -1;
        total += written;
    }

    return total;
}

/**
 * @brief Reads a radio definition from a text file.
 *
 * Reads a radio description from the text file pointed to by fin,
 * and fills the radio r.
 *
 * The first line in the file contains the number of music.
 * Then one line per music with the music description.
 * Finally one line per relation, with the ids of the origin and
 * the destination (this is one way only)
 *
 * For example:
 *
 * 5
 * id:"317" title:"Golden" artist:"Huntrix" duration:"194"
 * id:"482" title:"Watermelon Sugar" artist:"Harry Styles" duration: "174"
 * id:"105" title:"Don't Stop Believin" artist:"Journey" duration: "251"
 * id:"231" title:"Livin' on a Prayer" artist:"Bon Jovi" duration: "249"
 * id:"764" title:"Sweet Child O' Mine" artist:"Guns N' Roses" duration: "356"
 * 482 317
 * 105 231
 * 231 105 
 * 764 231
 *
 * @param fin Pointer to the input stream.
 * @param r Pointer to the radio.
 *
 * @return OK or ERROR
 */
Status radio_readFromFile(FILE *fin, Radio *r)
{
    char buffer[BUFFER_SIZE];
    int expected_music;
    int i;
    char *token;
    char *saveptr;
    long orig;

    if (!fin || !r)
        return ERROR;

    if (!fgets(buffer, sizeof(buffer), fin))
        return ERROR;

    expected_music = atoi(buffer);
    if (expected_music < 0)
        return ERROR;

    for (i = 0; i < expected_music; i++)
    {
        if (!fgets(buffer, sizeof(buffer), fin))
            return ERROR;

        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (radio_newMusic(r, buffer) == ERROR)
            return ERROR;
    }

    while (fgets(buffer, sizeof(buffer), fin))
    {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        token = strtok_r(buffer, " \t", &saveptr);
        if (!token)
            continue;

        orig = atol(token);

        token = strtok_r(NULL, " \t", &saveptr);
        while (token)
        {
            if (radio_newRelation(r, orig, atol(token)) == ERROR)
                return ERROR;
            token = strtok_r(NULL, " \t", &saveptr);
        }
    }

    return OK;
}

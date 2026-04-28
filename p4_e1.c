#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bstree.h"
#include "radio.h"
#include "types.h"

/* START Private methods */
int mainCleanUp (int ret_value, Radio *r, FILE *pf, Music **songs) {
  if (songs) {
    free(songs);
  }

  radio_free(r);
  if (pf) {
    fclose(pf);
  }

  exit(ret_value);
}

Music **copySongsFromRadio(const Radio *r, int *n) {
  Music **songs;
  int i, total;

  if (!r || !n) {
    return NULL;
  }

  total = radio_getNumberOfMusic(r);
  if (total <= 0) {
    return NULL;
  }

  songs = (Music **)malloc(sizeof(Music *) * total);
  if (!songs) {
    return NULL;
  }

  for (i = 0; i < total; i++) {
    songs[i] = radio_getMusicAt(r, i);
    if (!songs[i]) {
      free(songs);
      return NULL;
    }
  }

  *n = total;

  return songs;
}

Music *findMusicById(Music **songs, int n, long music_id) {
  int i;

  if (!songs || n <= 0 || music_id < 0) {
    return NULL;
  }

  for (i = 0; i < n; i++) {
    if (music_getId(songs[i]) == music_id) {
      return songs[i];
    }
  }

  return NULL;
}

FILE *openInputFile(const char *filename) {
  FILE *f;
  char alt_name[1024];
  size_t len;

  if (!filename) {
    return NULL;
  }

  f = fopen(filename, "r");
  if (f) {
    return f;
  }

  len = strlen(filename);
  if (len >= 4 && strcmp(filename + len - 4, ".txt") == 0) {
    return NULL;
  }

  if (len + 4 >= sizeof(alt_name)) {
    return NULL;
  }

  sprintf(alt_name, "%s.txt", filename);

  return fopen(alt_name, "r");
}

void loadBalancedTree_rec(Music **sorted_data, BSTree *t, int first, int last) {
  int middle = (first + last) / 2;
  Music *m;

  if (first <= last) {
    m = *(&(sorted_data[0]) + middle);
    if (tree_insert(t, m) == ERROR) {
      fprintf(stdout, "Music ");
      music_plain_print(stdout, m);
      fprintf(stdout, " not inserted!\n");
    }

    loadBalancedTree_rec(sorted_data, t, first, middle - 1);
    loadBalancedTree_rec(sorted_data, t, middle + 1, last);
  }
}

BSTree *loadBalancedTree(Music **data, int n) {
  BSTree *t;

  if (!data || (n <= 0)) {
    return NULL;
  }

  if (!(t = tree_init(music_plain_print, music_cmp))) { 
    return NULL;
  }

  loadBalancedTree_rec(data, t, 0, n - 1);

  return t;
}

BSTree *loadUnbalancedTree(Music **data, int n) {
  BSTree *t;
  Music *m;
  int i;

  if (!data || (n <= 0)) {
    return NULL;
  }

  if (!(t = tree_init(music_plain_print, music_cmp))) {
    return NULL;
  }

  for (i = 0; i < n; i++) {
    m = data[i];
    if (tree_insert(t, m) == ERROR) {
      fprintf(stdout, "Music ");
      music_plain_print(stdout, m);
      fprintf(stdout, " not inserted!\n");
    }
  }

  return t;
}

int qsort_fun(const void *e1, const void *e2){
  Music **pm1, **pm2;

  pm1 = (Music **) e1;
  pm2 = (Music **) e2;

  return music_cmp(*pm1, *pm2);
}
/* END Private methods */


int main(int argc, char const *argv[]) {
	FILE *f_in = NULL, *f_out = NULL;
	BSTree *t = NULL;
	Music **songs = NULL, *m = NULL;
	const char *mode;
	int n = 0;
	long	music_id;
	clock_t elapsed;
	Radio *r = NULL;

	if (argc != 4) {
		printf("Usage: %s music_file music_id mode[normal|sorted]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	mode = argv[3];
	if (strcmp(mode, "normal") && strcmp(mode, "sorted")) {
		printf("Incorrect mode: %s\n", mode);
		exit(EXIT_FAILURE);
	}

    f_in = openInputFile(argv[1]);
    if (!f_in) {
      fprintf(stdout, "Error opening input file: %s\n", argv[1]);
      return (EXIT_FAILURE);
    }
	f_out = stdout;

    r = radio_init();
    if (!r) mainCleanUp (EXIT_FAILURE, r, f_in, songs);
    
    /* lee el fichero */
    if  (radio_readFromFile(f_in, r) == ERROR) {
      fprintf(stdout, "Not file or File format incorrect\n");
      mainCleanUp (EXIT_FAILURE, r, f_in, songs);
    }
	
	music_id = atoi(argv[2]);
	songs = copySongsFromRadio(r, &n);
	if (!songs) {
		fprintf(stdout, "Error copying songs from radio\n");
		mainCleanUp (EXIT_FAILURE, r, f_in, songs);
	}

	m = findMusicById(songs, n, music_id);
	if (m == NULL) {
		printf("Error when initialising music with id: %ld\n", music_id);
		mainCleanUp (EXIT_FAILURE, r, f_in, songs);
	}

	if (!strcmp(mode, "normal")) {
		fprintf(f_out, "Mode: normal\n");
		elapsed = clock();
		t = loadUnbalancedTree(songs, n);
		elapsed = clock() - elapsed;
	}
	else {
		qsort(songs, n, sizeof(Music *), qsort_fun);
		fprintf(f_out, "Mode: sorted\n");
		elapsed = clock();
		t = loadBalancedTree(songs, n);
		elapsed = clock() - elapsed;
	}

  if (!t) {
    mainCleanUp (EXIT_FAILURE, r, f_in, songs);
  }

  fprintf(f_out, "Tree building time: %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);
  fprintf(f_out, "Tree size: %lu\nTree depth: %d\n", (unsigned long)tree_size(t), tree_depth(t));

  fprintf(f_out, "Min element in tree: ");
  elapsed = clock();
  music_plain_print(f_out, tree_find_min(t));
  elapsed = clock() - elapsed;
  fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);

  fprintf(f_out, "Max element in tree: ");
  elapsed = clock();
  music_plain_print(f_out, tree_find_max(t));
  elapsed = clock() - elapsed;
  fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);

  elapsed = clock();
  if (tree_contains(t, m) == TRUE) {
    fprintf(f_out, "Element found");
    elapsed = clock() - elapsed;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);

  /*EXERCISE 2 - TREE_REMOVE
    fprintf(f_out, "Removing element in tree: ");
    elapsed = clock();
    fprintf(f_out, "%s", tree_remove(t, m) == OK ? "OK" : "ERR");
    elapsed = clock() - elapsed;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);
    fprintf(f_out, "Tree size: %lu\nTree depth: %d\n", (unsigned long)tree_size(t), tree_depth(t));
*/
  } else {
    fprintf(f_out, "Element NOT found");
    elapsed = clock() - elapsed;
    fprintf(f_out, " - %ld ticks (%f seconds)\n", (long)elapsed, ((float) elapsed) / CLOCKS_PER_SEC);
  }
  
  tree_destroy(t);
  mainCleanUp (EXIT_SUCCESS, r, f_in, songs);
}

/*
P1:
Los tiempos cambian porque en modo normal el arbol se construye insertando las
canciones en el orden original del fichero, y ese orden puede generar un arbol
desbalanceado. En cambio, en modo sorted primero se ordenan las canciones y
luego se inserta recursivamente la del medio de cada tramo, lo que produce un
arbol mucho mas equilibrado.

La propiedad clave es la altura del BST. Cuando el arbol esta balanceado, su
altura es cercana a log2(n) y las operaciones de insercion y busqueda son mas
rapidas. Cuando el arbol se desequilibra, la altura puede acercarse a n y el
rendimiento empeora hasta parecerse al de una lista lineal.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "radio.h"
#include "stack.h"


int show_player_menu(Stack *history);
int show_player_menu(Stack *history) {
    int option;
    Music *m = NULL;
	
	/* Obtenemos canción actual del top de la pila */
	m = (Music *)stack_top (history);
    if (m != NULL) {
		music_formatted_print(stdout, m);
    } else {
        printf("\nNo song currently playing.\n");
    }
	
	/* Imprimimos historial (pila) */
	printf("\nRecently Played:\n");
	stack_print(stdout, history, music_plain_print);
	
	/* Mostramos menu y esperamos selección */
    printf("\n1. Back to previous song\n");
    printf("2. Exit\n");
    printf("Choose an option: ");

    scanf("%d", &option);

    return option;
}

/* TODO MAIN FUNCTION */
int main (int argc, char **argv) 
{   
    int cmd = 0;
    Radio* radio = NULL;
    Stack* stack = NULL;
    FILE* f = NULL;

    if (!(argv) || argc != 2)
    {
        return 0;
    }
    

    if (!(f = fopen(argv[1], "r")))
    {
        return 0;
    }

    radio = radio_init();
    if (radio == NULL)
    {
        return 0;
    }

    if (radio_readFromFile(f, radio) == ERROR)
    {      
        radio_free(radio);
        return 0;
    }
    fclose(f);
    
    stack = stack_init();
    if (stack == NULL)
    {   
        fprintf(stdout, "Error inicializando la pila");
        radio_free(radio);
        return 0;
    }

    for (int i = 0; i < radio_getNumberOfMusic(radio); i++)
    {
        if (stack_push(stack, radio_get_musicbyindex(radio, i)) == ERROR)
        {
            fprintf(stdout, "Error añadiendo canción a la pila");
            stack_free(stack);
            radio_free(radio);
            return 0;
        }
    }
    
    
    while (1)
    {
        cmd = show_player_menu(stack);
        if (cmd == 2)
        {
            stack_free(stack);
            radio_free(radio);
            break;
            return 0;
        }else if (cmd == 1)
        {
            stack_pop(stack);
        }
        
        
    }
    
    

    
}



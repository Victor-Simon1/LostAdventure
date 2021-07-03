#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "map.h"
#include "play.h"


void loadLine(int i, char line[], int tab[nb_line][nb_column]){ // remplie la ligne i du tableau
	char delim[] = ",";
    char *ptr = strtok(line, delim);
    int j = 0;
	while (ptr != NULL)
	{
        tab[i][j] =  atoi(ptr);
		ptr = strtok(NULL, delim);// on a besoin de le mettre a null pour qu'il passe au prochain token
        j++;
	}

}

void loadMap(char nom[], int tab[nb_line][nb_column]){ // lit chaque ligne de la map dans le fichier map.txt

	FILE *file = fopen(nom, "r");
    int i = 0;

	if (file != NULL){
		char line[524];
		while (fgets(line, sizeof line, file) != NULL)
		{
            loadLine(i, line, tab);
            i++;
		}
		if(fclose(file) != EOF);
        else fprintf(stderr, "Erreur fclose\n");
	}
	else
	{
		perror(nom);
	}

}

void initMapDecouverte(){
    minimap = false;
    for (int ligne = 0; ligne < nb_line; ligne++){
        for (int colonne = 0; colonne < nb_column; colonne++){
            if((ligne == 0) || (ligne == nb_line-1) || (colonne == 0) || (colonne == nb_column-1)){
                map_decouverte[ligne][colonne] = 1;
            }
            else{
                map_decouverte[ligne][colonne] = 1;
            }
        }
    }
}
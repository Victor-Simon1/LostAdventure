#ifndef MAP_H
#define MAP_H
#include "game.h"
#include "play.h"

void loadMap(char nom[], int tab[nb_line][nb_column]);
void loadLine(int i, char line[], int tab[nb_line][nb_column]);
void initMapDecouverte();

int map_test[nb_line][nb_column];
int map_lumiere[nb_line][nb_column];
int map_decouverte[nb_line][nb_column];

#endif
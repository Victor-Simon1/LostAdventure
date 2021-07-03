#ifndef LOAD_H
#define LOAD_H
#include "game.h"

int load(); 

SDL_Texture *LoadTexture2(SDL_Renderer *renderer, char *path); // fonction qui charge une image
Game game;
Fps fps;

SDL_Texture *textureBG;
SDL_Rect list_texture[200]; // contient tous les tiles

#endif
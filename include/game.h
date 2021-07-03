#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "SDL_image.h"

#define largeur_tile 32
#define hauteur_tile 32
#define nb_line 30
#define nb_column 50

typedef struct {
    SDL_Window *windows;
    SDL_Renderer *renderer;
    int screenWidth;
    int screenHeight;
    int camera_x; 
    int camera_y;
}Game;


typedef struct {
    uint32_t delta; // temps passé entre chaque frame
    uint32_t last_tick_time;
    uint32_t tick_time;
    int nb_fps; // nombre de frame par seconde
    int compteur;
}Fps;

#endif
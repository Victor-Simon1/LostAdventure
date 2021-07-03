#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>

typedef struct { //     PLAYER
    int x;
    int y;
    int vx;
    int vy;
    int w;
    int h;
    int vxmax;
    int vymax;
    int frottement;
    int acceleration;
    int spd_anim;
    SDL_Texture *current_texture; // texture de l animation en cour
    SDL_Rect *animation; // partie(rectangle) de la texture courante a l afficher
    double frame;
    int nb_frame;
    char nom[15];
    char etat[15];
    char regardX[10];
    char regardY[10];
}player;


player creerPlayer(char nom[15]); // fonction qui crée le player

SDL_Rect animation_idle[8]; // tableaux contenant chaque position des images (dans la tilesheet du player) pour en faire une animation
SDL_Rect animation_left[7];
SDL_Rect animation_right[7];
SDL_Rect animation_up[8];
SDL_Rect animation_down[8];


#endif
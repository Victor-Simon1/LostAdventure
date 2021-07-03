#ifndef TIR_H
#define TIR_H

#include "game.h"
#include <SDL.h>

typedef struct List_ {
    double x;
    double y;
    int w;
    int h;
    double vx;
    double vy;
    double distance;
    double distance_max;
    Direction direction;
    Type type;
    struct List_ *suiv;
    struct List_ *prec;
    SDL_Texture *img;
}Tir, *Tirs;

Tirs Liste_tirs; // liste de tirs

Tirs createTir(Direction p_dir, Type p_type, int pX, int pY, int p_spd);
Tirs ajouter_teteTirs(Tirs C);
Tirs supprimTirs(Tirs tir);
void libererTir(Tirs tir);
void libereListeTirs(Tirs tir);

#endif

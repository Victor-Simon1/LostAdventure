#ifndef ANIMAUX_H
#define ANIMAUX_H
#include <SDL.h>
#include "SDL_image.h"
#include <SDL_mixer.h>
#include "game.h"
#include <stdbool.h>


typedef enum {
    VACHE , POULE
}Espece;

typedef struct ListeA{
    double x;
    double y;
    double oldX;
    double oldY;
    double vie;
    Espece espece;
    struct ListeA *suiv;
    struct ListeA *prec;
    Animation anim;
    double vx;
    double vy;
    double vxmax;
    double vymax;
    double acceleration;
    double frottement;
    double targetX;
    double targetY;
    double frame;
    int nbFrame;
    Etat etat;
    Type type;
    Animation animation;
    SDL_Rect hitbox;
    int rangeRandom;
    double timer_fuite;
}animal , *Animaux;

Animaux listeAnimaux;
SDL_Texture *textureAnimaux;

Animaux createAnimal(Espece esp,double pX,double pY);
Animaux ajoutAnimaux(Animaux liste,Animaux new);
Animaux enleveAnimaux(Animaux old);
void setTargetAnimal(Animaux a);
bool collideAnimauxMap(Animaux a,double oldX, double oldY,double oldBoxX,double oldBoxY);
void chargerBoxAnimaux(Animaux a);
void libererAnimaux(Animaux old);
bool collideAnimauxPlayer(Animaux a, double oldX, double oldY, double oldBoxX, double oldBoxY);
bool collideAnimauxEnnemis(Animaux a,double oldX, double oldY,double oldBoxX,double oldBoxY);
void libererListeAnimaux(Animaux a);
void updateAnimaux(Animaux a);

#endif

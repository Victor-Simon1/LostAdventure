#ifndef ENNEMIS_H
#define ENNEMIS_H

#include "game.h"
#include <SDL.h>

typedef struct ListeE{
    int vie;
    int viemax;
    double x;
    double y;
    double oldX;
    double oldY;
    double vx;
    double vy;
    int vxmax;
    int vymax;
    int acceleration;
    int frottement;
    int targetX;
    int targetY;
    int rangeDetection;
    int rangeRandom;
    int rangeLost;
    int rangeAtk;
    bool isInDonjon;
    int tabDrop[5];
    Etat etat;
    Type type;
    Animation animation;
    SDL_Rect hitbox;
    struct ListeE *suiv;
    struct ListeE *prec;
    double frame;
    int npFrame;
    int timer_max;
    double timer_tir;
}Ennemie, *Ennemies;

Ennemies listeEnnemie; // liste d'ennemis

void moveTo2(Ennemies e,int move);
int random(int min ,int max);

void libereListeEnnemi(Ennemies e);
void libererEnnemi(Ennemies e);
Ennemies creerEnnemie(Type p_type, int p_x, int p_y, bool p_donjon);
bool collideEnnemisAnimaux(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY);
void updateEnnemis(Ennemies e, int move);
void chargerBox(Ennemies e);
void setTarget(Ennemies e);
Ennemies ajouter_teteEnn(Ennemies liste, Ennemies C);
Ennemies supprimEnnemis(Ennemies *list, Ennemies e);
bool collideEnnemis(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY);
bool collideRectEnnemis(Ennemies liste, SDL_Rect rect_box);

#endif
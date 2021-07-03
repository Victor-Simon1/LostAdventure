#ifndef FONCTIONS_H
#define FONCTIONS_H
#include "game.h"
#include "donjon.h"
#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "ennemis.h"
#include "animaux.h"



int update();
int draw();

int lireRecolter();

void affichageInventaire(int nb);
int collidePtMap(int x, int y);
int isSolide(int id);
bool collideRectDonjon(Donjons tab[], SDL_Rect rect, int nb);
int collideRectMap(SDL_Rect rect);
int appartient(int id,int *tabSolide, int taille);
void verifSolide(int *tilesSolide, int taille);
int distance2Pts(int p1_x, int p1_y, int p2_x, int p2_y);
int collideRectRect(SDL_Rect rect, SDL_Rect rect2);
bool collideEnnemisPlayer(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY);
bool collideEnnemisMap(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY);
bool collideRectAnimaux(Animaux liste,SDL_Rect rect_box);
void move(double moveX, double moveY);

// objets
Obj createObjet(int id, int pX, int pY);
Obj  ajouter_tete(Obj liste, Obj C);
Obj supprim(Obj *liste, Obj obj);
void libererObjet(Obj obj);
void libereListeObjet(Obj obj);

int collidePtRect(int x, int y,SDL_Rect rect);
int collideDonjon(Salles ma_salle, SDL_Rect rect);
TTF_Font* LoadFont(char police[] , int taille);
SDL_Texture* CreateFont(SDL_Renderer *renderer,TTF_Font* police, char message[] ,SDL_Color couleur);

SDL_Texture *LoadTexture2(SDL_Renderer *renderer, char *path); // fonction qui charge une image
int eventClavier(bool *attrape,Case **ObjAttr,int *ancienInvSelect,float *x, float *y);
int nbLignesinFile(char nomFichier[]);
TypeObjet determinerObjet(int p_id);
void respawnPlayer();
// ennemis


// ressources

/*
int updateMenu();
int drawMenu();

int updateSettings();
int drawSettings();*/

#endif
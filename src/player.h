#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include "SDL_image.h"
#include "game.h"
#include "craft.h"

#define LIGNE_INV 3
#define COLONNE_INV 9

typedef struct { //     PLAYER
    float x;
    float y;
    int vx;
    int vy;
    int w;
    int h;
    int vxmax;
    int vymax;
    int frottement;
    double protection;
    int acceleration;
    double vie;
    int vie_max;
    int spd_anim;
    int id_donjon;
    bool isInDonjon;
    double xDonjon;
    double yDonjon;
    int oldCamX;
    int oldCamY;
    SDL_Texture *current_texture; // texture de l animation en cour
    SDL_Texture *tex_lvl;
    double frame;
    int nb_frame;
    char nom[15];
    double timer_max; 
    double timer_tir;
    Type type;
    Animation animation;
    tempPlayer temperature;
    char regardX[10];
    char regardY[10];
    Direction direction;
    Inventaire inv;
    double barre_soif;
    double barre_manger;
    double respawnX;
    double respawnY;
    double max_faim;
    double max_soif;
    Biome biome;
    double barre_xp;
    int xp_max;
    int xp_lvl;
}Player;

Player mon_player;
SDL_Texture *textureBarre;
SDL_Texture *textureBarreFond;
SDL_Texture *textureManger;
SDL_Texture *textureSoif;
SDL_Texture *textureFond;
SDL_Texture *textureXp;
Player creerPlayer(char nom[15]); // fonction qui crée le player

void initialiserInventaire();
void echangerCase(Case *Case1 , Case *Case2);
void poserInInventory(int x , int y ,Case *caseSelect);
void libererTextureInventaire();
bool attraperObjet(int x,int y,Case **caseSelect);
int ajoutInventaire(Obj objet, Player *p);
bool lacherObjet(Obj objet, int x, int y);
bool AjoutObjCase(Case *case1, Obj objet);
void depence();
int recherche(int p_id, ObjCraft obj);
void replace(Case *case1);
bool verifCollidePlayer(double oldX, double oldY, double oldCamX, double  oldCamY);
bool restaurationPlayer();
void detecterBiome();
void gestionTemperature();
#endif
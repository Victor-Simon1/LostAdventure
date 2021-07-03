#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include "SDL_image.h"
#include <stdbool.h>
#include "SDL_ttf.h"

#define largeur_tile 32
#define hauteur_tile 32
#define nb_line 70
#define nb_column 100
#define largeur_map nb_column*largeur_tile
#define hauteur_map nb_line*hauteur_tile
#define MAX 15
#define PI 3.14159265358979224

typedef enum {
    IDLE, LEFT, RIGHT, UP, DOWN, TIREG, TIRED, TIREUP, TIREDOWN
}Animation;

typedef enum {
    D_LEFT, D_RIGHT, D_UP, D_DOWN
}Direction;

typedef enum {
    PLAYER, ENNEMI1, ANIMAL
}Type;

typedef enum {
    RESSOURCE
}TypeObj;

typedef enum {
    PRINCIPAL, MAIN, CRAFT, ARMURE, OBJ_A_CRAFT
}TypeCase;

typedef enum{
    WALK, TARGETPLAYER, ARC,FUITE
}Etat;

typedef enum{
    ARMURES,NOURRITURE,BOISSON,MINERAI,OBJET
}TypeObjet;

typedef struct {
    int x;
    int y;
    int w;
    int h;
}Camera;


typedef struct {
    SDL_Window *windows;
    SDL_Renderer *renderer;
    Camera camera;
}Ecran;

typedef struct {
    int jour;
    int heure;
    float minutes;
    float minutesPrec;
    char message[255];
    int speed;
    float alpha;
    float speed_alpha;
    int max_alpha;
    float speed_time;
}Journee;

typedef struct{
    SDL_Color noire;
    SDL_Color rouge;
}Couleur;

typedef struct {
    float delta; // temps passé entre chaque frame en seconde
    uint32_t last_tick_time;
    uint32_t tick_time;
    int nb_fps; // nombre de frame par seconde
    float compteur;
}Fps;

typedef struct {
    Ecran ecran;
    Fps fps;
    Couleur couleur;
    Journee journee;
    char affichage[20];
    bool etatPause;
}Game;

typedef struct _Liste  {
    char name[15];
    int id;
   // char type[15];
   TypeObjet type;
    int nb;
    double protection;
    SDL_Texture *texText;
    SDL_Rect rect;
    struct _Liste *suiv;
    struct _Liste *prec;
}Objet,*Obj;

typedef struct {
    Obj obj;
    SDL_Rect rect;
    int id;
    bool isFilled;
    bool isSelect;
    TypeCase type;
    SDL_Texture *fond;
}Case;

typedef struct {
    Case inventairePrin[3][9];
    SDL_Rect rectPrin;
    Case inventairemain[9];
    SDL_Rect rectMain;
    Case armure[4];
    SDL_Rect rectArmure;
    Case craft[3];
    SDL_Rect rectCraft;
    Case caseObjCraft;
    SDL_Rect rectObjCraft;
    SDL_Rect inventory;
    SDL_Texture *fond;
}Inventaire,*Inv;

typedef enum{
    NEIGE,PLAINE,DESERT,DONJON
}Biome;

typedef enum{
    FROIDGLACIAL,FROID,AMBIANT,CHAUD,DESERTIQUE
}tempPlayer;

TTF_Font *police;

SDL_Texture *selectInv; // texture pour la case de l inventaire main selectionné
SDL_Texture *textText; // texture pour le texte (heure)
SDL_Texture *textureBG; // texture de la tilesheet (map)
SDL_Texture *textEN; // texture de la tilesheet (ennemi)
SDL_Texture *textObj; // texture de la tilesheet (objet)
SDL_Texture *texAlerte;
SDL_Texture *textPause;
Game game;
Obj liste_objets; // liste d'objet
SDL_Event event;
int temperature;
bool running;
int nb_tiles;

int nbObjetCraftable;

int tabArmure[10];

/*
    Continent tous les animations
    tab_animations[PLAYER][IDLE][] contient les animations IDLE du PLAYER
    ...
    tab_animations[ENNEMI1][IDLE][] contient les animations IDLE du ENNEMI1
    tab_animations[ENNEMI1][LEFT][] contient les animations LEFT du ENNEMI1
    ...
*/
SDL_Rect tab_animations[3][9][9]; 

SDL_Rect tab_rect_obj[20];





#endif
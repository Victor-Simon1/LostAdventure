#ifndef GUI_H
#define GUI_H
#include <SDL.h>
#include "SDL_image.h"
#include <stdbool.h>
#include "SDL_ttf.h"
#include <SDL_mixer.h>

typedef struct button_{
    bool isPressed;
    bool oldPressed;
    bool isSurvol;
    SDL_Texture *img;
    SDL_Texture *imgSurvol;
    SDL_Texture *imgPressed;
    SDL_Rect box;
    int (*fct)(char[20]);
    char para[20];
    Mix_Chunk *hover;
    struct button_ *suiv;
    struct button_ *prec;
}Button,*Buttons;


typedef struct group_{
    bool actif;
    Buttons listeButtons;
    struct group_ *suiv;
    struct group_ *prec;
}Groupe, *Groupes;


Groupes listeGroupes;
Groupes groupeMenu;
Groupes groupeSetting;
Groupes groupeGame;
Groupes groupePause;


Button playButton;
Button settingsButton;
Button menuButton;

SDL_Texture *menuImg;
SDL_Texture *affSettingsImg;
SDL_Rect menuRect;

Mix_Music *myMus;
Mix_Chunk *playerWalk;
Mix_Chunk *breakWoods;
Mix_Chunk *rammaseObjet;

int changeAffichage(char affichage[20]);
void affichageJauge(int p_x, int p_y, int p_w, int p_h, double p_actu, double p_max, SDL_Texture *p_texFond,  SDL_Texture *p_texRemplissage);
int resetGroupe();
Buttons creerBouton(char *p_img, char *p_imgSurvol, char *p_imgPressed, int p_x, int p_y, int p_w, int p_h, int (*p_fonc)(char[20]), char* p_param);
Buttons ajouter_teteButton(Buttons liste, Buttons C);
Buttons supprimButton(Buttons *liste, Buttons mon_bouton);
void freeButton(Buttons my_button);
void freeListeButtons(Buttons my_button);

Groupes creerGroupe();
Groupes ajouter_teteGroupes(Groupes liste, Groupes C);
Groupes supprimGroupe(Groupes *liste, Groupes mon_bouton);
void freeGroupe(Groupes my_button);
void freeListeGroupe(Groupes my_button);

int updateGui();
int drawGui();


#endif 
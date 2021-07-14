#ifndef DONJON_H
#define DONJON_H
#include "game.h"
#include "ennemis.h"

#define nb_donjon 5
#define largeurMur 90
#define hauteurMur 115
#define largeurPorte 120
#define hauteurPorte 95

typedef struct porte_{
    char type[20];
    SDL_Rect box;
    struct porte_ *suiv;
}Porte, *Portes;

typedef struct{
    int ligne;
    int colonne;
    int numSalle;
    bool porteHaut;
    bool porteBas;
    bool porteGauche;
    bool porteDroite;
    bool porteSortie;
    bool open;
    int nbEnnemis;
    Portes portes;
    Ennemies liste_ennemis;
    Obj liste_objets;
}Salle, *Salles;

typedef struct{
    SDL_Rect box;
    Salles tab_salles[5][7];
    int nbSalles;
    int ligneActu;
    int colonneActu;
    int nb_ennemis;
    int departX;
    int departY;
    SDL_Texture *img_fond;
    SDL_Texture *img_donjon;
}Donjon, *Donjons;

Donjons creerdonjon(int p_x, int p_y, int p_type, int n);
Salles creerSalle(int i, int j);
int genererSalles(Salles tab_salles[5][7], int n, int *ligneActu, int *colonneActu);
void libereSalle(Salles salle);
void libereDonjon(Donjons donjon);
void libereTabDonjon();

void moveInDonjon(double moveX, double moveY);
void genererEnnemisDonjon(int l, int c);

void sortiDonjon();
int loadDonjon(Donjons mon_donjon, int p_id);
int updateDonjon();
int drawDonjon();

Donjon *tab_donjon[nb_donjon];
bool miniMap;

// texture des portes
SDL_Texture *porteG;
SDL_Texture *porteD;
SDL_Texture *porteH;
SDL_Texture *porteB;

#endif
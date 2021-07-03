#ifndef CRAFT_H
#define CRAFT_H
#include "game.h"

typedef struct{
    int idObjC;
    int id1;
    int nb_id1;
    int id2;
    int nb_id2;
    int id3;
    int nb_id3;
}ObjCraft;

ObjCraft tab[MAX];

int rechercheDicoInserer(ObjCraft obj ,int bInf,int bSup,int tailleTab);
int rechercheDico(ObjCraft obj ,int bInf,int bSup);
void insererInTab(ObjCraft obj,int tailleTab);
int lectureFile(char nomFichier[]);
int nbObjetinFIle(char nomFichier[]);
void trieIdObjet(ObjCraft *obj);


#endif
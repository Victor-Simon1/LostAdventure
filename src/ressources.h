#ifndef RESSOURCES_H
#define RESSOURCES_H

#include "game.h"

typedef struct{
  int idCasse;
  int idDrop1;
  int idDrop2;
  int idRemplace;
}Ressource;

Ressource tabRessource[MAX];

void detecterREssource(int mouseX,int mouseY);
int rechercheDicoInsereRessource(Ressource res ,int bInf,int bSup,int tailleTab);
int rechercheDicoRessource(Ressource res ,int bInf,int bSup);
void insererInTabRessource(Ressource res,int tailleTab);
void triIdRessource(Ressource *res);

#endif
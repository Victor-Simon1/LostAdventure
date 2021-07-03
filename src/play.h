#ifndef PLAY_H
#define PLAY_H
#include "gui.h"

bool inventaire;
bool minimap;
bool inDonjon;

Mix_Music *donjonMusic;
Mix_Chunk *bowSound ;
int loadGame();
int libereGame();
#endif
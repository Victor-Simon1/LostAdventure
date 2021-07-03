#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "gui.h"


Button playButton;
Button settingsButton;
Button menuButton;

SDL_Texture *menuImg;

SDL_Rect menuRect;

void libereMenu();
int loadMenu();
int drawMenu();
int updateMenu();
#endif
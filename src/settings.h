#ifndef SETTINGS_H
#define SETTINGS_H
#include "gui.h"

Button settingsMenuButton;
SDL_Texture *affSettingsImg;
int updateSettings();
void loadSettings();
void libererSettings();
int drawSettings();
#endif
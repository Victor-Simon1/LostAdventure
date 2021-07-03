#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include "SDL_image.h"

#include "game.h"
#include "load.h"
#include "map.h"
#include "player.h"
#include "fonctions.h"
#include "gui.h"
#include "donjon.h"
#include "tirs.h"
#include "menu.h"
#include "settings.h"
#include "animaux.h"
/*
  _        ___       _      ____  
 | |      / _ \     / \    |  _ \ 
 | |     | | | |   / _ \   | | | |
 | |___  | |_| |  / ___ \  | |_| |
 |_____|  \___/  /_/   \_\ |____/ 
                                
*/

int load(){


    // font
    police = LoadFont("src/font/arial.ttf",20);
    if(police == NULL)
        return -1;


   
   // ==================================== INITIALISATION VARIABLES ====================================

    running = true;
   



   
    // ==================== GUI ==================

    loadMenu();
    loadGame();
    loadSettings();

    strcpy(game.affichage,"MENU");
   
    return 1;
}

/* 
charge les images des annimations dans le tableau
parametre : 
    animi -> precise l animation (left, right, ...)
    type ->  pour qui est l animation (player ennemie,..) 
    x -> coordonée X de depart
    y -> coordonée y de depart
    w -> largeur d une image
    h -> hauteur d une image
    limit -> nombre d image pour cette animation
    mode -> 1 si contient les animations ligne par ligne, 2 si par colonne
*/
void loadImg(int type, int anim, int x, int y, int w, int h, int limit, int mode){  
    int i = 0, j = 0, n = 0;

    if(mode == 1){
        for(i = 0; i < limit; i++){
            SDL_Rect rectangle1 = {x, y, w, h};
            tab_animations[type][anim][n] = rectangle1;
            n++;    
            x += w;
        }
    }
    else{
        for(i = 0; i < limit; i++){
            SDL_Rect rectangle1 = {x, y, w, h};
            tab_animations[type][anim][n] = rectangle1;
            n++;    
            y += h;
        }
    }
}



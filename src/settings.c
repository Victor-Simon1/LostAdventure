
#include "settings.h"
#include "game.h"
#include "gui.h"
#include "menu.h"
#include "SDL_image.h"
#include "fonctions.h"

int updateSettings(){
     updateGui();
    return 1;

}

void loadSettings(){
    groupeSetting = creerGroupe();
    Buttons menuButton = creerBouton("src/gfx/menuButton.png", "src/gfx/menuButtonHover.png", "src/gfx/menuButtonHover.png", 
                                                                                                game.ecran.camera.w/2 , 
                                                                                                game.ecran.camera.h/2 , 
                                                                                                150, 50, 
                                                                                                &changeAffichage,
                                                                                                "MENU");
    
    menuButton->box.x -= menuButton->box.w/2;
    menuButton->box.y -= menuButton->box.h/2;
    groupeSetting->listeButtons = ajouter_teteButton(groupeSetting->listeButtons , menuButton);
    listeGroupes = ajouter_teteGroupes(listeGroupes, groupeSetting);
   
    affSettingsImg = LoadTexture2(game.ecran.renderer,"src/gfx/settings.png"); 
}

void libererSettings(){

}

int drawSettings(){
    SDL_RenderClear(game.ecran.renderer); // Efface l'écran
   
    SDL_Rect Rect = {0, 0, game.ecran.camera.w, game.ecran.camera.h};
    SDL_RenderCopy(game.ecran.renderer,affSettingsImg,NULL,&Rect);
    drawGui();
    SDL_RenderPresent(game.ecran.renderer); // Présente l'écran
    return 1;
}


#include "menu.h"
#include "game.h"
#include "gui.h"
#include "fonctions.h"
#include "SDL_mixer.h"
#include <SDL.h>
#include "SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int updateMenu(){ // mise a jour du jeu dans le menu

     updateGui();
    

    return 1;

}  

int loadMenu(){ // charge le menu
    groupeMenu = creerGroupe();

    Buttons playButton = creerBouton("src/gfx/playButton.png", "src/gfx/boutonJouerHover.png", "src/gfx/boutonJouerHover.png", 
                                                                                                  game.ecran.camera.w/2 , 
                                                                                                game.ecran.camera.h/2 , 
                                                                                                150, 50, 
                                                                                                &changeAffichage,
                                                                                                "GAME");
    groupeMenu->actif = true;
   playButton->box.x -= playButton->box.w/2;
   playButton->box.y-= playButton->box.h/2;
    listeGroupes = ajouter_teteGroupes(listeGroupes, groupeMenu);

    groupeMenu->listeButtons =ajouter_teteButton(groupeMenu->listeButtons , playButton);
    
    menuImg = LoadTexture2(game.ecran.renderer,"src/gfx/ImgFond.png");
  
    
    //Ouvric un evice audio

    Mix_AllocateChannels(10); // nb max de son géré a la fois
    Mix_VolumeMusic(MIX_MAX_VOLUME/10);
    Mix_Volume(-1, MIX_MAX_VOLUME/10);

    playerWalk = Mix_LoadWAV("src/son/Fantozzi-SandL1.ogg");
    myMus = Mix_LoadMUS("src/son/menu.wav");
 
    breakWoods = Mix_LoadWAV("src/son/bfh1_wood_breaking_01.ogg");
    rammaseObjet = Mix_LoadWAV("src/son/coin.ogg");

    Mix_PlayMusic(myMus,-1);// lancer la musique du menu

   
    Buttons settingsButton = creerBouton("src/gfx/boutonCommande.png", "src/gfx/boutonCommandeHover.png", "src/gfx/boutonCommandeHover.png", 
                                                                                                playButton->box.x, 
                                                                                                 playButton->box.y+80 , 
                                                                                                150, 50, 
                                                                                                &changeAffichage,
                                                                                                "SETTINGS");
    
    
    groupeMenu->listeButtons = ajouter_teteButton(groupeMenu->listeButtons , settingsButton);
    // retour menu

     Buttons quitButton = creerBouton("src/gfx/boutonQuitter.png", "src/gfx/boutonQuitterHover.png", "src/gfx/boutonQuitterHover.png", 
                                                                                                playButton->box.x , 
                                                                                                playButton->box.y+160 , 
                                                                                                150, 50, 
                                                                                                &changeAffichage,
                                                                                                "QUIT");

   
    
    groupeMenu->listeButtons = ajouter_teteButton(groupeMenu->listeButtons , quitButton);

    return 1;
}

void libereMenu(){


    SDL_DestroyTexture(menuImg);

    //Liberation des musique et sons 
    Mix_FreeChunk(playerWalk);
    Mix_FreeChunk(breakWoods);
    Mix_FreeChunk(rammaseObjet);
    Mix_FreeMusic(myMus);

    myMus = NULL;
    rammaseObjet = NULL;
    
    breakWoods = NULL;
    menuImg = NULL;
    playerWalk = NULL;
}


int drawMenu(){

    SDL_RenderClear(game.ecran.renderer); // Efface l'écran 
    SDL_Rect Rect = {0, 0, game.ecran.camera.w, game.ecran.camera.h};
    SDL_RenderCopy(game.ecran.renderer,menuImg,NULL,&Rect);
     
    drawGui();
    SDL_RenderPresent(game.ecran.renderer); // Présente l'écran
    return 1;   
}

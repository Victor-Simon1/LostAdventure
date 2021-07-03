#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL.h>
#include "SDL_image.h"
#include <math.h>
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "player.h"
#include "fonctions.h"
#include "load.h"
#include "gui.h"
#include "donjon.h"
#include "play.h"
#include "settings.h"
#include "menu.h"
#include "tirs.h"
#include "ressources.h"
#include "animaux.h"


/*

  __  __      _      ___   _   _ 
 |  \/  |    / \    |_ _| | \ | |
 | |\/| |   / _ \    | |  |  \| |
 | |  | |  / ___ \   | |  | |\  |
 |_|  |_| /_/   \_\ |___| |_| \_|


*/

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    
    game.ecran.camera.h = 768; // hauteur ecran
    game.ecran.camera.w = 1024; // largeur ecran

    // ===================================== gestion SDL =====================================

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) // initialise la SDL
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        return -1;
    }
    if(TTF_Init() == -1){ // initialise TTF pour les textes
        printf("SDL_ttf could not initialize! SDL_ttf Error:%s\n",TTF_GetError());
        return -1;
    }
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){ //Initialisation de l'API Mixer
        printf("%s", Mix_GetError());
    }

    game.ecran.windows = SDL_CreateWindow("Lost Adventure",                         // création de la fenetre
                                            SDL_WINDOWPOS_UNDEFINED, // x
                                            SDL_WINDOWPOS_UNDEFINED, // y
                                            game.ecran.camera.w, // largeur
                                            game.ecran.camera.h, // hauteur
                                            SDL_WINDOW_SHOWN); // visible

    if (game.ecran.windows == NULL)
    {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return -1;
    }

    game.ecran.renderer = SDL_CreateRenderer( game.ecran.windows, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // SDL_RENDERER_ACCELERATED qui permet au renderer de travailler avec l'acceleration materielle
    if (game.ecran.renderer == NULL)
    {
        SDL_Log("Could not create a renderer: %s", SDL_GetError());
        return -1;
    }
    else
    {
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image error: %s\n", IMG_GetError());
            return -1;
        }
    }

    SDL_SetRenderDrawBlendMode(game.ecran.renderer, SDL_BLENDMODE_BLEND); // activer la transparence
   
    // ===================================== variables =====================================

    double delay = 0;
    int ancienInvSelect = 0; // numero de l'ancienne case selectionnée
    int SourisDown = 0;
    float x = 0.0, y = 0.0;
    SDL_Event event;
    Case *ObjAttr = NULL; // case qui contient objet attrapé par la souris (dans l'inventaire)
    bool attrape = false; // si un objet est attrapé ou non

    if(load() == -1){ // chargement du jeu
        running = false; 
    }

    // ============================= GAME LOOP =============================
   
    while (running){
        // =========== delta time ===============
        game.fps.last_tick_time = game.fps.tick_time;        
        game.fps.tick_time = SDL_GetTicks(); 
        game.fps.delta = game.fps.tick_time - game.fps.last_tick_time; // delta entre les 2 derniers affichage
        game.fps.delta = game.fps.delta/1000; // tranforme en seconde
        game.fps.compteur += game.fps.delta;
        game.fps.nb_fps++;
        
        if(game.fps.compteur >= 1){ // si 1 seconde écoulée
            game.fps.compteur = 0;
            //SDL_Log("======================== fps : %d ==========================", game.fps.nb_fps);
            game.fps.nb_fps = 0;
        }   

        if(strcmp(game.affichage, "GAME") == 0){ // si on est en jeu
            if(update() == -1 || draw() == -1){
                running = false;      
                break;      
            }
        }
        else if(strcmp(game.affichage, "MENU") == 0){ // si on est dans le menu
            if(updateMenu() == -1 || drawMenu() == -1){
                running = false;      
                break;      
            }
        }
        else if(strcmp(game.affichage,"SETTINGS") == 0){ // si on est dans les parametres
            if(updateSettings() == -1 || drawSettings() == -1){
                running = false;
                break;
            }
        }

     
        eventClavier(&attrape,&ObjAttr,&ancienInvSelect,&x,&y);
        

        // =================== bride les fps du programme ===================

        delay = 1.0/60.0;
       // printf("delta : %lf\n", game.fps.delta);
       // printf("delay : %lf\n", delay);
        if(game.fps.delta < delay){ // si on est plus rapide que 60 fps
            SDL_Delay((delay - game.fps.delta)*1000); // on attend
        }
    }

    SDL_DestroyTexture(textText); // texture texte (heure)
    SDL_DestroyTexture(mon_player.current_texture); // tilesheet player
    SDL_DestroyTexture(mon_player.tex_lvl); // texte niveau
    SDL_DestroyTexture(textureBG); // tilesheet map
    SDL_DestroyTexture(selectInv); // image case selectionné
    SDL_DestroyTexture(textEN); // tilesheet ennemi
    SDL_DestroyTexture(textObj); // tilesheet objet
    SDL_DestroyTexture(texAlerte); // image alerte
    SDL_DestroyTexture(porteG); // porte du donjon
    SDL_DestroyTexture(porteB);
    SDL_DestroyTexture(porteD);
    SDL_DestroyTexture(porteH);
    SDL_DestroyTexture(textureAnimaux);
    SDL_DestroyTexture(textPause);
    SDL_DestroyTexture(textureXp);
    libererTextureInventaire(); // libere l'inventaire
    libereListeObjet(liste_objets); // libere la liste d'objet
    libereListeEnnemi(listeEnnemie); // libere la liste d'ennemis
    libereListeTirs(Liste_tirs); // libere la liste des tirs
    libereTabDonjon();
    libereMenu();
    libererListeAnimaux(listeAnimaux);
    liste_objets = NULL;
    listeEnnemie = NULL;
    Liste_tirs = NULL;
    libereGame();
    libereMenu();
    libererSettings();
    freeListeGroupe(listeGroupes);
    TTF_CloseFont(police);
    SDL_DestroyRenderer(game.ecran.renderer);
    SDL_DestroyWindow(game.ecran.windows);

    textEN = NULL;
    textObj = NULL;
    texAlerte = NULL;
    textText = NULL;
    textureXp = NULL;
    police = NULL;
    textPause = NULL;
    mon_player.current_texture = NULL;
    textureBG = NULL;
    selectInv = NULL;
    game.ecran.renderer = NULL;
    game.ecran.windows = NULL;

    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
    Mix_CloseAudio();

    return 0;
}

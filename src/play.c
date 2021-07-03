#include "play.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "donjon.h"
#include "ennemis.h"
#include "tirs.h"
#include "fonctions.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "load.h"
#include "animaux.h"

int loadGame(){ // charge le jeu

    inDonjon = false; // player hors d'un donjon
    inventaire = false;// inventaire inactif

    // ================== MAP ==================

    loadMap("src/map.txt", map_test); // charge la map pour les tiles
    loadMap("src/map_eclair.txt", map_lumiere); // charge la map pour l'eclairage

    textureBG = LoadTexture2(game.ecran.renderer, "src/gfx/tilesheet.png");// crée la texture de la tilesheet (contenant toutes les tiles)
    int textureBG_w, textureBG_h;
    SDL_QueryTexture(textureBG, NULL, NULL, &textureBG_w, &textureBG_h);// recupére la largeur/hauteur de l'image

    nb_tiles = (textureBG_w/largeur_tile) * (textureBG_h/hauteur_tile);// nombre de tiles dans la tilesheet
    int tilesSolide[] = {1,19,16,20,21,55,70,88,91,124,145,151}; // contient les id des tiles solide
    verifSolide(tilesSolide, sizeof(tilesSolide)/sizeof(int)); // va identifier toutes les tiles qui sont solides

    // mini map
    initMapDecouverte();

    
    // ================== PLAYER ==================

    mon_player = creerPlayer("player1");

   SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1}; // box de collision du player

    // ================== inventaire ==================

   
    nbObjetCraftable = lectureFile("src/objet.txt");
    if(nbObjetCraftable == -1){
        SDL_Log("Il y a trop d objet craftable dans le fichier ");
        return -1;
    }

    selectInv = LoadTexture2(game.ecran.renderer, "src/gfx/inventoryHover.png"); // texture case selectionnée
   
    // ==================== GUI =======================
    groupeGame = creerGroupe();
    listeGroupes = ajouter_teteGroupes(listeGroupes, groupeGame);

    Buttons menuButton2 = creerBouton("src/gfx/menuButton.png", "src/gfx/menuButtonHover.png", "src/gfx/menuButtonHover.png", 
                                                                                                game.ecran.camera.w/2 , 
                                                                                                game.ecran.camera.h/2 , 
                                                                                                150, 50, 
                                                                                                &changeAffichage,
                                                                                                "MENU");
    menuButton2->box.x -= menuButton2->box.w/2;
    menuButton2->box.y -= menuButton2->box.h/2 -135;

    groupeGame->listeButtons = ajouter_teteButton(groupeGame->listeButtons , menuButton2);

    Buttons settingsButton2 = creerBouton("src/gfx/boutonCommande.png", "src/gfx/boutonCommandeHover.png", "src/gfx/boutonCommandeHover.png", 
                                                                                                menuButton2->box.x , 
                                                                                                menuButton2->box.y + 80 , 
                                                                                                150, 50, 
                                                                                                &changeAffichage,
                                                                                                "SETTINGS");
 

    groupeGame->listeButtons = ajouter_teteButton(groupeGame->listeButtons , settingsButton2);
  
    game.etatPause = false;
    textPause = LoadTexture2(game.ecran.renderer,"src/gfx/pause.png");

    //Heure dans le jeu    
    game.journee.heure =  12;
    game.journee.jour = 0;
    game.journee.minutes = 0.0;
    game.journee.minutesPrec = 0.0;
    game.journee.speed_time = 6.0;
    game.journee.speed_alpha = -230.0/12.0;
    game.journee.max_alpha = 230;
    game.journee.alpha = game.journee.max_alpha + ((game.journee.heure-3) * game.journee.speed_alpha) ;

    // Couleur de la police
    game.couleur.noire.r = 0;
    game.couleur.noire.g = 0;
    game.couleur.noire.b = 0;

    // fps
    game.fps.last_tick_time = 0;
    game.fps.delta = 0;
    game.fps.tick_time = SDL_GetTicks();
    game.fps.nb_fps = 0;
    game.fps.compteur = 0;

    //camera
    game.ecran.camera.x = 400;
    game.ecran.camera.y = 100;

    // Message
    sprintf(game.journee.message,"Jour :%d - Heure : %d - Minutes : %d",game.journee.jour,game.journee.heure,game.journee.minutes);//pas possible de passer a la ligne
    textText = CreateFont(game.ecran.renderer, police, game.journee.message, game.couleur.noire);    
    
    tabArmure[0] = 9;
    

  
    // ================== ANNIMATION ==================

    // remplissage de la liste d'annimation pour le player
    loadImg(PLAYER, IDLE, 0, 1, 32, 48, 8, 2); // player
    loadImg(PLAYER, DOWN, 0, 1, 32, 48, 8, 2);
    loadImg(PLAYER, UP, 32, 0, 32, 48, 8, 2);
    loadImg(PLAYER, LEFT, 64, 0, 32, 48, 7, 2);
    loadImg(PLAYER, RIGHT, 96, 0, 32, 48, 7, 2);
 
   
    double w1 = 832/13, h1 = 1344/21;
    // remplissage de la liste d'annimation pour les ennemis
    loadImg(ENNEMI1, IDLE, 0,  2*h1, w1, h1, 9, 1); // skeleton
    loadImg(ENNEMI1, DOWN, 0, 2*h1, w1, h1, 9, 1);
    loadImg(ENNEMI1, UP, 0, 0, w1, h1, 9, 1);
    loadImg(ENNEMI1, LEFT, 0,  1*h1, w1, h1, 9, 1);
    loadImg(ENNEMI1, RIGHT, 0, 3*h1, w1, h1, 9, 1);
    loadImg(ENNEMI1, TIREG, 0, 14*h1, w1, h1, 9, 1);
    loadImg(ENNEMI1, TIRED, 0, 16*h1, w1, h1, 9, 1);
    loadImg(ENNEMI1, TIREUP, 0, 13*h1, w1, h1, 9, 1);
    loadImg(ENNEMI1, TIREDOWN, 0, 15*h1, w1, h1, 9, 1);
    
    double w2 = 249/3, h2 =324/4;
    loadImg(ANIMAL,IDLE,0,3*h2,w2,h2,4,1);
    loadImg(ANIMAL,DOWN,0,h2,w2,h2,4,1);
    loadImg(ANIMAL,UP,0,2*h2,w2,h2,4,1);
    loadImg(ANIMAL,LEFT,0,0,w2,h2,4,1);
    loadImg(ANIMAL,RIGHT,0,3*h2,w2,h2,4,1);



    // =========================== ENNEMIES ===========================
    int randX = 0, randY = 0;
    int nb_enn = 10; // nombre d'ennems à générer

    bool collide = false;
    // créer les ennemis aléatoirement sur la map
    while(nb_enn > 0){ 
        randX = random(0, largeur_map);
        randY = random(0, hauteur_map);
        SDL_Rect rect_box = {randX, randY, tab_animations[ENNEMI1][IDLE][0].w - 30,  tab_animations[ENNEMI1][IDLE][0].h - 15};

        if(collideRectMap(rect_box) == 0 && collideRectRect(rect_box, hit_box) == -1){//&& collideRectDonjon(tab_donjon, rect_box, nb_donjon) == false){
            listeEnnemie = ajouter_teteEnn(listeEnnemie, creerEnnemie(ENNEMI1, randX-15, randY-15, false));
            nb_enn--;
        }
    }





    //=========================== ANIMAUX ===========================
    int nb_ani = 10;
    while(nb_ani > 0){ 
        randX = random(0, largeur_map);
        randY = random(0, hauteur_map);
        SDL_Rect rect_box = {randX-5, randY, w2, h2};
        if(collideRectMap(rect_box) == 0 && collideRectRect(rect_box, hit_box) == -1  && collideRectEnnemis(listeEnnemie, rect_box) == false){
            listeAnimaux = ajoutAnimaux(listeAnimaux,createAnimal(VACHE,randX,randY));
            setTargetAnimal(listeAnimaux);
            nb_ani--;
        }
    }
    
    // =========================== DONJONS ===========================
    randX = 0;
    randY = 0;
 
    bool cree = false;
    int rnd = 0;
    int nb_cree = 0; // nb de donjons crée
    
    // créer les donjons aléatoirement sur la map
    for(int i = 0; i<nb_donjon; i++){
        randX = random(0, largeur_map);
        randY = random(0, hauteur_map);
        rnd = random(3, 5);
        tab_donjon[i] = creerdonjon(randX, randY, random(1, 3), rnd);
        cree = false;
        nb_cree++;

        while(cree == false){ 
            SDL_Rect rect_box = {randX-70, randY-70, tab_donjon[i]->box.w+70, tab_donjon[i]->box.h+140};
            if(collideRectMap(rect_box) == 0 && collideRectRect(rect_box, hit_box) == -1 && collideRectEnnemis(listeEnnemie, rect_box) == false && collideRectDonjon(tab_donjon, rect_box, nb_cree) == false && collideRectAnimaux(listeAnimaux, rect_box) == false){
                cree = true;
            }
            else{
                randX = random(0, largeur_map);
                randY = random(0, hauteur_map); 
            }
        }
        tab_donjon[i]->box.x = randX;
        tab_donjon[i]->box.y = randY;
    }   

    // texture
    porteG = LoadTexture2(game.ecran.renderer, "src/gfx/porte_gauche.png");
    porteD = LoadTexture2(game.ecran.renderer, "src/gfx/porte_droite.png");
    porteH = LoadTexture2(game.ecran.renderer, "src/gfx/porte_haut.png");
    porteB = LoadTexture2(game.ecran.renderer, "src/gfx/porte_bas.png");
    textureManger = LoadTexture2(game.ecran.renderer,"src/gfx/barreManger.png");
    textureSoif = LoadTexture2(game.ecran.renderer,"src/gfx/barreSoif.png");;
    textureFond = LoadTexture2(game.ecran.renderer,"src/gfx/barreFond.png");;
    textureXp = LoadTexture2(game.ecran.renderer,"src/gfx/barreXp.png");
  
    textureAnimaux = LoadTexture2(game.ecran.renderer,"src/gfx/cowPatreon.png");
    temperature = 0;
    
    textureBarreFond = LoadTexture2(game.ecran.renderer,"src/gfx/health_bar_decoration.png");
    textureBarre = LoadTexture2(game.ecran.renderer,"src/gfx/health_bar.png");
    textEN = LoadTexture2(game.ecran.renderer, "src/gfx/skeleton3.png"); // tilesheet du skeleton
    texAlerte = LoadTexture2(game.ecran.renderer, "src/gfx/alert.png"); // image alerte
  
  // =========================== OBJETS ===========================

    textObj = LoadTexture2(game.ecran.renderer, "src/gfx/objet_sheet.png"); // tilesheet des objets

    liste_objets = NULL;// liste  d'objets (liste chainé simple)

    int nb_obj = 30, randObj = 0;
    randX = 0, randY = 0;
    collide = false;

    // créer les objets aléatoirement sur la map
    while(nb_obj > 0){ 
        randX = random(0, largeur_map);
        randY = random(0, hauteur_map);
        SDL_Rect rect_box = {randX, randY, 32, 32};
        collide = false;
        if(collideRectMap(rect_box) == 0 && collideRectRect(rect_box, hit_box) == -1 && collideRectDonjon(tab_donjon, rect_box, nb_donjon) == false){
            // pour chaque donjon
            for(int i = 0; i<nb_donjon; i++){
                if(collideRectRect(rect_box, tab_donjon[i]->box) == 1){ // collision avec le donjon
                    collide = true;
                }
            }
            if(collide == false){ // si pas collision
                randObj =  random(1, 11);
                liste_objets = ajouter_tete(liste_objets, createObjet(randObj, randX, randY));
                nb_obj--;                
            }
        }
    }

    // crée une pioche et hache
    liste_objets = ajouter_tete(liste_objets, createObjet(1, mon_player.x+game.ecran.camera.x+50, mon_player.y+game.ecran.camera.y));
    liste_objets = ajouter_tete(liste_objets, createObjet(2, mon_player.x+game.ecran.camera.x+90, mon_player.y+game.ecran.camera.y+50));
    liste_objets = ajouter_tete(liste_objets, createObjet(8, mon_player.x+game.ecran.camera.x+50, mon_player.y+game.ecran.camera.y+50));

    // =========================== Tirs ===========================

    Liste_tirs = NULL;


    // =========================== Ressources ===========================
    
    if(lireRecolter() == -1){
        return -1;
    }

    //musique donjon

    donjonMusic = Mix_LoadMUS("src/son/Heroic Demise (New).mp3");
    bowSound = Mix_LoadWAV("src/son/Bow.wav");
    return 1;
}

int libereGame(){// libere l'espace memoire

    SDL_DestroyTexture(textureBarre);
    SDL_DestroyTexture(textureBarreFond);
    SDL_DestroyTexture(textureSoif);
    SDL_DestroyTexture(textureManger);
    SDL_DestroyTexture(textureFond);
    Mix_FreeMusic(donjonMusic);
    Mix_FreeChunk(bowSound);
    bowSound = NULL;
    textureFond = NULL;
    textureSoif = NULL;
    textureManger = NULL;
    textureBarre = NULL;
    textureBarreFond = NULL;

    return 1;
}
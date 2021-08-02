#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "game.h"
#include "donjon.h"
#include "fonctions.h"
#include "player.h"
#include "map.h"
#include "tirs.h"

Donjons creerdonjon(int p_x, int p_y, int p_type, int n){ // allocation mémoire d'un donjon
    Donjons mon_donjon = NULL;
    mon_donjon = (Donjons)malloc(sizeof(Donjon));

    mon_donjon->box.x = p_x;
    mon_donjon->box.y = p_y;

    int i=0, j=0;

    // initialisation du tableau de salle
    for(i=0; i<5; i++){
        for(j=0; j<7; j++){
           mon_donjon->tab_salles[i][j] = creerSalle(i, j);
        }
    }

    mon_donjon->nbSalles = n;
    mon_donjon->nb_ennemis = genererSalles(mon_donjon->tab_salles, mon_donjon->nbSalles, &(mon_donjon->ligneActu),  &(mon_donjon->colonneActu));// generation des n salles
    mon_donjon->departX = mon_donjon->colonneActu;
    mon_donjon->departY = mon_donjon->ligneActu;
    if(p_type == 1){ // type de donjon
        mon_donjon->img_donjon = LoadTexture2(game.ecran.renderer, "src/gfx/donjon1.png");
    } 
    else{
        mon_donjon->img_donjon = LoadTexture2(game.ecran.renderer, "src/gfx/donjon2.png");
    }
        
    SDL_QueryTexture(mon_donjon->img_donjon, NULL, NULL, &mon_donjon->box.w, &mon_donjon->box.h);// recupére la largeur/hauteur de l'image
    mon_donjon->img_fond = LoadTexture2(game.ecran.renderer, "src/gfx/room.png");// fond dans un donjon

    return mon_donjon;
}

Salles creerSalle(int i, int j){ // allocation mémoire d'une salle
    Salles ma_salle = NULL;
    ma_salle = (Salles)malloc(sizeof(Salle));
    
    ma_salle->ligne = i;
    ma_salle->colonne = j;
    ma_salle->numSalle = 0;
    ma_salle->portes = NULL;
    ma_salle->porteHaut = false;
    ma_salle->porteBas = false;
    ma_salle->porteGauche = false;
    ma_salle->porteDroite = false;
    ma_salle->porteSortie = false;
    ma_salle->open = false;
    ma_salle->nbEnnemis = 0;
    ma_salle->liste_ennemis = NULL;
    ma_salle->liste_objets = NULL;

    return ma_salle;
}

int genererSalles(Salles tab_salles[5][7], int n, int *ligneActu, int *colonneActu){ // genre les n salles d'un donjon
    int salles_open = 1;
    int nb_rand = 0;
    int nb_enn =  0;
    int nb_ennTotal = 0;
    bool creer = false;

    // choix random de la 1ere salle
    int l = random(0, 5);
    int c = random(0, 7);

    *(ligneActu) = l;
    *(colonneActu) = c;

    // Gestion de la salle 1
    tab_salles[l][c]->open = true; // on ouvre la 1ere salle
    tab_salles[l][c]->numSalle = salles_open;
    tab_salles[l][c]->porteSortie = true;

    while(salles_open < n){ // tant que les n salles ont pas été ouvertes
        nb_rand = random(1, 4); // emplacement aléatoire de la salle : 1 -> gauche, 2 -> haut, 3 -> droite, 4-> bas
        creer = false;
        if(nb_rand == 1 && c-1 >= 0){ // gauche
            if(tab_salles[l][c-1]->open == false){ 
                salles_open++;
                creer = true;
                tab_salles[l][c-1]->numSalle = salles_open;    
            }     
            tab_salles[l][c-1]->open = true;
            tab_salles[l][c-1]->porteDroite = true;
            tab_salles[l][c]->porteGauche = true;
            
            c -= 1;
        }
        else if(nb_rand == 2 && l-1 >= 0){ // haut
            if(tab_salles[l-1][c]->open == false){
                salles_open++;
                tab_salles[l-1][c]->numSalle = salles_open;
                creer = true;
            }
            tab_salles[l-1][c]->open = true;
            tab_salles[l-1][c]->porteBas = true;
            tab_salles[l][c]->porteHaut = true;            
            l -= 1;
        }
        else if(nb_rand == 3 && c+1 < 7){ // droite
            if(tab_salles[l][c+1]->open == false){
                salles_open++;
                tab_salles[l][c+1]->numSalle = salles_open;
                creer = true;
            }
            tab_salles[l][c+1]->open = true;
            tab_salles[l][c+1]->porteGauche = true;
            tab_salles[l][c]->porteDroite = true;            
            c += 1;
        }
        else if(nb_rand == 4 && l+1 < 5){ // bas
            if(tab_salles[l+1][c]->open == false){
                salles_open++;
                tab_salles[l+1][c]->numSalle = salles_open;
                creer = true;
            }
            tab_salles[l+1][c]->open = true;
            tab_salles[l+1][c]->porteHaut = true;
            tab_salles[l][c]->porteBas = true;            
            l += 1;
        }

        if(creer){ // si la salle a été crée
            int randX = 0, randY = 0;
            nb_enn = random(1, 4);
            tab_salles[l][c]->nbEnnemis = nb_enn;
            nb_ennTotal += nb_enn;
            // création des ennemis
            while(nb_enn > 0){ 
                randX = random(largeurMur+2, game.ecran.camera.w);
                randY = random(hauteurMur+2, game.ecran.camera.h);
                tab_salles[l][c]->liste_ennemis = ajouter_teteEnn(tab_salles[l][c]->liste_ennemis, creerEnnemie(ENNEMI1, randX, randY, true));
                nb_enn--; 
            }
        }
    }

    return nb_enn;
}

void libereSalle(Salles salle){ // libere l'espace memoire d'une salle
    libereListeEnnemi(salle->liste_ennemis);// libere la liste d'ennemi
    libereListeObjet(salle->liste_objets); // libere la liste d'objet
    free(salle);// libere la salle
}

void libereDonjon(Donjons donjon){ // libere l'espace memoire d'un donjon

    // liberation des salles
    for(int i=0; i<5; i++){
        for(int j=0; j<7; j++){
           libereSalle(donjon->tab_salles[i][j]);
           donjon->tab_salles[i][j] = NULL;
        }
    }
    free(donjon);
}

void libereTabDonjon(){ // libere l'espace memoire de tous les donjons

    for(int i=0; i<nb_donjon; i++){
        libereDonjon(tab_donjon[i]);
        tab_donjon[i] = NULL;
    }
}

void sortiDonjon(){ // le player sort du donjon
    Donjons mon_donjon = tab_donjon[mon_player.id_donjon];

    // repositionnement de la camera
    game.ecran.camera.x = mon_donjon->box.x + mon_donjon->box.w/2 - game.ecran.camera.w/2;
    game.ecran.camera.y = mon_donjon->box.y + mon_donjon->box.h + 10 - game.ecran.camera.h/2;
    
    if(game.ecran.camera.x < 0) // replace en x
        game.ecran.camera.x = 0;
    else if(game.ecran.camera.x + game.ecran.camera.w > largeur_map)
        game.ecran.camera.x = largeur_map - game.ecran.camera.w;
    
    if(game.ecran.camera.y < 0) // replace en y
        game.ecran.camera.y = 0;
    else if(game.ecran.camera.y + game.ecran.camera.h > hauteur_map)
        game.ecran.camera.y = hauteur_map - game.ecran.camera.h;

    // replace le player dans la map    
    mon_player.x = mon_donjon->box.x + mon_donjon->box.w/2 - game.ecran.camera.x;
    mon_player.y = mon_donjon->box.y + mon_donjon->box.h + 10 - game.ecran.camera.y;

    // reinitialise le player
    mon_player.vx = 0;
    mon_player.vy = 0;
    mon_player.isInDonjon = false;
    mon_player.frame = 0; 
    mon_player.timer_tir = mon_player.timer_max;

    minimap = false;
    Mix_HaltMusic(); // stop la musique
  
    libereListeTirs(Liste_tirs);  // vide la liste tirs
    Liste_tirs = NULL;
    
}

void moveInDonjon(double moveX, double moveY){ // deplace le player
 
    mon_player.x += moveX;  
    mon_player.y += moveY; 

}

void genererEnnemisDonjon(int l, int c){ // genere les ennemis d'une salle lorsque le player rentre dans celle cu*i
    Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
    Ennemies e = mon_donjon->tab_salles[l][c]->liste_ennemis;  
    bool collide = true;
    int randX = 0, randY = 0;
    SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1}; // box de collision du player

    while (e != NULL){
        collide = true;     
        while(collide == true){
            randX = random(largeurMur+2, game.ecran.camera.w- largeurMur-2-e->hitbox.w);
            randY = random(hauteurMur+2, game.ecran.camera.h- hauteurMur-2-e->hitbox.h);
            SDL_Rect rect_box = {randX, randY, e->hitbox.w,  e->hitbox.h};
            if(collideDonjon(mon_donjon->tab_salles[l][c], rect_box) == -1 && collideRectRect(rect_box, hit_box) == -1){ // si pas collision
                collide = false;    
                e->x = randX-15;
                e->y = randY-15;
                e->vx = 0;
                e->vy = 0;
                e->frame = 0;
                e->animation = IDLE;
                e->etat = WALK;
                chargerBox(e);
                setTarget(e);
            }
        }
        e = e->suiv;
    }
    // vide la liste tirs
    libereListeTirs(Liste_tirs);
    Liste_tirs = NULL;
}

int loadDonjon(Donjons mon_donjon, int p_id){ // appel lors de l'entrée dans un donjon

    // ========== PLAYER ===============
    mon_player.xDonjon = mon_player.x; // sauvegarde les positions du player
    mon_player.yDonjon = mon_player.y;
    mon_player.x = game.ecran.camera.w/4; // replace le player dans le donjon
    mon_player.y = game.ecran.camera.h/2;
    mon_player.vx = 0;
    mon_player.vy = 0;
    mon_player.isInDonjon = true;
    mon_player.id_donjon = p_id;
    mon_player.frame = 0; 
    mon_player.timer_tir = mon_player.timer_max;

    // camera
    game.ecran.camera.x = 0;
    game.ecran.camera.y = 0;

    minimap = false;

    //vide la liste tirs
    libereListeTirs(Liste_tirs); 
    Liste_tirs = NULL;

    return 1;
} 

int updateDonjon(){ // update du jeu lorsque le player est dans un donjon
    Donjons mon_donjon = tab_donjon[mon_player.id_donjon];

    // ==================== ENNEMIS ====================
    Ennemies e = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_ennemis;  
    while (e != NULL){
        double oldX = e->x;
        double oldY = e->y;
        double oldBoxX = e->hitbox.x;
        double oldBoxY = e->hitbox.y;

        updateEnnemis(e,0);

        // =============== Déplacements ==================

        e->x += e->vx * game.fps.delta;
        e->y += e->vy * game.fps.delta;

        chargerBox(e);
        // =================== Gere les tires ===================
        if(e->etat == ARC){
            e->timer_tir += game.fps.delta;
            if(e->timer_tir > e->timer_max){ // s il peut tirer
                e->timer_tir = 0;
                if(e->animation == TIRED)
                    Liste_tirs = ajouter_teteTirs(createTir(D_RIGHT, ENNEMI1, e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 200));// créé un tir
                else if(e->animation == TIREDOWN)
                    Liste_tirs = ajouter_teteTirs(createTir(D_DOWN, ENNEMI1,  e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 200));// créé un tir
                else if(e->animation == TIREUP)
                    Liste_tirs = ajouter_teteTirs(createTir(D_UP, ENNEMI1,  e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 200));// créé un tir
                else if(e->animation == TIREG)
                    Liste_tirs = ajouter_teteTirs(createTir(D_LEFT, ENNEMI1,  e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 200));// créé un tir
            }
        }
       
        int distance = sqrt(floor((e->x - oldX)*(e->x - oldX) + (e->y - oldY)*(e->y - oldY))); // distance parcourru

        if(distance > 15){ // si on c'est déplacé plus de 15 pixel
            for(int i = 0; i < distance; i+=15){
                e->x += (e->vx * game.fps.delta)/(distance/15);
                e->y += (e->vy * game.fps.delta)/(distance/15);
                oldBoxX = e->hitbox.x;
                oldBoxY = e->hitbox.y;
                chargerBox(e);
                if(collideEnnemisMap(e, oldX, oldY, oldBoxX, oldBoxY) || collideEnnemisPlayer(e, oldX, oldY, oldBoxX, oldBoxY)){
                    break;
                }
                else{
                    oldX = e->x;
                    oldY = e->y;
                }
            }
        }
        else{
            collideEnnemisMap(e, oldX, oldY, oldBoxX, oldBoxY);
            collideEnnemisPlayer(e, oldX, oldY, oldBoxX, oldBoxY);
        }

        chargerBox(e);

        if(e->etat == WALK){  
            if(e->oldX == e->x  && e->oldY == e->y){ // s il reste a la meme position
              setTarget(e);
            }
            e->oldX = e->x;
            e->oldY = e->y;    
        }

        if(e->etat == ARC)
           e->frame += (e->npFrame*game.fps.delta)/e->timer_max;
        else
            e->frame += 0.2;
       
        if(e->frame > e->npFrame){
            e->frame = 0;
        }
        e = e->suiv;
    }   


    // ================== PORTES ==================
    
    SDL_Rect rectPlayer = {mon_player.x, mon_player.y, mon_player.w-1, mon_player.h-1};

    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteGauche == true){ // gauche
        SDL_Rect rect = {0, game.ecran.camera.h/2-largeurPorte/2, hauteurPorte, largeurPorte};
        if(collideRectRect(rectPlayer, rect) == 1){ // si collision
            mon_donjon->colonneActu--;
            // replace le player
            mon_player.x = game.ecran.camera.w - largeurMur - mon_player.w - 10;
            mon_player.y = game.ecran.camera.h/2 - mon_player.w/2;
            genererEnnemisDonjon(mon_donjon->ligneActu, mon_donjon->colonneActu);
        }
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteHaut == true){ // haut
        SDL_Rect rect = {game.ecran.camera.w/2-largeurPorte/2, hauteurMur-hauteurPorte+1, largeurPorte, hauteurPorte};
        if(collideRectRect(rectPlayer, rect) == 1){ // si collision
            mon_donjon->ligneActu--;
            // replace le player
            mon_player.x = game.ecran.camera.w/2 - mon_player.w/2;
            mon_player.y = game.ecran.camera.h - hauteurMur - mon_player.h - 10;
            genererEnnemisDonjon(mon_donjon->ligneActu, mon_donjon->colonneActu);
        }
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteDroite == true){ // droite
        SDL_Rect rect = {game.ecran.camera.w-hauteurPorte, game.ecran.camera.h/2-largeurPorte/2, hauteurPorte, largeurPorte};
        if(collideRectRect(rectPlayer, rect) == 1){ // si collision
            mon_donjon->colonneActu++;
            // replace le player
            mon_player.x = largeurMur + mon_player.w + 10;
            mon_player.y = game.ecran.camera.h/2 - mon_player.w/2;
            genererEnnemisDonjon(mon_donjon->ligneActu, mon_donjon->colonneActu);
        }
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteBas == true){ // bas
        SDL_Rect rect = {game.ecran.camera.w/2-largeurPorte/2, game.ecran.camera.h-hauteurMur-1, largeurPorte, hauteurPorte};
        if(collideRectRect(rectPlayer, rect) == 1){ // si collision
            mon_donjon->ligneActu++;
            // replace le player
            mon_player.x = game.ecran.camera.w/2 - mon_player.w/2;
            mon_player.y = hauteurMur + mon_player.h + 10;
            genererEnnemisDonjon(mon_donjon->ligneActu, mon_donjon->colonneActu);
        }
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteSortie == true){ // sortie
        SDL_Rect rect = {game.ecran.camera.w/2-largeurPorte/2, game.ecran.camera.h/2-hauteurPorte/2, largeurPorte, hauteurPorte};
        if(collideRectRect(rectPlayer, rect) == 1){ // si collision
            sortiDonjon();
        }
    }
  
    return 1;
}

int drawDonjon(){ // affichage du jeu lorsque le player est dans un donjon

    Donjons mon_donjon = tab_donjon[mon_player.id_donjon];

    // ============================= AFFICHAGE du donjon ==========================

    // fond
    SDL_Rect rectangleFond = {0, 0, game.ecran.camera.w, game.ecran.camera.h};       
    SDL_RenderCopy(game.ecran.renderer, tab_donjon[mon_player.id_donjon]->img_fond, NULL, &rectangleFond); 

    // portes
    SDL_SetRenderDrawColor(game.ecran.renderer, 255, 0, 0, 255);
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteGauche == true){ // gauche
        SDL_Rect rect = {0, game.ecran.camera.h/2-largeurPorte/2, hauteurPorte, largeurPorte};
        SDL_RenderCopy(game.ecran.renderer, porteG, NULL, &rect); 
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteHaut == true){ // haut
        SDL_Rect rect = {game.ecran.camera.w/2-largeurPorte/2, hauteurMur-hauteurPorte, largeurPorte, hauteurPorte};
        SDL_RenderCopy(game.ecran.renderer, porteH, NULL, &rect); 
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteDroite == true){ // droite
        SDL_Rect rect = {game.ecran.camera.w-hauteurPorte, game.ecran.camera.h/2-largeurPorte/2, hauteurPorte, largeurPorte};
        SDL_RenderCopy(game.ecran.renderer, porteD, NULL, &rect); 
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteBas == true){ // bas
        SDL_Rect rect = {game.ecran.camera.w/2-largeurPorte/2, game.ecran.camera.h-hauteurMur, largeurPorte, hauteurPorte};
        SDL_RenderCopy(game.ecran.renderer, porteB, NULL, &rect); 
    }
    if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->porteSortie == true){ // sortie
        SDL_Rect rect = {game.ecran.camera.w/2-largeurPorte/2, game.ecran.camera.h/2-hauteurPorte/2, largeurPorte, hauteurPorte};
        SDL_RenderCopy(game.ecran.renderer, porteH, NULL, &rect); 
    }
    SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255);

    // ========================  AFFICHAGE DE LA MINI MAP ================================
    int i = 0, j = 0;
    int debutX = 10, debutY = 5;
    int largeur_rect = 40, hauteur_rect = 19;
    int espacement = 4;

    if(minimap == true){ // si mini map est active
        for(i=0; i<5; i++){
            for(j=0; j<7; j++){
                if(mon_donjon->tab_salles[i][j]->open == true){// si la salle est ouverte
                    if(mon_donjon->ligneActu == i && mon_donjon->colonneActu == j) // si le player est dans la salle 
                        SDL_SetRenderDrawColor(game.ecran.renderer, 0, 255, 0, 255); // vert
                    else
                        SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255); // blanc
                } 
                else{
                    SDL_SetRenderDrawColor(game.ecran.renderer, 111, 111, 111, 255); // bleu
                }
                SDL_Rect rectangleSalle = {debutX, debutY, largeur_rect, hauteur_rect}; // position de la salle
                SDL_RenderFillRect(game.ecran.renderer, &rectangleSalle); 
                // affichage des portes (ligne rouge)
                SDL_SetRenderDrawColor(game.ecran.renderer, 255, 0, 0, 255); // rouge
                if(mon_donjon->tab_salles[i][j]->porteGauche == true){ // porte gauche
                    SDL_RenderDrawLine(game.ecran.renderer, 
                                            debutX - 8, 
                                            (int)(debutY + hauteur_rect/2),
                                            debutX + 5,
                                            (int)(debutY + hauteur_rect/2));
                }
                if(mon_donjon->tab_salles[i][j]->porteHaut == true){ // porte haut
                    SDL_RenderDrawLine(game.ecran.renderer, 
                                            debutX + largeur_rect/2, 
                                            (int)(debutY - 8),
                                            debutX + largeur_rect/2, 
                                            (int)(debutY + 5));
                }
                debutX += largeur_rect + espacement;
            }
            debutY += hauteur_rect + espacement;
            debutX = 10;
        }
    }
    SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255); // on remet la couleur blanche

    // ========================== AFFICHAGE DES ENNEMIS ===================

    Ennemies e = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_ennemis;
    while(e){
        SDL_Rect rectangleDest = {e->x, e->y, tab_animations[e->type][e->animation][(int)e->frame].w, tab_animations[e->type][e->animation][(int)e->frame].h}; //  a changéééééééé
        // si l objet est visible a l ecran
        if(rectangleDest.x + rectangleDest.w > 0 && rectangleDest.x < game.ecran.camera.w && rectangleDest.y + rectangleDest.h > 0 && rectangleDest.y < game.ecran.camera.h){
           SDL_RenderCopy(game.ecran.renderer, textEN,  &tab_animations[e->type][e->animation][(int)e->frame], &rectangleDest);

            SDL_SetRenderDrawColor(game.ecran.renderer, 255, 0, 0, 255);
            SDL_Rect rectDest = {e->hitbox.x - game.ecran.camera.x, e->hitbox.y - game.ecran.camera.y, e->hitbox.w, e->hitbox.h};
            //target
            SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255);
            if(e->etat == TARGETPLAYER || e->etat == ARC){
                SDL_Rect rectDest = {e->hitbox.x - game.ecran.camera.x+5, e->hitbox.y - game.ecran.camera.y-40, 20, 30};
                SDL_RenderCopy(game.ecran.renderer, texAlerte,  NULL, &rectDest);
            }      
        } 
        e = e->suiv;
    }
   
    return 1;
}
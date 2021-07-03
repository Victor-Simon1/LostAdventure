#include "game.h"
#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include "fonctions.h"
#include "game.h"
#include "donjon.h"
#include "ennemis.h"
#include <math.h>
#include "animaux.h"

bool collideEnnemisPlayer(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY){ // test de collision entre un ennemi et le player
    bool collide = false; // collision avec le player
    SDL_Rect boxEnn = {e->hitbox.x, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};// box de l'ennemi 
    SDL_Rect boxPlayer;// box du player 
    
    if(mon_player.isInDonjon == true && e->isInDonjon == false){ // si le player est dans le donjon
        boxPlayer.x = mon_player.xDonjon;
        boxPlayer.y = mon_player.yDonjon;
        boxPlayer.w = mon_player.w-1;
        boxPlayer.h = mon_player.h-1;
    }
    else{
        boxPlayer.x = mon_player.x+game.ecran.camera.x;
        boxPlayer.y = mon_player.y+game.ecran.camera.y;
        boxPlayer.w = mon_player.w-1;
        boxPlayer.h = mon_player.h-1;
    }

    if(collideRectRect(boxEnn, boxPlayer) == 1){ // collision avec le player
        SDL_Rect hitX = {e->hitbox.x, oldBoxY, e->hitbox.w-1, e->hitbox.h-1};
        if(collideRectRect(hitX, boxPlayer) == 1){ // si collision en x
            e->x = oldX;
            collide = true;
        }
        SDL_Rect hitY = {oldBoxX, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
        if(collideRectRect(hitY, boxPlayer) == 1){ // si collision en y
            e->y = oldY;
            collide = true;
        }
        if(collide == false){
            e->x = oldX;
            e->y = oldY;
            collide = true;
        }
    }  
    return collide;
}

bool collideEnnemisAnimaux(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY){ // test de collision entre un ennemi et tous les animaux
    bool collide = false; // collision avec le player
    SDL_Rect boxEnn = {e->hitbox.x, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
    
    Animaux a = listeAnimaux;

    while (a != NULL){
        if(collideRectRect(boxEnn, a->hitbox) == 1){ // collision avec le player
            SDL_Rect hitX = {e->hitbox.x, oldBoxY, e->hitbox.w-1, e->hitbox.h-1};
            if(collideRectRect(hitX, a->hitbox) == 1){ // si collision en x
                e->x = oldX;
                collide = true;
            }
            SDL_Rect hitY = {oldBoxX, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
            if(collideRectRect(hitY, a->hitbox) == 1){ // si collision en y
                e->y = oldY;
                collide = true;
            }
            if(collide == false){// collision en X et Y
                e->x = oldX;
                e->y = oldY;
                collide = true;
            }
        }
        a = a->suiv;
    }
    return collide;
}

bool collideEnnemisMap(Ennemies e, double oldX, double oldY, double oldBoxX, double oldBoxY){// collision entre un ennemi et la map
    bool collide = false; // collision avec la map
    bool collide2 = false; // collision avec les donjons
    SDL_Rect boxEnn = {e->hitbox.x, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};

    if(e->isInDonjon == false){ // si hors d'un donjon
        if(collideRectMap(boxEnn)== 1){ // collision avec la map
            SDL_Rect hitX = {e->hitbox.x, oldBoxY, e->hitbox.w-1, e->hitbox.h-1};
            if(collideRectMap(hitX) == 1){ // si collision en x
                e->x = oldX;
                collide = true;
            }
            SDL_Rect hitY = {oldBoxX, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
            if(collideRectMap(hitY) == 1){ // si collision en y
                e->y = oldY;
                collide = true;
            }
            if(collide == false){// collision en X et Y
                e->x = oldX;
                e->y = oldY;
                collide = true;
            }
        }

        // pour chaque donjon
        
        for(int i = 0; i<nb_donjon; i++){
            SDL_Rect rectDonj = {tab_donjon[i]->box.x, tab_donjon[i]->box.y, tab_donjon[i]->box.w, tab_donjon[i]->box.h+ 70};
            if(collideRectRect(boxEnn, rectDonj) == 1){ // collision avec le donjon
                SDL_Rect hitX = {e->hitbox.x, oldBoxY, e->hitbox.w-1, e->hitbox.h-1};
                if(collideRectRect(hitX, rectDonj) == 1){ // si collision en x
                    e->x = oldX;
                    collide2 = true;
                }
                SDL_Rect hitY = {oldBoxX, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
                if(collideRectRect(hitY, rectDonj) == 1){ // si collision en y
                    e->y = oldY;
                    collide2 = true;
                }
                if(collide2 == false){// collision en X et Y
                    e->x = oldX;
                    e->y = oldY;
                    collide2 = true;
                }
            }
        }

    }
    else{ // si dans un donjon
        Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
        if(collideDonjon(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], boxEnn) == 1){ // collision avec le donjon
            SDL_Rect hitX = {e->hitbox.x, oldBoxY, e->hitbox.w-1, e->hitbox.h-1};
            if(collideDonjon(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], hitX) == 1){ // si collision en x
                e->x = oldX;
                collide = true;
            }
            SDL_Rect hitY = {oldBoxX, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
            if(collideDonjon(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], hitY) == 1){ // si collision en y
                e->y = oldY;
                collide = true;
            }
            if(collide == false){// collision en X et Y
                e->x = oldX;
                e->y = oldY;
            }
        }
    }

    return collide || collide2;

}

void setTarget(Ennemies e){ // genere une nouvelle target

    int randomX = random(-e->rangeRandom, e->rangeRandom);
    
    int min = -sqrt((e->rangeRandom*e->rangeRandom)+1 - (randomX*randomX));
    int max = sqrt((e->rangeRandom*e->rangeRandom)+1- (randomX*randomX));
    int randomY = random(min,max);
    e->targetX = e->hitbox.x + e->hitbox.w/2 + randomX;
    e->targetY = e->hitbox.y + e->hitbox.h/2 + randomY;

    if(e->isInDonjon){ // s il est dans un donjon
        SDL_Rect rect = {e->targetX, e->targetY, 2, 2};
        Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
        if(collideDonjon(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], rect) == 1){
            setTarget(e);
        }
    }
    else{
        if(collidePtMap(e->targetX, e->targetY) == 1){
            setTarget(e);
        }
    }

         
}

Ennemies ajouter_teteEnn(Ennemies liste, Ennemies C){  // ajoute un ennemi dans une liste

    C->suiv = liste;
    C->prec = NULL;
    if(liste)
        liste->prec = C;

    return C;
}

void chargerBox(Ennemies e){// met a jour la box de l'ennemi en fonction de ses coordonées

    e->hitbox.x = e->x + 15;
    e->hitbox.y = e->y + 15;
    e->hitbox.w = tab_animations[e->type][e->animation][(int)e->frame].w - 30;
    e->hitbox.h = tab_animations[e->type][e->animation][(int)e->frame].h - 15;
}

Ennemies creerEnnemie(Type p_type, int p_x, int p_y, bool p_donjon){// allocation memoire d'un ennemi
    Ennemies e = NULL;
    e = (Ennemies)malloc(sizeof(Ennemie));
    
    e->viemax = 200;
    e->vie = e->viemax;
    e->type = p_type;
    e->animation = IDLE;
    e->x = p_x;
    e->y = p_y;
    e->oldX = p_x;
    e->oldY = p_y;
    e->vx = 0.0;
    e->vy = 0.0;
    e->vxmax = 60;
    e->vymax = 60;
    e->acceleration = 20;
    e->frottement = 20;
    e->timer_max = 2; // en seconde
    e->timer_tir = 0;
    e->isInDonjon = p_donjon;
    e->tabDrop[0] = 5;
    //e->tabDrop[1] = 2;

    e->rangeDetection = 350;
    e->rangeRandom = 100;
    e->rangeAtk = 200;
    e->rangeLost = 400;
    e->suiv = NULL;
    e->etat = WALK;
    e->frame = 0;
    e->npFrame = 8;

    chargerBox(e); 
    // target
    e->targetX = e->x;
    e->targetY = e->y;
 
    return e;
}

Ennemies supprimEnnemis(Ennemies *list, Ennemies e){ // supprime un ennemi d'une liste donnée
    Ennemies temp = e->suiv;

    if(e->prec)
         (e->prec)->suiv = e->suiv;
    else
        *list = e->suiv;

    if(e->suiv)
        (e->suiv)->prec = e->prec;

    libererEnnemi(e);
    return temp;
}

void libererEnnemi(Ennemies e){// libere l'espace memoire d'un ennemi
    e->suiv = NULL;
    free(e);
    e = NULL;
}

void libereListeEnnemi(Ennemies e){// libere l'espace memoire d'une liste d'ennemis
    Ennemies temp, c = e;

    while(c){
        temp = c->suiv;
        libererEnnemi(c);
        c = temp;
    }
}

void updateEnnemis(Ennemies e, int move){ // mise a jour de l'ennemi (etat, vitesse, annimation)

    // ==================================== vérification de l'etat de l'ennemi ===========================================

    int centreEnn_x = e->hitbox.x+e->hitbox.w/2;
    int centreEnn_y = e->hitbox.y+e->hitbox.h/2;
    int centrePlayer_x = game.ecran.camera.x + mon_player.x+mon_player.w/2;
    int centrePlayer_y = game.ecran.camera.y + mon_player.y+mon_player.h/2;
    
    if(e->isInDonjon == true || (e->isInDonjon == false && mon_player.isInDonjon == false)){
        int distance = distance2Pts(centreEnn_x, centreEnn_y, centrePlayer_x, centrePlayer_y); // distance entre l'ennemie et le player
        if(e->etat == TARGETPLAYER){
            if(distance > e->rangeLost){// si on perd le player de vue
                e->etat = WALK;
                e->frame = 0;
                setTarget(e); // trouve une nouvelle target
            }
            else if(distance < e->rangeAtk){// si on est assez proche pour attaquer
                e->etat = ARC;
                e->frame = 0;
            }
        }
        else if(e->etat == WALK){
            if(distance < e->rangeAtk){// si on est dans la range d'attaque
                e->etat = ARC;
                e->frame = 0;
            }
            else if(distance < e->rangeDetection){// si on est dans la range de detection
                e->etat = TARGETPLAYER;
                e->frame = 0;
            }
            else{
                if(distance2Pts(centreEnn_x, centreEnn_y, e->targetX, e->targetY) < 15){// si on est assez proche
                    setTarget(e); // trouve une nouvelle target
                }   
            }
        }
        else if(e->etat == ARC){
            if(distance < e->rangeDetection && distance > e->rangeAtk){// si on n'est plus dans la range d'attaque
                e->etat = TARGETPLAYER;
                e->timer_tir = 0;
                e->frame = 0;
            }
            else if(distance > e->rangeLost){// si on perd le player de vue
                e->etat = WALK;  
                setTarget(e); // trouve une nouvelle target
                e->timer_tir = 0;
                e->frame = 0;
            }
        }

        if(e->etat == ARC){
            e->targetX = centrePlayer_x;
            e->targetY = centrePlayer_y;
            e->vx = 0;
            e->vy = 0;
        }
        else if(e->etat == TARGETPLAYER){
            e->targetX = centrePlayer_x;
            e->targetY = centrePlayer_y;
        }        
    }
    else{ // pas dans donjon
        if(e->etat != WALK){
            e->etat = WALK;
            setTarget(e); // trouve une nouvelle target
            e->timer_tir = 0;
            e->frame = 0;
        }
        else{
            if(distance2Pts(centreEnn_x, centreEnn_y, e->targetX, e->targetY) < 15){ // si target atteint
                setTarget(e); // trouve une nouvelle target
            }   
        }
        
    }

    // ==================================== Change sa vitesse ===========================================

    
    if((abs(centreEnn_x - e->targetX)  >= 20 && abs(centreEnn_y - e->targetY) >=  20) && e->etat == ARC|| e->etat != ARC ){
        if(abs(centreEnn_x - e->targetX) >= 20){
            if((e->targetX - centreEnn_x) < 0)
                e->vx -= e->acceleration ;    
            else
                e->vx += e->acceleration ;
        }
            
        if(abs(centreEnn_y - e->targetY) >=  20){
            if((e->targetY - centreEnn_y) < 0)
                e->vy -= e->acceleration ; 
            else
                e->vy += e->acceleration ; 
        }   
    }

    // Verification des vitesses
    if(e->vx > e->vxmax)
        e->vx = e->vxmax;
    else if(e->vx < -e->vxmax)
        e->vx = -e->vxmax;
    if(e->vy > e->vymax)
        e->vy = e->vymax;
    else if(e->vy < -e->vymax)
        e->vy = -e->vymax;

    // ====================================== Change les annimations =================================================

    if(e->etat != ARC){
        if(e->vy < 0){ // s'il va vers le haut
            e->animation = UP;
        }
        else if(e->vy > 0){ // s'il va vers le bas
            e->animation = DOWN;
        }

        if(e->vx < 0){ // s'il va vers la gauche
            e->animation = LEFT;
        }
        else if(e->vx > 0){ // s'il va vers la droite
            e->animation = RIGHT;
        }
        
        if(e->vx == 0 && e->vy == 0){ // s il ne bouge pas
            e->animation = IDLE;
        }
    }
    else {
        
        double angle2 = acos((
                        -(double)(distance2Pts(centreEnn_x, centreEnn_y, centrePlayer_x, centrePlayer_y))*(double)(distance2Pts(centreEnn_x, centreEnn_y, centrePlayer_x, centrePlayer_y))
                        +(centreEnn_y - centrePlayer_y)*(centreEnn_y - centrePlayer_y)
                        -(centreEnn_x - centrePlayer_x)*(centreEnn_x - centrePlayer_x))
                        /(-2*(centreEnn_x - centrePlayer_x)*(distance2Pts(centreEnn_x, centreEnn_y, centrePlayer_x, centrePlayer_y))
                        ))*(180/PI);

        if(angle2 >= 0 && angle2 <= 45)
            e->animation = TIREG;
        else if(angle2 >= 135 && angle2 <= 180)
            e->animation = TIRED;
        else if(angle2 >= 45 && angle2 <= 135){
            if((centreEnn_y - centrePlayer_y) < 0)
                e->animation = TIREDOWN;
            else
                e->animation = TIREUP;
        }
    }


}
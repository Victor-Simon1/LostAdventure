#include "animaux.h"
#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include "player.h"
#include <math.h>
#include <stdbool.h>

Animaux createAnimal(Espece esp, double pX, double pY){ // créer un animal
    Animaux a = NULL;
    a = (Animaux)malloc(sizeof(animal));
    printf("ssssssssssssSALT");
    printf("ssssssssssssSALT");
    printf("ssssssssssssSALT");

    //attribut
    a->x = pX;
    a->y = pY;
    a->oldX = a->x;
    a->oldY = a->y;
    a->vie = 180.0;
    a->espece = esp;
    a->vx = 0.0;
    a->vy = 0.0;
    a->vxmax = 60.0;
    a->vymax = 60.0;
    a->acceleration = 20.0;
    a->frottement = 20.0;
    a->timer_fuite = 0;
    //graphisme/move
    a->animation = IDLE;
    a->etat = WALK;
    a->type = ANIMAL;
    a->frame = 0.0;
    a->nbFrame = 3;
    a->rangeRandom =50;
    //Gestion liste
    a->suiv = NULL;
    a->prec = NULL;
    chargerBoxAnimaux(a);
    
    return a;
}

Animaux ajoutAnimaux(Animaux liste, Animaux new){ // ajoute un animal a une liste d'animaux
    new->suiv = liste;
    new->prec = NULL;
    if(liste)
        liste->prec = new;

    return new;
}

Animaux enleveAnimaux(Animaux old){ // enleve et libere un animal de la liste principale
    Animaux temp = old->suiv;

    if(old->prec)
        old->prec->suiv = temp;
    else 
        listeAnimaux = temp;

    if(temp)
        temp->prec = old->prec;
    
    libererAnimaux(old);

    return temp;
}

void setTargetAnimal(Animaux a){ // change la target de l'animal
    int randomX = random(-a->rangeRandom, a->rangeRandom);
    
    int min = -sqrt((a->rangeRandom*a->rangeRandom)+1 - (randomX*randomX));
    int max = sqrt((a->rangeRandom*a->rangeRandom)+1 - (randomX*randomX));
    int randomY = random(min,max);
    a->targetX = a->hitbox.x + a->hitbox.w/2 + randomX;
    a->targetY = a->hitbox.y + a->hitbox.h/2 + randomY;

    if(collidePtMap(a->targetX, a->targetY) == 1){
        setTargetAnimal(a);
    }
}

bool collideAnimauxMap(Animaux a, double oldX, double oldY, double oldBoxX, double oldBoxY){ // test les collisions entre un animal et la map
    bool collide = false;
    bool collide2 = false;
    SDL_Rect boxA = {a->hitbox.x, a->hitbox.y, a->hitbox.w-1,a->hitbox.h-1};

    if(collideRectMap(boxA)== 1){ // collision avec la map
        SDL_Rect hitX = {a->hitbox.x, oldBoxY,a->hitbox.w-1, a->hitbox.h-1};
        if(collideRectMap(hitX) == 1){ // si collision en x
            a->x = oldX;
            collide = true;
        }
        SDL_Rect hitY = {oldBoxX, a->hitbox.y, a->hitbox.w-1,a->hitbox.h-1};
        if(collideRectMap(hitY) == 1){ // si collision en y
            a->y = oldY;
            collide = true;
        }
        if(collide == false){// collision en X et Y
            a->x = oldX;
            a->y = oldY;
            collide = true;
        }
    }

        // pour chaque donjon
        for(int i = 0; i<nb_donjon; i++){
            SDL_Rect rectDonj = {tab_donjon[i]->box.x, tab_donjon[i]->box.y, tab_donjon[i]->box.w, tab_donjon[i]->box.h+ 70};
            if(collideRectRect(boxA, rectDonj) == 1){ // collision avec le donjon
                SDL_Rect hitX = {a->hitbox.x, oldBoxY, a->hitbox.w-1, a->hitbox.h-1};
                if(collideRectRect(hitX, rectDonj) == 1){ // si collision en x
                    a->x = oldX;
                    collide2 = true;
                }
                SDL_Rect hitY = {oldBoxX, a->hitbox.y, a->hitbox.w-1, a->hitbox.h-1};
                if(collideRectRect(hitY, rectDonj) == 1){ // si collision en y
                    a->y = oldY;
                    collide2 = true;
                }
                if(collide == false){// collision en X et Y
                    a->x = oldX;
                    a->y = oldY;
                    collide2 = true;
                }
            }
        }

    return collide || collide2;
}

void chargerBoxAnimaux(Animaux a){ // charge la box de collision de l'animal en fonction de ses coordonées

    a->hitbox.x = a->x + 4;
    a->hitbox.y = a->y + 8 ;
    a->hitbox.w =tab_animations[a->type][a->animation][(int)a->frame].w - 4;
    a->hitbox.h = tab_animations[a->type][a->animation][(int)a->frame].h - 20;
    
}

void libererAnimaux(Animaux old){ // libere l'espace mémoire d'un animal
    old->suiv = NULL;
    old->prec = NULL;
    free(old);
    old = NULL;
}

void libererListeAnimaux(Animaux a){ // libere l'espace mémoire de tous les animaux de la liste
    Animaux temp, c = a;

    while(c != NULL){
        temp = c->suiv;
        libererAnimaux(c);
        c = temp;
    }
}

bool collideAnimauxPlayer(Animaux a, double oldX, double oldY, double oldBoxX, double oldBoxY){ // test les collisions entre un animal et le player
    bool collide = false; // collision avec le player
    SDL_Rect boxAni = {a->hitbox.x, a->hitbox.y, a->hitbox.w-1, a->hitbox.h-1};
    SDL_Rect boxPlayer = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y , mon_player.w-1, mon_player.h-1};// box du player

    if(collideRectRect(boxAni, boxPlayer) == 1){ // collision avec le player
        SDL_Rect hitX = {a->hitbox.x, oldBoxY, a->hitbox.w-1, a->hitbox.h-1};
        if(collideRectRect(hitX, boxPlayer) == 1){ // si collision en x
            a->x = oldX;
            collide = true;
        }
        SDL_Rect hitY = {oldBoxX, a->hitbox.y, a->hitbox.w-1, a->hitbox.h-1};
        if(collideRectRect(hitY, boxPlayer) == 1){ // si collision en y
            a->y = oldY;
            collide = true;
        }
        if(collide == false){ // collision en X et Y
            a->x = oldX;
            a->y = oldY;
            collide = true;
        }
    }
    return collide;
}

bool collideAnimauxEnnemis(Animaux a,double oldX, double oldY,double oldBoxX,double oldBoxY){ // test les collisions entre un animal et tous les ennemis
    bool collide = false;
    SDL_Rect boxA = {a->hitbox.x, a->hitbox.y, a->hitbox.w-1,a->hitbox.h-1};
    Ennemies e = listeEnnemie;

    while(e != NULL){
         SDL_Rect boxEnn = {e->hitbox.x, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
        if(collideRectRect(boxA, boxEnn) == 1){ // si collision avec l'ennemi
            SDL_Rect hitX = {a->hitbox.x, oldBoxY, a->hitbox.w-1, a->hitbox.h-1};
            if(collideRectRect(hitX, boxEnn) == 1){ // si collision en X
                a->x = oldX; 
                collide = true;
            }
             SDL_Rect hitY = {oldBoxX, a->hitbox.y, a->hitbox.w-1, a->hitbox.h-1};
            if(collideRectRect(hitY, boxEnn) == 1){ // si collision en Y
                a->y= oldY;
                collide = true;
            }
            if(collide == false){ // pour gerer les cas ou il y a collision uniquement avec X ET Y actuel (lors d une collision avec le coin d une image)
                a->x = oldX; 
                a->y = oldY;
                collide = true;
            }
        }
        e = e->suiv;
    }

    return collide;
}

void updateAnimaux(Animaux a){ // mise a jour etat/vitesse/animation d'un animal

    double oldX = a->x;
    double oldY = a->y;
    double oldBoxX = a->hitbox.x;
    double oldBoxY = a->hitbox.y;

    double centreAniX = a->hitbox.x + a->hitbox.w/2;
    double centreAniY = a->hitbox.y + a->hitbox.h/2;

    if(a->etat == WALK){
        if(distance2Pts(centreAniX,centreAniY,a->targetX,a->targetY) < 15){ // si proche de la target
            setTargetAnimal(a); // change la target
        }
    }
    else if(a->etat == FUITE){
        if(mon_player.x + mon_player.w/2+game.ecran.camera.x < centreAniX)
            a->targetX = centreAniX +200 ;
        else
            a->targetX = centreAniX -200 ;

        if(mon_player.y + mon_player.h/2 + game.ecran.camera.y < centreAniY) 
            a->targetY = centreAniY +200 ;
        else  
            a->targetY = centreAniY -200 ;

        a->timer_fuite += game.fps.delta;

        if(a->timer_fuite > 15){
            a->timer_fuite = 0.0;
            a->etat = WALK;
        }
    }
    //change la vitesse
    if(abs(centreAniX - a->targetX)>20 && abs(centreAniY -a->targetY)>20){
         if(abs(centreAniX - a->targetX) >= 20){
            if((a->targetX - centreAniX) < 0)
                a->vx -= a->acceleration ;    
            else
                a->vx +=a->acceleration ;
        }
            
        if(abs(centreAniY - a->targetY) >=  20){
            if((a->targetY - centreAniY) < 0)
                a->vy -= a->acceleration ; 
            else
                a->vy += a->acceleration ; 
        }   
    }
    //vitesse
    if(a->vx > a->vxmax)
        a->vx = a->vxmax;
    else if(a->vx < -a->vxmax)
        a->vx = -a->vxmax;
    if(a->vy > a->vymax)
        a->vy = a->vymax;
    else if(a->vy < -a->vymax)
        a->vy = -a->vymax;
    
    //animation
    if(a->vy<0)
     a->animation = UP;
    else if(a->vy>0)
        a->animation = DOWN;
    if(a->vx <0)
        a->animation = LEFT;
    else if(a->vx>0)
        a->animation = RIGHT;
    if(a->vx == 0 && a->vy ==0)
        a->animation = IDLE;


    // déplacement
    a->x += a->vx * game.fps.delta;
    a->y += a->vy * game.fps.delta;
    chargerBoxAnimaux(a);

    int distance = sqrt(floor((a->x - oldX)*(a->x - oldX) + (a->y - oldY)*(a->y - oldY))); // distance parcourru

    if(distance > 15){ // si on c'est déplacé plus de 15 pixel
        for(int i = 0; i < distance; i+=15){
            a->x += (a->vx * game.fps.delta)/(distance/15);
            a->y += (a->vy * game.fps.delta)/(distance/15);
            oldBoxX = a->hitbox.x;
            oldBoxY = a->hitbox.y;
            chargerBoxAnimaux(a);
            if(collideAnimauxMap(a, oldX, oldY, oldBoxX, oldBoxY) || collideAnimauxPlayer(a, oldX, oldY, oldBoxX, oldBoxY) || collideAnimauxEnnemis(a,oldX, oldY, oldBoxX, oldBoxY)){
                break;
            }
            else{
                oldX = a->x;
                oldY = a->y;
            }
        }
    }
    else{
        collideAnimauxMap(a, oldX, oldY, oldBoxX, oldBoxY);
        collideAnimauxPlayer(a, oldX, oldY, oldBoxX, oldBoxY);
        collideAnimauxEnnemis(a,oldX, oldY, oldBoxX, oldBoxY);
    }
    chargerBoxAnimaux(a);

    if(a->etat == WALK){
        if(a->oldX == a->x  && a->oldY == a->y){ // s il reste a la meme position
          setTargetAnimal(a);
        }
        a->oldX = a->x;
        a->oldY = a->y;    
    }
     a->frame += 0.2;
   
    if(a->frame > a->nbFrame){
        a->frame = 0;
    }
}

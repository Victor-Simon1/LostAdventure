#include <SDL.h>
#include <SDL_mixer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "gui.h"
#include "fonctions.h"

int changeAffichage(char affichage[20]){ // change l'etat de l'affichage ecran

    if(strcmp(game.affichage, affichage) != 0){ // si l'affichage change
        strcpy(game.affichage, affichage);
        resetGroupe();
        Mix_HaltMusic();
        if(strcmp("SETTINGS",affichage) == 0){
            groupeSetting->actif = true;
        }
        else if(strcmp("MENU",affichage) == 0){
            groupeMenu->actif = true;
            Mix_PlayMusic(myMus, -1);
        }
        else if(strcmp("GAME",affichage) == 0){
            groupeGame->actif = false;
            game.etatPause = false;
        }
        else if(strcmp("QUIT",affichage) == 0){
            running = false;
        }
    } 
    return 1;
}

int resetGroupe(){ // remet tous les groupes en inactif
    Groupes temp = listeGroupes;

    while(temp){  // parcour des groupes
        temp->actif = false;
        temp = temp->suiv;
    }
    return 1;
}

// ============== GUI ========================

void affichageJauge(int p_x, int p_y, int p_w, int p_h, double p_actu, double p_max, SDL_Texture *p_texFond,  SDL_Texture *p_texRemplissage){ // affiche une jauge
    double percent = 0;
    percent = (p_actu/p_max)*100; // pourcentage de remplissage
    if(percent > 100)
        percent = 100;
    else if(percent < 0)
        percent = 0;

    SDL_Rect rectF = {p_x, p_y, p_w, p_h}; // fond

    SDL_RenderCopy(game.ecran.renderer, p_texFond, NULL, &rectF); 
    SDL_Rect rectR = {p_x, p_y, (double)(p_w)/100.0*(double)(percent), p_h}; // remplissage
 
    SDL_RenderCopy(game.ecran.renderer, p_texRemplissage, NULL, &rectR); 
}

Buttons creerBouton(char *p_img, char *p_imgSurvol, char *p_imgPressed, int p_x, int p_y, int p_w, int p_h, int (*p_fonc)(char[20]), char* p_param){ // alloue dynamique un boutton
    Buttons mon_bouton = NULL;
    mon_bouton = (Buttons)malloc(sizeof(Button));

    mon_bouton->box.x = p_x;
    mon_bouton->box.y = p_y;
    mon_bouton->box.h = p_h;
    mon_bouton->box.w = p_w;

    mon_bouton->isPressed = false;
    mon_bouton->oldPressed= false;
    mon_bouton->isSurvol = false;

    mon_bouton->fct = p_fonc;
    strcpy(mon_bouton->para, p_param);

    mon_bouton->img = LoadTexture2(game.ecran.renderer, p_img); // image par defaut
    mon_bouton->imgSurvol = LoadTexture2(game.ecran.renderer, p_imgSurvol); // image lors du survol
    mon_bouton->imgPressed = LoadTexture2(game.ecran.renderer, p_imgPressed);// image lors du click

    mon_bouton->hover = Mix_LoadWAV("src/son/button.ogg");

    return mon_bouton;
}


Buttons ajouter_teteButton(Buttons liste, Buttons C){ // ajoute un bouton a une liste chainée

    C->suiv = liste;
    C->prec = NULL;
    if(liste)
        liste->prec = C;

    return C;
}

Buttons supprimButton(Buttons *liste, Buttons mon_bouton){ // supprime et libere un bouton d'une liste 
    Buttons temp = mon_bouton->suiv;

    if(mon_bouton->prec)
         (mon_bouton->prec)->suiv = mon_bouton->suiv;
    else
        *liste =  mon_bouton->suiv;

    if(mon_bouton->suiv)
        (mon_bouton->suiv)->prec = mon_bouton->prec;

    freeButton(mon_bouton);
    return temp;
}

void freeButton(Buttons my_button){ // libere un bouton
    my_button->suiv = NULL;
    my_button->prec = NULL;
    Mix_FreeChunk(my_button->hover);
    SDL_DestroyTexture(my_button->img);
    SDL_DestroyTexture(my_button->imgSurvol);
    SDL_DestroyTexture(my_button->imgPressed);
    my_button->img = NULL;
    free(my_button);
    my_button = NULL;
}

void freeListeButtons(Buttons my_button){ // libere tous les boutons d'une liste
    Buttons temp, c = my_button;

    while(c){
        temp = c->suiv;
        freeButton(c);
        c = temp;
    }
}

// GROUPES

Groupes creerGroupe(){ // alloue dynamiquement un groupe
    Groupes mon_groupe = NULL;
    mon_groupe = (Groupes)malloc(sizeof(Groupe));
    mon_groupe->actif = false;
    mon_groupe->listeButtons = NULL;
    mon_groupe->suiv = NULL;
    mon_groupe->prec = NULL;

    return mon_groupe;
}

Groupes ajouter_teteGroupes(Groupes liste, Groupes C){ // ajoute un groupe a une liste

    C->suiv = liste;
    C->prec = NULL;
    if(liste)
        liste->prec = C;

    return C;
}

Groupes supprimGroupe(Groupes *liste, Groupes mon_groupe){ // supprime et libere un groupe d'une liste
    Groupes temp = mon_groupe->suiv;

    if(mon_groupe->prec)
        (mon_groupe->prec)->suiv = mon_groupe->suiv;
    else
        *liste =  mon_groupe->suiv;

    if(mon_groupe->suiv)
        (mon_groupe->suiv)->prec = mon_groupe->prec;

    freeGroupe(mon_groupe);
    return temp;
}

void freeGroupe(Groupes mon_groupe){ // libere l'espace memoire d'un groupe
    mon_groupe->suiv = NULL;
    mon_groupe->prec = NULL;
    freeListeButtons(mon_groupe->listeButtons);
    free(mon_groupe);
    mon_groupe = NULL;
}

void freeListeGroupe(Groupes mon_groupe){ // libere tous les groupes d'une liste
    Groupes temp, c = mon_groupe;

    while(c){
        temp = c->suiv;
        freeGroupe(c);
        c = temp;
    }
}


int updateGui(){// met à jour tous les groupes actif

    // ============================== POUR CHAQUE GROUPE ==============================
    
    Groupes temp = listeGroupes;

    while(temp){  // parcour des groupes
        if(temp->actif){ // s'il est actif
            // -------------- parcour des boutons --------------
            Buttons tempButton = temp->listeButtons;
            while(tempButton){
                if(collidePtRect(event.motion.x, event.motion.y, tempButton->box) == 1){ // si on survole le boutton
                    tempButton->isSurvol = true;
                    if(event.type == SDL_MOUSEBUTTONDOWN && tempButton->oldPressed == false && tempButton->isPressed == false){
                        tempButton->isPressed = true;
                        tempButton->fct(tempButton->para);
                        Mix_PlayChannel(4,tempButton->hover,0);
                    }

                }
                else{
                    tempButton->isSurvol = false;
                    tempButton->isPressed = false;
                }

                if(event.type != SDL_MOUSEBUTTONDOWN){ // si on click pas
                    tempButton->isPressed = false;
                     tempButton->oldPressed = false;
                }
                else 
                    tempButton->oldPressed = true;

                tempButton = tempButton->suiv;
            }


        }


        temp = temp->suiv;
    }



}

int drawGui(){ // affiche tous les groupes actifs

    // ============================== POUR CHAQUE GROUPE ==============================

    Groupes temp = listeGroupes;
   
    while(temp){  // parcour des groupes
  
        if(temp->actif){ // s'il est actif
            
            // -------------- parcour des boutons --------------
            Buttons tempButton = temp->listeButtons;
            while(tempButton){
             
                if(tempButton->isPressed){
                    SDL_RenderCopy(game.ecran.renderer, tempButton->imgPressed, NULL, &(tempButton->box));
                }
                else if(tempButton->isSurvol){
                    SDL_RenderCopy(game.ecran.renderer, tempButton->imgSurvol, NULL, &(tempButton->box));
                }
                else{
                    SDL_RenderCopy(game.ecran.renderer, tempButton->img, NULL, &(tempButton->box));   
                }
                tempButton = tempButton->suiv;
            }
        }
        temp = temp->suiv;
    }


  
}









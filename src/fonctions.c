#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "map.h"
#include "player.h"
#include "fonctions.h"
#include "game.h"
#include "load.h"
#include "donjon.h"
#include "ennemis.h"
#include "tirs.h"
#include "gui.h"
#include "SDL_mixer.h"
#include "menu.h"
#include "craft.h"
#include "animaux.h"
#include "play.h"
#include "ressources.h"
#include "settings.h"



// ====================== INVENTAIRE ====================================

void affichageInventaire(int nb){//  affiche les cases des inventaires puis leur objets (parametre : 1)

    bool fini = false;
    int nbColonnePrinc =sizeof(mon_player.inv.inventairePrin[0]) /sizeof(Case) ;
    int nbLignesPrinc =sizeof(mon_player.inv.inventairePrin)/ (sizeof(Case) *nbColonnePrinc) ;

    int colonnePrinc = (nb-1) % nbColonnePrinc;
    int lignePrinc =(nb-1)/ nbColonnePrinc;
    if(nb<=nbLignesPrinc*nbColonnePrinc){
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].fond,NULL, & mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].rect);
        fini = true;
    }
    int nbColonneMain = sizeof(mon_player.inv.inventairemain) /sizeof(Case) ;
    int nbLignesMain = sizeof(mon_player.inv.inventairemain)/ (sizeof(Case)*nbColonneMain) ;

    int colonneMain = (nb-1) % nbColonneMain;
    if(nb <= nbColonneMain){
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.inventairemain[colonneMain].fond,NULL, &mon_player.inv.inventairemain[colonneMain].rect);
        fini = true;
    }
    int nbColonneCraft = sizeof(mon_player.inv.craft) /(sizeof(Case)) ;
    int nbLignesCraft = sizeof(mon_player.inv.craft)/ (sizeof(Case)*nbColonneCraft) ;
    
    int colonneCraft = (nb-1) %nbColonneCraft;
    if(nb <= nbColonneCraft){
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.craft[colonneCraft].fond,NULL, &mon_player.inv.craft[colonneCraft].rect);
        fini = true;
    }
    int nbColonneArmure =sizeof(mon_player.inv.armure) /(sizeof(Case)) ;
    int nbLignesArmure=sizeof(mon_player.inv.armure)/ (sizeof(Case)*nbColonneArmure) ;
    
    int colonneArmure = (nb-1) % nbColonneArmure;
    if(nb<= nbColonneArmure){
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.armure[colonneArmure].fond,NULL, &mon_player.inv.armure[colonneArmure].rect);
        fini = true;
    }
    
    
    if(fini == true){
        affichageInventaire(nb+1);
    }
    else{
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.caseObjCraft.fond,NULL, &mon_player.inv.caseObjCraft.rect);
        if(mon_player.inv.caseObjCraft.isFilled){
            int colonne = (mon_player.inv.caseObjCraft.obj->id-1)%3;
            int ligne = (mon_player.inv.caseObjCraft.obj->id-1)/3;
            SDL_Rect rect = {colonne*32, ligne*32, 32, 32};
            SDL_RenderCopy(game.ecran.renderer, textObj, &rect, &mon_player.inv.caseObjCraft.obj->rect);
            
            // affiche la quantité de l'objet
            char text[3];   
            snprintf (text, sizeof text, "%d", mon_player.inv.caseObjCraft.obj->nb);
            SDL_DestroyTexture(mon_player.inv.caseObjCraft.obj->texText);
            mon_player.inv.caseObjCraft.obj->texText = NULL;
            mon_player.inv.caseObjCraft.obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
            SDL_Rect rect2 = {mon_player.inv.caseObjCraft.obj->rect.x, mon_player.inv.caseObjCraft.obj->rect.y, 10, 10};
            SDL_RenderCopy(game.ecran.renderer,mon_player.inv.caseObjCraft.obj->texText,NULL,&rect2); 
        }
    }

    // ============= AFFICHAGE DES OBJETS ===============================

    if(nb<=nbLignesPrinc * nbColonnePrinc){
        if(mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].isFilled){
            int colonne = (mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->id-1) % 3;
            int ligne = (mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->id-1)/ 3;
            SDL_Rect rect = {colonne*32, ligne*32, 32, 32};
            SDL_RenderCopy(game.ecran.renderer, textObj, &rect, &mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->rect);

            // affiche la quantité de l'objet
            char text[3];   
            snprintf (text, sizeof text, "%d",  mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->nb);
            SDL_DestroyTexture(mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->texText);
            mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->texText = NULL;
            mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
            SDL_Rect rect2 = {mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->rect.x,  mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->rect.y, 10, 10};
            SDL_RenderCopy(game.ecran.renderer, mon_player.inv.inventairePrin[lignePrinc][colonnePrinc].obj->texText,NULL,&rect2); 
        }
    }
    if(nb<=nbLignesCraft * nbColonneCraft){
        if(mon_player.inv.craft[colonneCraft].isFilled){
            int colonne = (mon_player.inv.craft[colonneCraft].obj->id-1) % 3;
            int ligne = (mon_player.inv.craft[colonneCraft].obj->id-1)/ 3;
            SDL_Rect rect = {colonne*32, ligne*32, 32, 32};
            SDL_RenderCopy(game.ecran.renderer, textObj, &rect, &mon_player.inv.craft[colonneCraft].obj->rect);

            // affiche la quantité de l'objet
            char text[3];   
            snprintf (text, sizeof text, "%d",  mon_player.inv.craft[colonneCraft].obj->nb);
            SDL_DestroyTexture( mon_player.inv.craft[colonneCraft].obj->texText);
            mon_player.inv.craft[colonneCraft].obj->texText = NULL;
            mon_player.inv.craft[colonneCraft].obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
            SDL_Rect rect2 = {mon_player.inv.craft[colonneCraft].obj->rect.x, mon_player.inv.craft[colonneCraft].obj->rect.y, 10, 10};
            SDL_RenderCopy(game.ecran.renderer, mon_player.inv.craft[colonneCraft].obj->texText,NULL,&rect2); 
        }
    }
    if(nb <= nbLignesMain * nbColonneMain){
        if(mon_player.inv.inventairemain[colonneMain].isFilled){
                int colonne = (mon_player.inv.inventairemain[colonneMain].obj->id-1) % 3;
                int ligne = (mon_player.inv.inventairemain[colonneMain].obj->id-1)/ 3;
                SDL_Rect rect = {colonne*32, ligne*32, 32, 32};
                SDL_RenderCopy(game.ecran.renderer, textObj, &rect, &mon_player.inv.inventairemain[colonneMain].obj->rect);
                
                // affiche la quantité de l'objet
                char text[3];   
                snprintf (text, sizeof text, "%d",  mon_player.inv.inventairemain[colonneMain].obj->nb);
                SDL_DestroyTexture(mon_player.inv.inventairemain[colonneMain].obj->texText);
                mon_player.inv.inventairemain[colonneMain].obj->texText = NULL;
                mon_player.inv.inventairemain[colonneMain].obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
                SDL_Rect rect2 = {mon_player.inv.inventairemain[colonneMain].obj->rect.x, mon_player.inv.inventairemain[colonneMain].obj->rect.y, 10, 10};
                SDL_RenderCopy(game.ecran.renderer,mon_player.inv.inventairemain[colonneMain].obj->texText,NULL,&rect2); 
            }
    }
    if(nb<=nbLignesArmure*nbColonneArmure){
        if(mon_player.inv.armure[colonneArmure].isFilled){
                int colonne = (mon_player.inv.armure[colonneArmure].obj->id-1) % 3;
                int ligne = (mon_player.inv.armure[colonneArmure].obj->id-1)/ 3;
                SDL_Rect rect = {colonne*32, ligne*32, 32, 32};
                SDL_RenderCopy(game.ecran.renderer, textObj, &rect, &mon_player.inv.armure[colonneArmure].obj->rect);
                
                // affiche la quantité de l'objet
                char text[3];   
                snprintf (text, sizeof text, "%d", mon_player.inv.armure[colonneArmure].obj->nb);
                SDL_DestroyTexture(mon_player.inv.armure[colonneArmure].obj->texText);
                mon_player.inv.armure[colonneArmure].obj->texText = NULL;
                mon_player.inv.armure[colonneArmure].obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
                SDL_Rect rect2 = {mon_player.inv.armure[colonneArmure].obj->rect.x, mon_player.inv.armure[colonneArmure].obj->rect.y, 10, 10};
                SDL_RenderCopy(game.ecran.renderer,mon_player.inv.armure[colonneArmure].obj->texText,NULL,&rect2); 
            }
    }

}

TTF_Font* LoadFont(char *police , int taille){ // charge une police d'ecriture
    TTF_Font *font = TTF_OpenFont(police,taille);
    if(font == NULL)
        fprintf(stderr,"Unable to create font from %s ! SDL Error %s \n",police,SDL_GetError());
    return font;
}

SDL_Texture *CreateFont(SDL_Renderer *renderer,TTF_Font *police, char message[] ,SDL_Color couleur){ // charge une texture pour un texte
    SDL_Texture *texture = NULL;
    SDL_Surface *surface =  TTF_RenderText_Blended(police,message,couleur);
    if(surface == NULL){
        fprintf(stderr,"Unable to create render %s SDL_font error %s\n",police,SDL_GetError());
    }
    else{
        texture = SDL_CreateTextureFromSurface(renderer,surface);
        if(texture == NULL){

            printf("Unable to create texture from %s! SDL Error %s \n",police,SDL_GetError());
            return NULL;
        }
        else{
            SDL_FreeSurface(surface);
            surface = NULL;
        }
    }
    return texture;
}

// ====================== COLLISION ====================================

int isSolide(int id){ // renvoie 1 si la tile est solide, 0 sinon
    if(id  > nb_tiles){
        return 1;
    }
    return 0;
}

bool collideRectDonjon(Donjons tab[], SDL_Rect rect, int nb){ // retourne true si collision avec un donjon, false sinon
    bool collide = false;

    for(int i = 0; i<nb; i++){ // pour chaque donjon
        if(collideRectRect(tab[i]->box, rect) == 1){ // si collision
            collide = true;
        }
    }

    return collide;
}

int collideRectMap(SDL_Rect rect){ // collision entre un rectangle et les tiles solides de la map / 1 si collision, 0 sinon
    int collide = 0;
    // collision avec les 4 coins
    if(collidePtMap(rect.x, rect.y) == 1 || collidePtMap(rect.x+rect.w, rect.y) == 1 || collidePtMap(rect.x, rect.y+rect.h) == 1 || collidePtMap(rect.x+rect.w, rect.y+rect.h) == 1)
        collide = 1; // collision                                                                        
                                               
    int nbPtsLargeur = (rect.w/(largeur_tile-1))+1;
    int nbPtsHauteur = (rect.h/(hauteur_tile-1))+1;
    int espaceX =0, espaceY=0;
    // collision quadriller du rectangle
    for(int i = 0; i <= nbPtsHauteur; i++){
        espaceY = i*(hauteur_tile-1);
        
        if(espaceY > rect.h){
            espaceY = rect.h;
        }
        for(int j = 0; j <= nbPtsLargeur; j++){
            espaceX = j*(largeur_tile-1);
            if(espaceX > rect.w){
                espaceX = rect.w;
            }
            if(collidePtMap(rect.x+espaceX, rect.y+espaceY) == 1){
                collide = 1;
            }    
        }
    }

/*
    for(int i = 0; i <= rect.h; i += rect.h/nbPtsHauteur){
        //SDL_Log("i : %d", i);
        for(int j = 0; j <= rect.w; j += rect.w/nbPtsLargeur){
            if(collidePtMap(rect.x+j, rect.y+i) == 1){
                collide = 1;
            }    
        }
    }
*/

    return collide; 
}



int collidePtMap(int x, int y){ // Pour une coordonée donnée, retourne 1 si la tile est solide ou hors map, retourne 0 sinon
    
    int ligne = 0, colonne = 0;
    ligne = floor(y/hauteur_tile)+1; // numero de la ligne
    colonne = floor(x/largeur_tile)+1;// numero de la colonne

    if(ligne <= nb_line && ligne > 0 && colonne <= nb_column && colonne > 0)
        return isSolide(map_test[ligne-1][colonne-1]);
    else{
        return 1; // hors de la map
    }
        
}

int collidePtRect(int x, int y,SDL_Rect rect){ // retroune 1 si collision entre le point et le rectangle, -1 sinon
    if(x < rect.x + rect.w && x >= rect.x && y < rect.y + rect.h && y >= rect.y )
        return 1;
    return -1; 
}

int collideRectRect(SDL_Rect rect, SDL_Rect rect2){ // retourne 1 si collision entre 2 rectangles, -1 sinon
    if(rect.x < rect2.x + rect2.w && rect.x + rect.w > rect2.x && rect.y < rect2.y + rect2.h && rect.y + rect.h > rect2.y )
        return 1;
    return -1; // hors de la map
}

bool collideRectAnimaux(Animaux liste,SDL_Rect rect_box){// test de collision entre une rectangle et tous les animaux

    bool collide = false;
    Animaux temp = liste;

    while(temp && collide == false){
        
        if(collideRectRect(temp->hitbox,rect_box) == 1){
            collide = true;
        }
        temp = temp->suiv;
    }

    return collide;
}

bool collideRectEnnemis(Ennemies liste, SDL_Rect rect_box){// test de collision en un rectangle et une liste d'ennemi
    int collide = false;
    Ennemies temp = liste;

    while(temp && collide == false){
        if(collideRectRect(temp->hitbox, rect_box) == 1){
            collide = true;
        }
        temp = temp->suiv;
    }

    return collide;
}

int collideDonjon(Salles ma_salle, SDL_Rect rect){ // retourne 1 si collision entre un rectangle et les murs (dans un donjon), -1 sinon
    

    if(rect.x < largeurMur || rect.x+rect.w > game.ecran.camera.w-largeurMur || rect.y < hauteurMur || rect.y+rect.h > game.ecran.camera.h-hauteurMur){// si collision avec les murs
        return 1;
    }
    /*
    if(ma_salle->porteSortie == true){// s'il y a une prote de sortie
         SDL_Rect rectPorte = {game.ecran.camera.w/2-largeurPorte/2, game.ecran.camera.h/2-hauteurPorte/2, largeurPorte, hauteurPorte};
        if(collideRectRect(rect, rectPorte) == 1){ // si collision
            return 1;
        }
    }
    */
    return -1;
}

int distance2Pts(int p1_x, int p1_y, int p2_x, int p2_y){ // retourne la distance entre 2 points
    return sqrt(floor(pow(p1_x - p2_x, 2) + pow(p1_y - p2_y, 2)));
}

int appartient(int id,int tabSolide[], int taille){// verifie si un nombre appartient à une liste
    int i = 0;
    int trouve = 0 ;
    while( i < taille && trouve == 0 ){
        if(tabSolide[i] == id){
            trouve=1;
        }
        i++;
    }
    return trouve;
}

void verifSolide(int *tilesSolide, int taille){ // identifie tous les tiles solides de la map / arg : listes des id des tiles solide
    int i,j;
    int x = sizeof(map_test)/(sizeof(int)*(sizeof(map_test[0])/sizeof(int)));
    int y = sizeof(map_test[0])/sizeof(int);

    for ( i = 0 ; i < x ; i++){ // parcour la map
        for ( j = 0 ; j < y ; j++){
           if(appartient(map_test[i][j], tilesSolide, taille)){ // si l'id appartient a la liste
               map_test[i][j] += nb_tiles; // incremente l'id par le  nombre de tile (cela permet de l identifié plus tard sans avoir a reparcourir la liste des tiles solides)
           }
        }
    }    
}

SDL_Texture *LoadTexture2(SDL_Renderer *renderer, char *path) // crée et revoie la texture / arg(game.renderer et le nom de l img)
{
    SDL_Texture *texture = NULL; // creation de la texture
    texture = IMG_LoadTexture(renderer, path); // load texture
    if (texture == NULL)
    {
        printf("Unable to create texture from %s Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    return texture; // retourne la texture
}

void move(double moveX, double moveY){ // deplace le player en X et en Y

    // si on se dirige vers la droite
       if (strcmp(mon_player.regardX,"droite") == 0){ 
            if(nb_column * largeur_tile - (game.ecran.camera.x + game.ecran.camera.w) > 0 && mon_player.x > game.ecran.camera.w /2){
                game.ecran.camera.x += moveX ; // on deplace la camera
                if(nb_column * largeur_tile - (game.ecran.camera.x + game.ecran.camera.w) < 0){ // si on a dépassé le bord
                  game.ecran.camera.x = (nb_column * largeur_tile) - game.ecran.camera.w; // on replace la camera
                }
            }
            else{
                mon_player.x += moveX; // on deplace le player
            }     
       }
       // si on se dirige vers la gauche
       else{
            if(game.ecran.camera.x > 0 && mon_player.x < game.ecran.camera.w /2){
               game.ecran.camera.x += moveX; // on déplace la camera
                if(game.ecran.camera.x < 0){ // si on a dépassé le bord
                    game.ecran.camera.x = 0; // on replace la camera
                }
            }
            else{
               mon_player.x += moveX; // on déplace de player
            }           
        }
        // si on se dirige vers le bas
        if(strcmp(mon_player.regardY,"bas") == 0){
            if(nb_line * hauteur_tile -(game.ecran.camera.y + game.ecran.camera.h) > 0 && mon_player.y > game.ecran.camera.h / 2){
                game.ecran.camera.y += moveY; // on deplace la camera
                if(nb_line * hauteur_tile -(game.ecran.camera.y + game.ecran.camera.h) < 0){ // si on a dépassé le bord
                    game.ecran.camera.y = (nb_line * hauteur_tile) - game.ecran.camera.h; // on replace la camera
                }
            }
            else{
                mon_player.y += moveY; // on deplace le player
            }
           }
        // si on se dirige vers le haut
        else{
            if(game.ecran.camera.y > 0 && mon_player.y < game.ecran.camera.h / 2){
                game.ecran.camera.y += moveY;
                if(game.ecran.camera.y < 0){ // si on a dépassé le bord
                    game.ecran.camera.y = 0; // on replace la camera
                }
            }
            else{
               mon_player.y += moveY;
            }
        }



}

int nbLignesinFile(char nomFichier[]){ // renvie le nb de ligne d'un ficher
    FILE *f;
    f = fopen(nomFichier,"r");
    char buffer[256];
    int nbLignes = 0;
    if(f != NULL){
        while(fgets(buffer,200,f) != NULL){
                nbLignes++;
                if(feof(f))break;
        }
        if(fclose(f) != EOF);
        else fprintf(stderr, "Erreur fclose\n");
    }
    else{
        perror(nomFichier);
    }
   
    return nbLignes;
}

int random(int min ,int max){ // genere un nombre aléatoire entre min et max
    return rand()%(max-min)+min;
}


int eventClavier(bool *attrape, Case **ObjAttr, int *ancienInvSelect, float *x, float *y){ // recupere et gere les evenements clavier

    while(SDL_PollEvent(&event)){
       if(strcmp(game.affichage,"GAME") == 0){
            switch(event.type){
                case SDL_KEYDOWN: // apppuie sur une touche
                    switch(event.key.keysym.sym){
                        case SDLK_e: // active ou non l'inventaire
                            if(minimap == false){
                                if(inventaire == true){
                                    inventaire = false;
                                    if(*attrape){// si on a attrapé un objet
                                        *attrape = false;
                                        (*ObjAttr)->obj->rect.x = (*ObjAttr)->rect.x;// on le replace dans sa case
                                       (*ObjAttr)->obj->rect.y = (*ObjAttr)->rect.y;
                                    }
                                }
                                else{
                                    inventaire = true;
                                }
                            }
                        break; 
                        case SDLK_m:
                            if(inventaire == false){
                                minimap = !minimap;
                            }
                        break; 
                        case SDLK_r:
                            if(restaurationPlayer() == true){
                                *attrape = false;
                                ObjAttr = NULL;      
                            }

                        break;
                        case SDLK_w:
                            game.etatPause = !game.etatPause;
                            groupeGame->actif = !groupeGame->actif;
                        break;
                        case SDLK_1:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[0].isSelect = true;
                            *ancienInvSelect = 0;
                        break;
                        case SDLK_2:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[1].isSelect = true;
                            *ancienInvSelect = 1;
                        break;
                        case SDLK_3:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[2].isSelect = true;
                            *ancienInvSelect = 2;
                        break;
                        case SDLK_4:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[3].isSelect = true;
                            *ancienInvSelect = 3;
                        break;
                        case SDLK_5:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[4].isSelect = true;
                            *ancienInvSelect = 4;
                        break;
                        case SDLK_6:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[5].isSelect = true;
                            *ancienInvSelect = 5;
                        break;
                        case SDLK_7:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[6].isSelect = true;
                            *ancienInvSelect = 6;
                        break;
                        case SDLK_8:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[7].isSelect = true;
                            *ancienInvSelect = 7;
                        break;
                        case SDLK_9:
                            mon_player.inv.inventairemain[*ancienInvSelect].isSelect = false;
                            mon_player.inv.inventairemain[8].isSelect = true;
                            *ancienInvSelect = 8;
                        break;
                    }
                    continue;
                case SDL_MOUSEBUTTONDOWN: // clique de la souirs                   
                    if(event.button.button == SDL_BUTTON_LEFT){
                        if(inventaire && *attrape == false){
                            *attrape = attraperObjet(event.motion.x, event.motion.y, &(*ObjAttr));
                            if(*attrape){
                                *x = event.motion.x - (*ObjAttr)->obj->rect.x;
                                *y = event.motion.y -(*ObjAttr)->obj->rect.y;
                            }
                        }
                        else{
                            detecterREssource(event.motion.x,event.motion.y);
                        }
                    }
                break;
                case SDL_MOUSEBUTTONUP: // relachement de la souris
                    if(event.button.button == SDL_BUTTON_LEFT){
                        if(inventaire){
                            if(*attrape){
                                poserInInventory(event.motion.x, event.motion.y, (*ObjAttr));
                                *attrape = false;
                                ObjAttr = NULL;                            
                            }
                        }
                    }      
                break;
                case SDL_KEYUP: // relachement d'une touche
                    switch(event.key.keysym.sym){
                        
                    }
                    continue;
                case SDL_MOUSEMOTION: // si on bouge la souris
                    if(inventaire){
                        if(*attrape){
                            (*ObjAttr)->obj->rect.x = event.motion.x - *x;
                            (*ObjAttr)->obj->rect.y = event.motion.y - *y;
                        }                     
                    }
                break;
                case SDL_QUIT: // si on ferme la fenetre
                    running = false;
                    break;
                default:
                    break;
            }
        }
        else if(strcmp(game.affichage, "MENU") == 0){
            switch(event.type){
                case SDL_QUIT: // si on ferme la fenetre
                    running = false;
                    break;
                default:
                    break;
            }
        }
        else if(strcmp(game.affichage,"SETTINGS") == 0){
            switch (event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
    return 1;
}


/*
    =========================== OBJETS ============================
*/

Obj createObjet(int p_id, int pX, int pY){ // alloue dynamiquement un objet
    Objet *mon_obj = NULL;
    mon_obj = (Obj)malloc(sizeof(Objet));
    
    mon_obj->suiv = NULL;
    mon_obj->prec = NULL;
    
    mon_obj->rect.x = pX;
    mon_obj->rect.y = pY;
    mon_obj->rect.w = largeur_tile;
    mon_obj->rect.h = hauteur_tile;
    mon_obj->nb = 1;
    mon_obj->id = p_id;

    char text[3];   
    snprintf (text, sizeof text, "%d", mon_obj->nb);
    mon_obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);

    mon_obj->type = determinerObjet(p_id);
    if(mon_obj->type == ARMURE)mon_obj->protection = 0.5;
    else mon_obj->protection = -1.0;  
    return mon_obj;
}

TypeObjet determinerObjet(int p_id){ // renvoie le type d'un objet
    if(appartient(p_id,tabArmure,sizeof(tabArmure)/sizeof(int)) == 1){
        return ARMURES;
    }
    else return OBJET;
}

Obj ajouter_tete(Obj liste, Obj C){  // ajoute un objet dans une liste d'obj

    C->suiv = liste;
    C->prec = NULL;
    if(liste)
        liste->prec = C;

    return C;
}

Obj supprim(Obj *liste, Obj obj){// supprime et libere un obj d'une liste d'objet
    Obj temp = obj->suiv;

    if(obj->prec)
         (obj->prec)->suiv = obj->suiv;
    else
        *liste =  obj->suiv;

    if(obj->suiv)
        (obj->suiv)->prec = obj->prec;

    libererObjet(obj);
    return temp;
}

void libererObjet(Obj obj){ // libere l'espace memoire d'un objet
    obj->suiv = NULL;
    obj->prec = NULL;
    SDL_DestroyTexture(obj->texText);
    obj->texText = NULL;
    free(obj);
    obj = NULL;
}

void libereListeObjet(Obj obj){// libere l'espace memoire de tous les objets d'une liste
    Obj temp, c = obj;

    while(c){
        temp = c->suiv;
        libererObjet(c);
        c = temp;
    }
}

void respawnPlayer(){ // fait réaparraitre le player sur le territoire
    int i,j,k,l;
    mon_player.vie = mon_player.vie_max;
    int randomX,randomY;
    //arret de la musique de donjon si dans donjon
    if(mon_player.isInDonjon == true){
        Mix_HaltMusic();
    }
    // lache les obj de l inventaire main
    for(i = 0;i<9;i++){
        if(mon_player.inv.inventairemain[i].isFilled == true){
            randomX = random(-50,50);
            randomY = random(-50,50);
            mon_player.inv.inventairemain[i].obj->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 -mon_player.inv.inventairemain[i].obj->rect.w/2 +randomX;
            mon_player.inv.inventairemain[i].obj->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - mon_player.inv.inventairemain[i].obj->rect.h/2 + randomY;
            liste_objets = ajouter_tete(liste_objets, mon_player.inv.inventairemain[i].obj);// on ajoute l'objet aux objets sur la map
            mon_player.inv.inventairemain[i].obj = NULL;
            mon_player.inv.inventairemain[i].isFilled = false;
        }
    }
    //lache les obj de l inventaire principal
    for(j = 0;j<3;j++){
        for(k = 0;k<9;k++){
            if(mon_player.inv.inventairePrin[j][k].isFilled == true){
                randomX = random(-50,50);
                randomY = random(-50,50);
                mon_player.inv.inventairePrin[j][k].obj->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 -mon_player.inv.inventairePrin[j][k].obj->rect.w/2 +randomX;
                mon_player.inv.inventairePrin[j][k].obj->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - mon_player.inv.inventairePrin[j][k].obj->rect.h/2 + randomY;
                liste_objets = ajouter_tete(liste_objets,mon_player.inv.inventairePrin[j][k].obj);// on ajoute l'objet aux objets sur la map
                mon_player.inv.inventairePrin[j][k].obj = NULL;
                mon_player.inv.inventairePrin[j][k].isFilled = false;
            }
        }
    }
    // lache les obj de l inventaire armure
    for(l = 0;l<4;l++){
        if(mon_player.inv.armure[l].isFilled == true){
            randomX = random(-50,50);
            randomY = random(-50,50);
            mon_player.inv.armure[l].obj->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 - mon_player.inv.armure[l].obj->rect.w/2 +randomX;
            mon_player.inv.armure[l].obj->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - mon_player.inv.armure[l].obj->rect.h/2 + randomY;
            liste_objets = ajouter_tete(liste_objets, mon_player.inv.armure[l].obj);// on ajoute l'objet aux objets sur la map
            mon_player.inv.armure[l].obj = NULL;
            mon_player.inv.armure[l].isFilled = false;
        }
    }
    // lache les obj de l inventaire craft
    for(i = 0;i<3;i++){
        if(mon_player.inv.craft[i].isFilled == true){
            randomX = random(-50,50);
            randomY = random(-50,50);
            mon_player.inv.craft[i].obj->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 - mon_player.inv.craft[i].obj->rect.w/2 + randomX;
            mon_player.inv.craft[i].obj->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - mon_player.inv.craft[i].obj->rect.h/2 + randomY;
            liste_objets = ajouter_tete(liste_objets, mon_player.inv.craft[i].obj);// on ajoute l'objet aux objets sur la map
            mon_player.inv.craft[i].obj = NULL;
            mon_player.inv.craft[i].isFilled = false;
        }
    }
    mon_player.barre_manger = 100;
    mon_player.barre_soif = 100;
    int randX = random(0,largeur_map);
    int randY = random(0,hauteur_map);
    SDL_Rect rect = {randX,randY,mon_player.w,mon_player.h};

    while(collideRectEnnemis(listeEnnemie,rect) == true || collideRectAnimaux(listeAnimaux,rect) == true || collideRectMap(rect) == true || collideRectDonjon(tab_donjon,rect,nb_donjon) == true){
        randX = random(0,largeur_map);
        randY = random(0,hauteur_map);
        rect.x = randX;
        rect.y = randY;
    }
    mon_player.isInDonjon = false;
    mon_player.x = rect.x;
    mon_player.y = rect.y;
    // camera
    game.ecran.camera.x = mon_player.x + mon_player.w /2 - game.ecran.camera.w/2;
    game.ecran.camera.y = mon_player.y + mon_player.h/2 -  game.ecran.camera.h/2;

    if(game.ecran.camera.x < 0) // replace en x
        game.ecran.camera.x = 0;
    else if(game.ecran.camera.x + game.ecran.camera.w > largeur_map)
        game.ecran.camera.x = largeur_map - game.ecran.camera.w;

    if(game.ecran.camera.y < 0) // replace en y
        game.ecran.camera.y = 0;
    else if(game.ecran.camera.y + game.ecran.camera.h > hauteur_map)
        game.ecran.camera.y = hauteur_map - game.ecran.camera.h;
        mon_player.x = rect.x - game.ecran.camera.x;
        mon_player.y = rect.y - game.ecran.camera.y;

    mon_player.timer_tir = mon_player.timer_max;
    minimap = false;
}

/*
  _   _   ____    ____       _      _____   _____  
 | | | | |  _ \  |  _ \     / \    |_   _| | ____| 
 | | | | | |_) | | | | |   / _ \     | |   |  _|   
 | |_| | |  __/  | |_| |  / ___ \    | |   | |___  
  \___/  |_|     |____/  /_/   \_\   |_|   |_____| 
                                                  
*/

int update(){
    if(game.etatPause == false){ // si le jeu n est pas en pause
        if(mon_player.isInDonjon){
            updateDonjon();
        }

        //  ==================== GESTION DE L'HEURE =================

        game.journee.minutesPrec = game.journee.minutes;
        game.journee.minutes += game.journee.speed_time* game.fps.delta;

        if(game.journee.minutes >= 60){
            
            game.journee.heure++;
            game.journee.minutes = 0;
            game.journee.alpha += game.journee.speed_alpha;
            
            if(game.journee.alpha > game.journee.max_alpha){ // verifie que l on ne depasse pas le max
                game.journee.alpha = game.journee.max_alpha;
                game.journee.speed_alpha = - game.journee.speed_alpha;
            }
            if(game.journee.alpha < 1){
                game.journee.alpha = 0;
                game.journee.speed_alpha = - game.journee.speed_alpha;
            }
        }
        else if(game.journee.heure >= 24){
            game.journee.jour++;
            game.journee.heure = 0;
        }

        // =========================== Gestion des events (clavier) ===============================

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) // HAUT
        {
            mon_player.vy -= mon_player.acceleration;
            if(mon_player.animation != UP){
                mon_player.direction = D_UP;
                mon_player.animation = UP;
                mon_player.nb_frame = 8;
                if(strcmp(mon_player.regardY ,"haut") != 0){
                    mon_player.frame = 0;
                    strcpy(mon_player.regardY ,"haut");
                }
            }
        }
        if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) // BAS
        {
            mon_player.vy += mon_player.acceleration;
            if(mon_player.animation != DOWN){
                mon_player.direction = D_DOWN;
                mon_player.animation = DOWN;
                mon_player.nb_frame = 8;
                if(strcmp(mon_player.regardY ,"bas") != 0){
                    mon_player.frame = 0;
                    strcpy(mon_player.regardY ,"bas");
                }
            }
        }
        if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) /// DROITE
        {
            mon_player.vx += mon_player.acceleration;
            if(mon_player.animation != RIGHT){
                mon_player.direction = D_RIGHT;
                mon_player.animation = RIGHT;
                mon_player.nb_frame = 7;        
                if(strcmp(mon_player.regardX ,"droite") != 0){
                    mon_player.frame = 0;
                    strcpy(mon_player.regardX ,"droite");
                }
            }
        }
        if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) // GAUCHE 
        {
            mon_player.vx -= mon_player.acceleration;
            if(mon_player.animation != LEFT){
                mon_player.direction = D_LEFT;
                mon_player.animation = LEFT;
                mon_player.nb_frame = 7;
                if(strcmp(mon_player.regardX ,"gauche") != 0){
                    mon_player.frame = 0;
                    strcpy(mon_player.regardX ,"gauche");
                }
            }
        }
    
        // tir
        if( mon_player.timer_tir <= mon_player.timer_max)
            mon_player.timer_tir += game.fps.delta;
        if (state[SDL_SCANCODE_SPACE]){
            if(mon_player.timer_tir > mon_player.timer_max){ // s il peut tirer
                mon_player.timer_tir = 0;
                Liste_tirs = ajouter_teteTirs(createTir(mon_player.direction, PLAYER, mon_player.x+game.ecran.camera.x+mon_player.w/2, mon_player.y+game.ecran.camera.y+mon_player.h/2, 300));// créé un tir
            }
        }
        
        // ======================= DEPLACEMENT PLAYER ===================

        // sauvegarde les anciennes position
        double oldX = mon_player.x;
        double oldY = mon_player.y;
        double oldCamX = game.ecran.camera.x;
        double oldCamY = game.ecran.camera.y;

        // ralentissement du player 
        
        if(mon_player.vx > 0){                  
            mon_player.vx -= mon_player.frottement; 
            if(mon_player.vx < 0) // pour s'arreter
                mon_player.vx = 0;
        }
        else if(mon_player.vx < 0){
            mon_player.vx += mon_player.frottement;
            if(mon_player.vx > 0) // pour s'arreter
                mon_player.vx = 0;
        }

        if(mon_player.vy > 0){
        mon_player.vy -= mon_player.frottement;
            if(mon_player.vy < 0) // pour s'arreter
                mon_player.vy = 0;
        }      
        else if(mon_player.vy < 0){
        mon_player.vy += mon_player.frottement;
        if(mon_player.vy > 0) // pour s'arreter
            mon_player.vy = 0;
        }

        // verifie que la vitesse ne depasse pas la vitesse max

        if(mon_player.vx < -mon_player.vxmax)
            mon_player.vx = -mon_player.vxmax;
        else if(mon_player.vx > mon_player.vxmax) 
            mon_player.vx = mon_player.vxmax;

        if(mon_player.vy < -mon_player.vymax)
            mon_player.vy = -mon_player.vymax;
        else if(mon_player.vy > mon_player.vymax) 
            mon_player.vy = mon_player.vymax;

        // s il ne bouge pas
        if(mon_player.vx == 0 && mon_player.vy == 0){ 
            mon_player.animation = IDLE; // on change son animation
            mon_player.nb_frame = 8;
        }
        // s il bouge
        else{ 
            if(mon_player.isInDonjon)
                moveInDonjon(mon_player.vx * game.fps.delta, mon_player.vy * game.fps.delta);        
            else
                move(mon_player.vx * game.fps.delta, mon_player.vy * game.fps.delta);
        
            if(Mix_Playing(1) != 1){
                Mix_PlayChannel(1,playerWalk,0); // joue le son de déplacement
            }
        }

        // ================================== Verifie les collisions =================================

        int distancePlay = 0, distanceCam = 0, intervale = 15, diviseur = 0;
        
        distancePlay = sqrt(floor((mon_player.x - oldX)*(mon_player.x - oldX) + (mon_player.y - oldY)*(mon_player.y - oldY)));
        distanceCam = sqrt(floor((game.ecran.camera.x - oldCamX)*(game.ecran.camera.x - oldCamX) + (game.ecran.camera.y  - oldCamY)*(game.ecran.camera.y  - oldCamY)));
        diviseur = (distancePlay + distanceCam)/intervale;

        if(distancePlay + distanceCam > intervale){ // si la distance parcourut est > 15 
            mon_player.x = oldX; // retour en arriere
            mon_player.y = oldY;
            game.ecran.camera.x = oldCamX;
            game.ecran.camera.y = oldCamY;

            for (int i = 0; i < distancePlay + distanceCam; i += intervale){ // on "simule" le deplacement tous les 15(intervale) pixels pour verifier les collisions 
                if(mon_player.isInDonjon) //si le player est dans un donjon
                    moveInDonjon((mon_player.vx * game.fps.delta)/diviseur, (mon_player.vy * game.fps.delta)/diviseur);
                else
                    move((mon_player.vx * game.fps.delta)/diviseur, (mon_player.vy * game.fps.delta)/diviseur);  
                SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};
                if(verifCollidePlayer(oldX, oldY, oldCamX, oldCamY)){
                    break;
                }
                else{
                    oldX = mon_player.x; // on avance les anciennes positions 
                    oldY = mon_player.y;
                    oldCamX = game.ecran.camera.x;
                    oldCamY = game.ecran.camera.y;
                }
            }
        }
        else{
            verifCollidePlayer(oldX, oldY, oldCamX, oldCamY);
            
        }

        // ===================== ANIMATION PLAYER ============================

        mon_player.frame += mon_player.spd_anim * game.fps.delta;
        
        if(mon_player.frame >= mon_player.nb_frame){
            mon_player.frame = 0;
        }

        // ================================================== GESTION SOIF/MANGER ==================================================

        mon_player.barre_soif -= (100*game.fps.delta)/(5*60 / game.journee.speed_time);
        if(mon_player.barre_soif <0){
            mon_player.vie -=(mon_player.vie_max*game.fps.delta)/(5*60 / game.journee.speed_time);
            mon_player.barre_soif =0;
        }
        mon_player.barre_manger -=  (100*game.fps.delta)/(5*60 / game.journee.speed_time);
        if(mon_player.barre_manger <0){
        mon_player.vie -= (mon_player.vie_max*game.fps.delta)/(5*60 / game.journee.speed_time);;
            mon_player.barre_manger =0;
        }

        if(mon_player.barre_xp >= mon_player.xp_max){
            mon_player.barre_xp = mon_player.barre_xp - mon_player.xp_max;
            mon_player.xp_lvl++;
        }

        //================================================== Gestion Biome ==================================================

        detecterBiome();
        gestionTemperature();
     
        if(mon_player.temperature == FROIDGLACIAL){
            mon_player.vie -=(mon_player.vie_max*game.fps.delta)/(5*60 / game.journee.speed_time);
        }
        else if(mon_player.temperature == FROID){
            mon_player.vie -= (mon_player.vie_max*game.fps.delta)/(5*60 / game.journee.speed_time);
        }
        else if(mon_player.temperature == CHAUD){
            mon_player.barre_soif -=(100*game.fps.delta)/(5*60 / game.journee.speed_time);
        }
        else if(mon_player.temperature == DESERTIQUE){
            mon_player.barre_soif -=(100*game.fps.delta)/(5*60 / game.journee.speed_time);
        }
        

        // ================================================== GESTION DES OBJETS ==================================================
        Obj listeObj = NULL;

        if(mon_player.isInDonjon){ // si player dans donjon
            Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
            int nbbb = 0;
            if(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets){
                Obj temp = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets;
                while(temp){ // parcour de la liste des objets
                    nbbb++;
                    SDL_Rect rectangleObj = {temp->rect.x, temp->rect.y, temp->rect.w-1, temp->rect.h-1};// box de l'objet
                    SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y , mon_player.w-1, mon_player.h-1};// box du player
                    if(collideRectRect(rectangleObj, hit_box) == 1){// on regarde si collision entre objet et player
                        if(ajoutInventaire(temp, &mon_player) == 1){ // si on a placé l'objet dans l inventaire
                            Mix_PlayChannel(5,rammaseObjet,0);
                            temp = supprim(&mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets, temp);// on supprime l'objet 
                        }  
                        else
                        temp = temp->suiv;         
                    }
                    else
                        temp = temp->suiv;
                }
            }
        }
        else{
            int nbbb = 0;
            if(liste_objets){
                Obj temp = liste_objets;
                while(temp){ // parcour de la liste des objets
                    nbbb++;
                    SDL_Rect rectangleObj = {temp->rect.x, temp->rect.y, temp->rect.w-1, temp->rect.h-1};// box de l'objet
                    SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y , mon_player.w-1, mon_player.h-1};// box du player
                    if(collideRectRect(rectangleObj, hit_box) == 1){ // on regarde si collision entre objet et player
                        if(ajoutInventaire(temp, &mon_player) == 1){ // si on a placé l'objet dans l inventaire
                            Mix_PlayChannel(5,rammaseObjet,0);
                            temp = supprim(&liste_objets, temp); // on supprime l'objet 
                        }  
                        else
                        temp = temp->suiv;         
                    }
                    else
                        temp = temp->suiv;
                }
            }
        }


        // =============================== CRAFT ========================================
        
        if(inventaire){
            int ancienId = -1; // id de l'objet dans la case craft avant quelle soit changé (-1 si pas d'objet)
            if(mon_player.inv.caseObjCraft.isFilled){
                ancienId = mon_player.inv.caseObjCraft.obj->id; 
            }

            int tab2[3];
            bool caseFilled = false;
            for (int i = 0; i < 3; i++){ //parcour des objets utiles au craft
                if(mon_player.inv.craft[i].isFilled){
                    tab2[i] = mon_player.inv.craft[i].obj->id;
                    caseFilled = true;
                }
                else tab2[i] = -1;
            }

            if(caseFilled == true){ // s'il y a au moins un objet
                ObjCraft obj;
                obj.id1 = tab2[0];
                obj.id2 = tab2[1];
                obj.id3 = tab2[2];

                trieIdObjet(&obj); // trie les id
                
                int pos = rechercheDico(obj,0,nbObjetCraftable);
                if(pos == -1){ // au aucun objet peut etre fabriqué avec ces matériaux
                    mon_player.inv.caseObjCraft.isFilled = false;
                }
                else{
                    // regarde si on a assez de materiaux
                    int nb = 0;
                    bool peutcraft = true;
                    for(int i = 0; i<3; i++){// parcour inventaire craft
                        if(mon_player.inv.craft[i].isFilled == true){
                            nb = recherche(mon_player.inv.craft[i].obj->id, tab[pos]); // recherche de la quantité necessaire
                            if(mon_player.inv.craft[i].obj->nb < nb)
                                peutcraft = false;
                        }
                    }
                    if(peutcraft == true){
                        mon_player.inv.caseObjCraft.isFilled = true;
                        if(ancienId == -1){ // il n y avait pas d obj avant
                            mon_player.inv.caseObjCraft.obj = createObjet(tab[pos].idObjC, mon_player.inv.rectObjCraft.x, mon_player.inv.rectObjCraft.y);
                            replace(&mon_player.inv.caseObjCraft);
                        }
                        else{ 
                            if(ancienId != tab[pos].idObjC){  // on verifie si c est le meme obj  
                                libererObjet(mon_player.inv.caseObjCraft.obj);
                                mon_player.inv.caseObjCraft.obj = createObjet(tab[pos].idObjC, mon_player.inv.rectObjCraft.x, mon_player.inv.rectObjCraft.y);
                                replace(&mon_player.inv.caseObjCraft);
                            }        
                        }
                    }
                }
            }
            else{
                mon_player.inv.caseObjCraft.isFilled = false;
            }        
        }

        // ================================================== ENNEMIES =========================================================

        Ennemies e = listeEnnemie;
        while (e != NULL){
            double oldX = e->x;
            double oldY = e->y;
            double oldBoxX = e->hitbox.x;
            double oldBoxY = e->hitbox.y;

            updateEnnemis(e,0);

        // ================= Déplacements ====================

            e->x += e->vx * game.fps.delta;
            e->y += e->vy * game.fps.delta;

            chargerBox(e);
            // =================== Gere les tires ===================
            if(e->etat == ARC){
                if(e->timer_tir <=  e->timer_max)
                    e->timer_tir += game.fps.delta;
                if(e->timer_tir > e->timer_max){ // s il peut tirer
                    e->timer_tir = 0;
                    Mix_PlayChannel(7, bowSound, 0);
                    if(e->animation == TIRED)
                        Liste_tirs = ajouter_teteTirs(createTir(D_RIGHT, ENNEMI1, e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 300));// créé un tir
                    else if(e->animation == TIREDOWN)
                        Liste_tirs = ajouter_teteTirs(createTir(D_DOWN, ENNEMI1,  e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 300));// créé un tir
                    else if(e->animation == TIREUP)
                        Liste_tirs = ajouter_teteTirs(createTir(D_UP, ENNEMI1,  e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 300));// créé un tir
                    else if(e->animation == TIREG)
                        Liste_tirs = ajouter_teteTirs(createTir(D_LEFT, ENNEMI1,  e->hitbox.x+e->hitbox.w/2, e->hitbox.y+e->hitbox.h/2, 300));// créé un tir
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
                    if(collideEnnemisMap(e, oldX, oldY, oldBoxX, oldBoxY) || collideEnnemisPlayer(e, oldX, oldY, oldBoxX, oldBoxY) || collideEnnemisAnimaux(e, oldX, oldY, oldBoxX, oldBoxY)){
                        break;
                    }
                    else{
                        oldX = e->x; // on avance
                        oldY = e->y;
                    }
                }
            }
            else{
                collideEnnemisMap(e, oldX, oldY, oldBoxX, oldBoxY);
                collideEnnemisPlayer(e, oldX, oldY, oldBoxX, oldBoxY);
                collideEnnemisAnimaux(e, oldX, oldY, oldBoxX, oldBoxY);
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

        // =================================== animaux ===================================
        Animaux a = listeAnimaux;
        int nbA = 1;
        while(a != NULL){
            updateAnimaux(a);
            a = a->suiv;
        }
        // ===================================  TIRS ==========================================================

        Tirs t = Liste_tirs;
        int nb = 0;
        double oldTirX = 0.0, oldTirY = 0.0;
        bool supprime = false;
        
        while (t != NULL){
            nb++;
            supprime = false;
            oldTirX = t->x;
            oldTirY = t->y;
            t->x += t->vx * game.fps.delta;
            t->y += t->vy * game.fps.delta;
            t->distance += sqrt((t->x - oldTirX)*(t->x - oldTirX) + (t->y - oldTirY)*(t->y - oldTirY)); // distance parcourru

            if(t->distance >= t->distance_max)
                supprime = true;

            SDL_Rect box = {t->x, t->y, t->w, t->h};

            if(t->type == ENNEMI1){ // si c'est un tir ennemi
                SDL_Rect boxPlayer = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};
                if(collideRectRect(box, boxPlayer) == 1){
                    supprime = true;
                    mon_player.vie -= 25;
                }
            }
            else{ // si c'est un tir allié
                Ennemies e = NULL;
                if(mon_player.isInDonjon){
                    Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
                    e = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_ennemis;
                }   
                else  
                    e = listeEnnemie;
                while (e != NULL){ // parcour la liste d'ennemis
                    SDL_Rect boxEnn = {e->hitbox.x, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
                    if(collideRectRect(box, boxEnn) == 1){
                        supprime = true;
                        e->vie -= 50;
                        if(e->vie <= 0){
                            int nb_rand = random(0,10);
                            int obj_rand = random(0,1);
                            mon_player.barre_xp += 2;
                            if(e->isInDonjon){
                                Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
                                Obj listObj = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets;
                                if(nb_rand < 5)
                                    listObj = ajouter_tete(listObj, createObjet(e->tabDrop[obj_rand], e->x, e->y));
                                e = supprimEnnemis(&mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_ennemis, e);                           
                            }
                            else{
                                if(nb_rand < 5)
                                    liste_objets = ajouter_tete(liste_objets, createObjet(e->tabDrop[obj_rand], e->x, e->y));
                                e = supprimEnnemis(&listeEnnemie, e); 
                            }

                        }
                        else
                            e = e->suiv;
                        break;
                    }
                    else{
                        e = e->suiv;
                    }   
                }
                Animaux a = listeAnimaux;
                while(a != NULL){
                    SDL_Rect boxAni = {a->hitbox.x, a->hitbox.y, a->hitbox.w-1, a->hitbox.h-1};
                    if(collideRectRect(box,boxAni) == 1){
                        supprime = true;
                        a->vie -= 25;
                        a->etat = FUITE;
                        a->timer_fuite = 0.0;
                        if(a->vie < 0){
                            mon_player.barre_xp +=2;
                            liste_objets = ajouter_tete(liste_objets,createObjet(4,a->x,a->y));
                            a = enleveAnimaux(a);
                        }
                        else a = a->suiv;
                    }
                    else a = a->suiv;
                }
            }

            if(mon_player.isInDonjon){
                Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
                if(collideDonjon( mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], box) == 1){  
                    supprime = true; 
                }    
            }
            else{
                // pour chaque donjon
                if(mon_player.isInDonjon == false ){
                    for(int i = 0; i<nb_donjon; i++){
                        if(collideRectRect(box, tab_donjon[i]->box) == 1){
                            supprime = true; 
                        }
                    }
                }
                if(collideRectMap(box) == 1){  
                    supprime = true; 
                }               
            }
        
            
            if(supprime == true){
                t = supprimTirs(t);
            }       
            else
                t = t->suiv;


        }

        // ================== MINI MAP ===============

        if(mon_player.isInDonjon == false ){
            int ligne;
            int colonne;
            for(int i = -1; i < 3; i++){
                for(int j = -1;j < 2; j++){
                    ligne = (mon_player.y+mon_player.h/2 + largeur_tile*i + game.ecran.camera.y)/largeur_tile;
                    colonne = (mon_player.x+mon_player.w/2 + hauteur_tile*j + game.ecran.camera.x)/hauteur_tile;
                    map_decouverte[ligne][colonne] = 1;
                }

            }
        }

        // ============= DONJON ==============

        SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};// box du player
        if(mon_player.isInDonjon == false ){
            for(int i = 0; i<nb_donjon; i++){
                if(collideRectRect(tab_donjon[i]->box, hit_box) == 1){
                    Mix_PlayMusic(donjonMusic,-1);// lancer la musique du menu
                    game.ecran.camera.x = oldCamX; // on reprend les anciennes positions X
                    mon_player.x = oldX; 
                    game.ecran.camera.y = oldCamY; // on reprend les anciennes positions Y
                    mon_player.y = oldY; 
                    loadDonjon(tab_donjon[i], i);
                }
            }
        }

        // mort du player
        if(mon_player.vie <0){
            respawnPlayer();
        }

    }
    
    // mlise a jour de l'interface graphique
    updateGui();
    
    return 1;
    
}

/*
  ____    ____       _     __        __
 |  _ \  |  _ \     / \    \ \      / /
 | | | | | |_) |   / _ \    \ \ /\ / / 
 | |_| | |  _ <   / ___ \    \ V  V /  
 |____/  |_| \_\ /_/   \_\    \_/\_/   
                                     
*/

int draw(){ // affichage du jeu
   SDL_RenderClear(game.ecran.renderer); // Efface l'écran
   
    if(mon_player.isInDonjon == false){ // si player pas dans un donjon
        // ============================ affichage de la map =========================
        int i = 0, j = 0;
        int x = 0, y = 0;
        int n = 0;
        
        int ligne = floor(game.ecran.camera.y/hauteur_tile); // numero de la ligne
        int colonne = floor(game.ecran.camera.x/largeur_tile);// numero de la colonne
        int borneX = floor((game.ecran.camera.y + game.ecran.camera.h)/hauteur_tile);
        int borneY = floor((game.ecran.camera.x + game.ecran.camera.w)/largeur_tile);
        y = ligne * hauteur_tile;


        for(i = ligne; i <= borneX; i++){ // on commence a l endoit où est la camera pour n'afficher que les tiles visible a l ecran
            x = colonne * largeur_tile ;
            if(i < nb_line){
                for(j = colonne; j <= borneY; j++){
                    if(j < nb_column){
                        n++;
                        int id = map_test[i][j];
                        if(id > nb_tiles){
                            id -= nb_tiles; 
                        }    
                        int ligneImg = (id-1)/9;
                        int colonneImg = (id-1)%9;
                        SDL_Rect rectangle = {colonneImg*largeur_tile, ligneImg*hauteur_tile, largeur_tile, hauteur_tile};
                        SDL_Rect rectangleDest = {x-game.ecran.camera.x, y-game.ecran.camera.y, largeur_tile, hauteur_tile}; // la ou veut l afficher
                        SDL_RenderCopy(game.ecran.renderer, textureBG, &rectangle, &rectangleDest); // ajout dans legame.render
                        x = x + largeur_tile;
                    } 
                }
            }
            y = y + hauteur_tile;
        }  
    }
    
    if(mon_player.isInDonjon){
        drawDonjon();
    }


    // ================================ AFFICHAGE OBJETS ===========================================
    
    Obj listeObj = NULL;

    if(mon_player.isInDonjon){ // si player dans donjon
        Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
        listeObj = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets;
    }
    else{
        listeObj = liste_objets;
    }

    int nb1 = 0;
    if(listeObj){
        Obj temp = listeObj;
        while(temp){
            SDL_Rect rectangleDest = {temp->rect.x - game.ecran.camera.x, temp->rect.y - game.ecran.camera.y, temp->rect.w, temp->rect.h};
            // si l objet est visible a l ecran
            if(rectangleDest.x + rectangleDest.w > 0 && rectangleDest.x < game.ecran.camera.w && rectangleDest.y + rectangleDest.h > 0 && rectangleDest.y < game.ecran.camera.h){
                int colonne = (temp->id-1)%3;
                int ligne = (temp->id-1)/3;
                SDL_Rect rect = {colonne*32, ligne*32, 32, 32};
                SDL_RenderCopy(game.ecran.renderer, textObj, &rect, &rectangleDest);
                
                // affiche la quantité de l'objet
                char text[3];   
                snprintf (text, sizeof text, "%d", temp->nb);
                SDL_DestroyTexture(temp->texText);
                temp->texText = NULL;
                temp->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
                SDL_Rect rect2 = {rectangleDest.x, rectangleDest.y, 10, 10};
                SDL_RenderCopy(game.ecran.renderer,temp->texText,NULL,&rect2); 
            }
            temp = temp->suiv;
        }
    }

    // =================================== affichage le player ==============================

    SDL_Rect rectangleDest = {mon_player.x, mon_player.y, mon_player.w-1, mon_player.h-1}; // position player
    SDL_RenderCopy(game.ecran.renderer, mon_player.current_texture, &tab_animations[mon_player.type][mon_player.animation][(int)mon_player.frame], &rectangleDest); // ajout dans le game.render
    SDL_Rect rect = {mon_player.x, mon_player.y, mon_player.w, mon_player.h};

    // =================================== affichage des ennemis ==============================  
    
    if(mon_player.isInDonjon == false){    
        if(listeEnnemie){
            Ennemies e = listeEnnemie;
            while(e){
                SDL_Rect rectangleDest = {e->x - game.ecran.camera.x, e->y - game.ecran.camera.y, tab_animations[e->type][e->animation][(int)e->frame].w, tab_animations[e->type][e->animation][(int)e->frame].h}; //  a changéééééééé
                // si l objet est visible a l ecran
                if(rectangleDest.x + rectangleDest.w > 0 && rectangleDest.x < game.ecran.camera.w && rectangleDest.y + rectangleDest.h > 0 && rectangleDest.y < game.ecran.camera.h){
                    SDL_RenderCopy(game.ecran.renderer, textEN,  &tab_animations[e->type][e->animation][(int)e->frame], &rectangleDest);
                    
                    //target
                    SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255);
                    if(e->etat == TARGETPLAYER || e->etat == ARC){
                        SDL_Rect rectDest = {e->hitbox.x - game.ecran.camera.x+5, e->hitbox.y - game.ecran.camera.y-40, 20, 30};
                        SDL_RenderCopy(game.ecran.renderer, texAlerte,  NULL, &rectDest);
                    }      
                    SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255); // remet en blanc
                   // affichageJauge(e->hitbox.x- game.ecran.camera.x, e->hitbox.y-7- game.ecran.camera.y, 30, 5, e->vie, e->viemax, textureFond, textureXp); // vie
                } 
                e = e->suiv;
            }
        }

        // =================================== affichage des Animaux ============================== 

        if(listeAnimaux){
            Animaux a = listeAnimaux;
       
            while(a){
               
                SDL_Rect rect = {a->x - game.ecran.camera.x, a->y - game.ecran.camera.y, tab_animations[a->type][a->animation][(int)a->frame].w, tab_animations[a->type][a->animation][(int)a->frame].h};
                
                 if(rect.x + rect.w > 0 && rect.x < game.ecran.camera.w && rect.y + rect.h > 0 && rect.y < game.ecran.camera.h){
         
                    SDL_RenderCopy(game.ecran.renderer,textureAnimaux,&tab_animations[a->type][a->animation][(int)a->frame],&rect);
                    SDL_SetRenderDrawColor(game.ecran.renderer, 255, 0, 0, 255);
                    SDL_Rect rectDest = {a->hitbox.x - game.ecran.camera.x, a->hitbox.y - game.ecran.camera.y, a->hitbox.w, a->hitbox.h};
                     SDL_SetRenderDrawColor(game.ecran.renderer, 255, 255, 255, 255);
                     if(a->etat == FUITE){
                        SDL_Rect rectDest = {a->hitbox.x - game.ecran.camera.x+a->hitbox.w/2 -5, a->hitbox.y - game.ecran.camera.y-40, 20, 30};
                        SDL_RenderCopy(game.ecran.renderer, texAlerte,  NULL, &rectDest);
                     }
                 }
                 a = a->suiv;
                 
            }
            
        }
    }
    else{

    }

    // ================================== AFFICHAGE DES TIRS ===========================================

    Tirs t = Liste_tirs;

    while(t){
        SDL_Rect box = {t->x-game.ecran.camera.x, t->y-game.ecran.camera.y, t->w, t->h};
        SDL_RenderCopy(game.ecran.renderer, t->img,  NULL, &box);
        t = t->suiv;
    }

    // ================================== AFFICHAGE DES DONJONS ===========================================

    if(mon_player.isInDonjon == false){
        for(int i = 0; i<nb_donjon; i++){
            SDL_Rect box = {tab_donjon[i]->box.x-game.ecran.camera.x, tab_donjon[i]->box.y-game.ecran.camera.y, tab_donjon[i]->box.w, tab_donjon[i]->box.h};
            SDL_RenderCopy(game.ecran.renderer, tab_donjon[i]->img_donjon,  NULL, &box);
            // zone sous donjon
            SDL_SetRenderDrawColor(game.ecran.renderer, 211, 211, 211, 160);  // gris  
            SDL_Rect rectangleDest = {tab_donjon[i]->box.x-game.ecran.camera.x, tab_donjon[i]->box.y-game.ecran.camera.y+tab_donjon[i]->box.h, tab_donjon[i]->box.w, 70};
            SDL_RenderFillRect(game.ecran.renderer, &rectangleDest);
        }
    }
    
    // ================================== GESTION DE L'ALPHA (NUIT/JOUR) ==================================

    if(mon_player.isInDonjon == false){
        int ligne = floor(game.ecran.camera.y/hauteur_tile); // numero de la ligne
        int colonne = floor(game.ecran.camera.x/largeur_tile);// numero de la colonne
        int borneX = floor((game.ecran.camera.y + game.ecran.camera.h)/hauteur_tile);
        int borneY = floor((game.ecran.camera.x + game.ecran.camera.w)/largeur_tile);
        int i = 0, j = 0;
        int x = 0, y = ligne * hauteur_tile;
        int alpha = 0;
        int player_ligne = floor((game.ecran.camera.y + mon_player.y + mon_player.h/2)/hauteur_tile);
        int player_col = floor((game.ecran.camera.x + mon_player.x + mon_player.w/2)/largeur_tile);
        int dist = 0; 

        for(i = ligne; i <= borneX; i++){ // on commence a l'endoit où est la camera pour n'afficher que les tiles visible a l ecran
            x = colonne * largeur_tile ;
            for(j = colonne; j <= borneY; j++){
                alpha = (int)(game.journee.alpha) - map_lumiere[i][j]; // alpha - l'eclairage sur cette zone
                dist = floor(sqrt(pow(i-player_ligne,2) + pow(j-player_col,2) )); // distance entre la tile et le player (en nombre de case)
                if(dist < 10){
                    if(dist == 0){
                        alpha = 0;
                    }
                    else{
                        alpha = alpha - 20*(10/dist); // on reduit l alpha autour du player
                    }
                        
                }
                
                if(alpha > game.journee.max_alpha){ // verifie que l on ne depasse pas le max
                    alpha = game.journee.max_alpha;

                }
                else if (alpha < 0){
                    alpha = 0;
                }

                SDL_SetRenderDrawColor(game.ecran.renderer, 0, 0, 0, alpha);  // rectangle  
                SDL_Rect rectangleDest = {x-(int)(game.ecran.camera.x), y-(int)(game.ecran.camera.y), largeur_tile, hauteur_tile}; // la ou veut l afficher

                SDL_RenderFillRect(game.ecran.renderer, &rectangleDest);
                x = x + largeur_tile;
            }
            y = y + hauteur_tile;
        }
 
    }
    
    // ========================================= affichage inventaire ==================================


    if(inventaire){
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.fond, NULL, &mon_player.inv.inventory);
        affichageInventaire(1);
    }
       
    for (int i = 0; i < 9; i++){ // inventaire main
        rect.x = game.ecran.camera.w / 2  - 4.5*50 + i*50;
        rect.y = game.ecran.camera.h - 50;
        rect.w = 50;
        rect.h = 50;
        SDL_RenderCopy(game.ecran.renderer,  mon_player.inv.inventairemain[i].fond, NULL, &rect);           
        if(mon_player.inv.inventairemain[i].isFilled){
            int colonne = (mon_player.inv.inventairemain[i].obj->id-1)%3;
            int ligne = (mon_player.inv.inventairemain[i].obj->id-1)/3;
            SDL_Rect rect1 = {colonne*32, ligne*32, 32, 32};

            SDL_RenderCopy(game.ecran.renderer, textObj, &rect1, &rect);

            //==== Quantité ====
            char text[3];   
            snprintf(text, sizeof text, "%d",  mon_player.inv.inventairemain[i].obj->nb);
            SDL_DestroyTexture(mon_player.inv.inventairemain[i].obj->texText);
            mon_player.inv.inventairemain[i].obj->texText = NULL;
            mon_player.inv.inventairemain[i].obj->texText = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
            SDL_Rect rect2 = {rect.x, rect.y, 10, 10};
            SDL_RenderCopy(game.ecran.renderer, mon_player.inv.inventairemain[i].obj->texText, NULL, &rect2); 
        }
        if(mon_player.inv.inventairemain[i].isSelect){
            SDL_RenderCopy(game.ecran.renderer, selectInv,NULL, &rect);
        }
    }

    if(minimap == true && mon_player.isInDonjon == false){
        for (int ligne = 0; ligne< nb_line; ligne++){
            for (int colonne = 0; colonne < nb_column; colonne++){
                int id = map_test[ligne][colonne];
                if(id > nb_tiles){
                    id -= nb_tiles; 
                }
                int ligneImg = (id-1)/9;
                int colonneImg = (id-1)%9;
                double tailleX = (game.ecran.camera.w -100) / nb_column;
                double tailleY = (game.ecran.camera.h -100) / nb_line;
                double coordX = (game.ecran.camera.x+ mon_player.x )* tailleX / largeur_tile;
                double coordY = (game.ecran.camera.y +mon_player.y) * tailleY  / hauteur_tile;
                SDL_Rect rect = {50 + colonne*tailleX, 50 + ligne*tailleY , tailleX,tailleY }; // position de la tile
                SDL_Rect rectangle = {colonneImg*largeur_tile, ligneImg*hauteur_tile, largeur_tile, hauteur_tile}; // position dans la tilsheet
                SDL_Rect rectPlayer = {50+coordX, 50+coordY, tailleX, tailleY*1.5}; // rectengle du player
                if(map_decouverte[ligne][colonne] == 1){
                    SDL_RenderCopy(game.ecran.renderer , textureBG , &rectangle , &rect ) ;
                }
                else{
                    SDL_SetRenderDrawColor(game.ecran.renderer, 0, 0, 0, 255);
                 SDL_RenderFillRect(game.ecran.renderer,&rect);
                }
                 SDL_SetRenderDrawColor(game.ecran.renderer, 255, 0, 0, 255);
                 SDL_RenderDrawRect(game.ecran.renderer,&rectPlayer);
            } 
        }
    }


    affichageJauge(15, game.ecran.camera.h-24, 200, 10, (double)mon_player.vie, (double)mon_player.vie_max, textureFond, textureBarre); // vie

    SDL_Rect rectCoeur = {0,game.ecran.camera.h-29,20,20};
    SDL_RenderCopy(game.ecran.renderer,textureBarreFond,NULL,&rectCoeur);

    affichageJauge(20, game.ecran.camera.h-40, 100, 10, mon_player.barre_manger, mon_player.max_faim, textureFond, textureManger); // faim
    affichageJauge(121, game.ecran.camera.h-40, 100, 10, mon_player.barre_soif, mon_player.max_soif, textureFond, textureSoif); // soif
    affichageJauge(game.ecran.camera.w - 110, 5, 100, 10, mon_player.barre_xp, mon_player.xp_max, textureFond,textureXp); // xp
    // niveau
    char text[20];   
    snprintf(text, sizeof text, "niveau : %d",  mon_player.xp_lvl);
    SDL_DestroyTexture(mon_player.tex_lvl);
    mon_player.tex_lvl = NULL;
    mon_player.tex_lvl = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
    SDL_Rect rect3 = {game.ecran.camera.w - 205, 5, 80, 10};
    SDL_RenderCopy(game.ecran.renderer, mon_player.tex_lvl, NULL, &rect3);
    
    // =================================== TEXT ===========================

    if((int)game.journee.minutes != (int)game.journee.minutesPrec){
        sprintf(game.journee.message,"Jour :%d - Heure : %d - Minutes : %d",game.journee.jour,game.journee.heure,(int)game.journee.minutes);
        SDL_DestroyTexture(textText);
        textText = NULL;
        textText = CreateFont(game.ecran.renderer, police, game.journee.message, game.couleur.noire);
    }
    SDL_Rect rect2 = {game.ecran.camera.w/2, 0, 150, 20};

    SDL_RenderCopy(game.ecran.renderer,textText,NULL,&rect2); 
    if(game.etatPause == true){
        SDL_Rect rectPause = {0, 0, game.ecran.camera.w, game.ecran.camera.h};
        SDL_RenderCopy(game.ecran.renderer,textPause,NULL,&rectPause);
    }
        drawGui();

    SDL_RenderPresent(game.ecran.renderer); // Présente l'écran
    return 1;
}


 



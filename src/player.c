#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "math.h"

#include "fonctions.h"
#include "donjon.h"
#include "player.h"
#include "game.h"
#include "gui.h"
#include "SDL_mixer.h"
#include "craft.h"
#include "animaux.h"
#include "map.h"


Player creerPlayer(char p_nom[15]){// creer le player
    Player p;

    p.vie = 200.0;
    p.x = game.ecran.camera.w /2;
    p.y = game.ecran.camera.h /2;
    p.vx = 0; // vitesse horizontale 
    p.vy = 0;
    p.respawnX = game.ecran.camera.w/2;
    p.respawnY = game.ecran.camera.h/2;
    p.w = 32; // largeur
    p.h = 48; // hauteur
    p.vxmax = 200; // vitesse max horizontale
    p.vymax = 200;
    p.frottement = 10;
    p.acceleration = 90;
    p.vie = 200;
    p.vie_max = 200;
    p.spd_anim = 7;
    p.id_donjon = 0;
    p.isInDonjon = false;
    p.xDonjon = 0.0;
    p.yDonjon = 0.0;
    p.oldCamX = 0;
    p.oldCamY = 0;
    p.direction = D_DOWN;
    p.current_texture = LoadTexture2(game.ecran.renderer, "src/gfx/tilesheet_player.png");
   // level
    p.barre_xp = 0;
    p.xp_lvl = 0;
    p.xp_max = 10;
    p.tex_lvl = NULL;
    char text[20];   
    snprintf(text, sizeof text, "niveau : %d",  p.xp_lvl);
    p.tex_lvl = CreateFont(game.ecran.renderer, police, text, game.couleur.noire);
   
    p.type = PLAYER;
    p.protection = 0;
    p.temperature = AMBIANT;
    p.animation = IDLE;
    p.frame = 0;
    p.nb_frame = 7;
    p.timer_max = 2.0; // en seconde
    p.timer_tir = 0.0;
    strcpy(p.regardX,"droite");
    strcpy(p.regardY,"haut");
    strcpy(p.nom, p_nom);
    initialiserInventaire(&p);
    p.max_faim = 100;
    p.max_soif = 100;
    p.barre_manger = p.max_faim ;
    p.barre_soif = p.max_soif;
    p.biome = NEIGE;


    return p;
}

void initialiserInventaire(Player *p){ // initialise l'inventaire du player
    (*p).inv.rectPrin.x = game.ecran.camera.w / 2 - 4.5 * 50 - 1.5 *50 - 5;
    (*p).inv.rectPrin.y = game.ecran.camera.h /2 - 1.5*50 - 5 - 0.5*50;
    (*p).inv.rectPrin.w = 9 *50;
    (*p).inv.rectPrin.h = 3 *50;
    int compteur = 0;

    for ( int i = 0; i < 3; i++){      // Principal
        for (int j = 0; j < 9 ; j++){
            (*p).inv.inventairePrin[i][j].rect.x = (*p).inv.rectPrin.x + j*50;
            (*p).inv.inventairePrin[i][j].rect.y = (*p).inv.rectPrin.y + i*50;
            (*p).inv.inventairePrin[i][j].rect.w = 50;
            (*p).inv.inventairePrin[i][j].rect.h = 50;
            (*p).inv.inventairePrin[i][j].id = compteur++;
            (*p).inv.inventairePrin[i][j].obj = NULL;
            (*p).inv.inventairePrin[i][j].isFilled = false;
            (*p).inv.inventairePrin[i][j].isSelect = false;
            (*p).inv.inventairePrin[i][j].fond = LoadTexture2(game.ecran.renderer,"src/gfx/emptySlot.png");
            (*p).inv.inventairePrin[i][j].type = PRINCIPAL;
        }
    }   
    (*p).inv.rectMain.x =  (*p).inv.rectPrin.x;
    (*p).inv.rectMain.y =  (*p).inv.rectPrin.y +  (*p).inv.rectPrin.h + 10;
    (*p).inv.rectMain.h = 50;
    (*p).inv.rectMain.w = 9 * 50;
    for (int j = 0; j < 9 ; j++){ // main
        (*p).inv.inventairemain[j].rect.x = (*p).inv.rectMain.x + j*50;
        (*p).inv.inventairemain[j].rect.y = (*p).inv.rectMain.y;
        (*p).inv.inventairemain[j].rect.w = 50;
        (*p).inv.inventairemain[j].rect.h = 50;
        (*p).inv.inventairemain[j].id = compteur++;
        (*p).inv.inventairemain[j].obj = NULL;
        (*p).inv.inventairemain[j].isFilled = false;
        (*p).inv.inventairemain[j].isSelect = false;
        (*p).inv.inventairemain[j].fond = LoadTexture2(game.ecran.renderer,"src/gfx/emptySlot.png");
        (*p).inv.inventairemain[j].type = MAIN;
    }
    (*p).inv.rectCraft.x =  (*p).inv.rectPrin.x +  (*p).inv.rectPrin.w + 10;
    (*p).inv.rectCraft.y = (*p).inv.rectPrin.y ;
    (*p).inv.rectCraft.w = 3 *50;
    (*p).inv.rectCraft.h = 50;
    for (int j = 0; j < 3 ; j++){ // craft
        (*p).inv.craft[j].rect.x = (*p).inv.rectCraft.x + j*50;
        (*p).inv.craft[j].rect.y = (*p).inv.rectCraft.y;
        (*p).inv.craft[j].rect.w = 50;
        (*p).inv.craft[j].rect.h = 50;
        (*p).inv.craft[j].id = compteur++;
        (*p).inv.craft[j].obj = NULL;
        (*p).inv.craft[j].isFilled = false;
        (*p).inv.craft[j].isSelect = false;
        (*p).inv.craft[j].fond = LoadTexture2(game.ecran.renderer,"src/gfx/emptySlot.png");
        (*p).inv.craft[j].type = CRAFT;
    }

    (*p).inv.rectArmure.x =  (*p).inv.rectCraft.x;
    (*p).inv.rectArmure.y =  (*p).inv.rectMain.y;
    (*p).inv.rectArmure.w = 4 * 50;
    (*p).inv.rectArmure.h = 50;
    for (int i = 0; i < 4; i++){ // armure
        (*p).inv.armure[i].rect.x = (*p).inv.rectArmure.x + i*50;
        (*p).inv.armure[i].rect.y = (*p).inv.rectArmure.y;
        (*p).inv.armure[i].rect.w = 50;
        (*p).inv.armure[i].rect.h = 50;
        (*p).inv.armure[i].id = compteur++;
        (*p).inv.armure[i].obj = NULL;
        (*p).inv.armure[i].isFilled = false;
        (*p).inv.armure[i].isSelect = false;
        (*p).inv.armure[i].fond = LoadTexture2(game.ecran.renderer,"src/gfx/Slot for clothes.png");
        (*p).inv.armure[i].type = ARMURE;
    }
    (*p).inv.inventairemain[0].isSelect = true;

    (*p).inv.caseObjCraft.rect.x = (*p).inv.rectCraft.x + 50;
    (*p).inv.caseObjCraft.rect.y = (*p).inv.rectCraft.y + 50;
    (*p).inv.caseObjCraft.rect.w = 50;
    (*p).inv.caseObjCraft.rect.h = 50;
    (*p).inv.caseObjCraft.id = compteur++;
    (*p).inv.caseObjCraft.obj = NULL;
    (*p).inv.caseObjCraft.isFilled = false;
    (*p).inv.caseObjCraft.isSelect = false;
    (*p).inv.caseObjCraft.fond = LoadTexture2(game.ecran.renderer,"src/gfx/emptySlot.png");
    (*p).inv.caseObjCraft.type = OBJ_A_CRAFT;
    (*p).inv.rectObjCraft.x = (*p).inv.caseObjCraft.rect.x;
    (*p).inv.rectObjCraft.y = (*p).inv.caseObjCraft.rect.y;
    (*p).inv.rectObjCraft.w = 50;
    (*p).inv.rectObjCraft.h = 50;

    // inventaire global
    (*p).inv.inventory.x = (*p).inv.rectPrin.x -40 ;
    (*p).inv.inventory.y = (*p).inv.rectPrin.y -40 ;
    (*p).inv.inventory.w = (*p).inv.rectPrin.w + (*p).inv.rectArmure.w + 100;
    (*p).inv.inventory.h = (*p).inv.rectPrin.h + (*p).inv.rectMain.h + 80;
    (*p).inv.fond = LoadTexture2(game.ecran.renderer,"src/gfx/fondInventaire.png");

}

void replace(Case *case1){// on replace l'objet dans sa case
    case1->obj->rect.x = case1->rect.x + case1->rect.w/2 -  case1->obj->rect.w/2; 
    case1->obj->rect.y = case1->rect.y + case1->rect.h/2 -  case1->obj->rect.h/2;
}

void echangerCase(Case *Case1 , Case *Case2){ // échange les contenus des 2 cases / case1 est la case de depart, case2 arrivé
    Obj temp = NULL;

    temp = Case1->obj;
    Case1->obj = Case2->obj;
    Case2->obj = temp;

   if((*Case2).isFilled == true){ // si a l'arrivé il y avait un objet
        (*Case1).isFilled = true; // la case de depart a donc un objet
        replace(Case1);
    }
    else{
        (*Case1).isFilled = false;
    }
    (*Case2).isFilled = true;
    replace(Case2);
}

bool attraperObjet(int x, int y, Case **caseSelect){ // attrape un objet dans l'inventaire (appel lors d'un clic et inventaire ouvert)
    //detecte la zone
    int colonne = 0,ligne = 0;
    bool objIsSelect = false;

    if(collidePtRect(x,y,mon_player.inv.rectArmure) == 1){    // si la souris est dans la zone de l'armure
        // calcul de la ligne et colonne sur laquelle on est 
        colonne = floor((x - mon_player.inv.rectArmure.x) /50); 
        ligne = floor((y - mon_player.inv.rectArmure.y) /50); 
        *caseSelect = &mon_player.inv.armure[colonne];
        if((*caseSelect)->isFilled == true)
            objIsSelect = true;
    }   
    if(collidePtRect(x,y,mon_player.inv.rectCraft) == 1){   // si la souris est dans la zone de craft
        colonne = floor((x - mon_player.inv.rectCraft.x) /50);  
        ligne = floor((y - mon_player.inv.rectCraft.y) /50);  
        *caseSelect = &mon_player.inv.craft[colonne];
        if((*caseSelect)->isFilled == true)
            objIsSelect = true;
     }   
    if(collidePtRect(x,y,mon_player.inv.rectMain) == 1){     // si la souris est dans la zone de raccourci
        colonne = floor((x - mon_player.inv.rectMain.x) /50);  
        ligne = floor((y - mon_player.inv.rectMain.y) /50); 
        *caseSelect = &mon_player.inv.inventairemain[colonne];
        if((*caseSelect)->isFilled == true)
            objIsSelect = true;
     }   
    if(collidePtRect(x,y,mon_player.inv.rectPrin) == 1){    // si la souris est dans la zone principale
        colonne = floor((x - mon_player.inv.rectPrin.x) /50);  
        ligne = floor((y - mon_player.inv.rectPrin.y) /50);  
        *caseSelect = &mon_player.inv.inventairePrin[ligne][colonne];
        if((*caseSelect)->isFilled == true)
            objIsSelect = true;
    } 

    if(collidePtRect(x,y,mon_player.inv.rectObjCraft) == 1){ // case obj craftable
        *caseSelect = &mon_player.inv.caseObjCraft;
        if((*caseSelect)->isFilled == true)
            objIsSelect = true;
    } 

    return objIsSelect;
}

bool verifCollidePlayer(double oldX, double oldY, double oldCamX, double  oldCamY){ // verifie les collisions du player

     
    bool isCollide = false;
    SDL_Rect boxPlayer = {mon_player.x+game.ecran.camera.x, mon_player.y+game.ecran.camera.y , mon_player.w-1, mon_player.h-1};// box du player
    bool collide = false;
    Ennemies e = NULL;
     bool collide3  = false;
    if(mon_player.isInDonjon){
        Donjons mon_donjon = tab_donjon[mon_player.id_donjon];
        e = mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_ennemis;
        if(collideDonjon(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], boxPlayer) == 1){ // si collision
            // on regarde si la collision est en X ou Y (permet de pouvoir glisser contre les murs lorsqu on appuie sur 2 touches)
            SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, oldY+oldCamY, mon_player.w-1, mon_player.h-1}; // coordonées X actuel et ancien Y
            if(collideDonjon( mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], hit_box) == 1){
                game.ecran.camera.x = oldCamX; // on reprend les anciennes positions X
                mon_player.x = oldX; 
                collide = true;
            }
            SDL_Rect hit_box2 = {oldX+oldCamX, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};// coordonées Y actuel et ancien X
            if(collideDonjon( mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], hit_box2) == 1){ 
                game.ecran.camera.y = oldCamY; // on reprend les anciennes positions Y
                mon_player.y = oldY;
                collide = true;
            }
            if(collide == false){ // pour gerer les cas ou il y a collision uniquement avec X ET Y actuel (lors d une collision avec le coin d une image)
                game.ecran.camera.x = oldCamX; // on reprend les anciennes positions
                game.ecran.camera.y = oldCamY;
                mon_player.x = oldX; 
                mon_player.y = oldY;
                collide = true;
            } 
        }    
    }
    else{ // player hors du donjon
        e = listeEnnemie;
        if(collideRectMap(boxPlayer) == 1){ // si collision 
            // on regarde si la collision est en X ou Y (permet de pouvoir glisser contre les murs lorsqu on appuie sur 2 touches)
            SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, oldY+oldCamY, mon_player.w-1, mon_player.h-1}; // coordonées X actuel et ancien Y
            if(collideRectMap(hit_box)){
                game.ecran.camera.x = oldCamX; // on reprend les anciennes positions X
                mon_player.x = oldX; 
                collide = true;
            }
            SDL_Rect hit_box2 = {oldX+oldCamX, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};// coordonées Y actuel et ancien X
            if(collideRectMap(hit_box2) == 1){ 
                game.ecran.camera.y = oldCamY; // on reprend les anciennes positions Y
                mon_player.y = oldY;
                collide = true;
            }
            if(collide == false){ // pour gerer les cas ou il y a collision uniquement avec X ET Y actuel (lors d une collision avec le coin d une image)
                game.ecran.camera.x = oldCamX; // on reprend les anciennes positions
                game.ecran.camera.y = oldCamY;
                mon_player.x = oldX; 
                mon_player.y = oldY;
                collide = true;
            }     
        }
        // ============== collisions animaux ========================
        Animaux a = listeAnimaux;
        collide3 = false ;
        while(a != NULL){
            SDL_Rect boxAni = {a->hitbox.x, a->hitbox.y, a->hitbox.w-1, a->hitbox.h-1};
            if(collideRectRect(boxPlayer, boxAni) == 1){ // si collision avec l'animal
                // on regarde si la collision est en X ou Y (permet de pouvoir glisser contre les murs lorsqu on appuie sur 2 touches)
                SDL_Rect hit_box3 = {mon_player.x+game.ecran.camera.x, oldY+oldCamY, mon_player.w-1, mon_player.h-1}; // coordonées X actuel et ancien Y
                if(collideRectRect(hit_box3, boxAni) == 1){
                    game.ecran.camera.x = oldCamX; // on reprend les anciennes positions X
                    mon_player.x = oldX; 
                    collide3 = true;
                }
                
                SDL_Rect hit_box4 = {oldX+oldCamX, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};// coordonées Y actuel et ancien X
                if(collideRectRect(hit_box4, boxAni) == 1){ 
                    game.ecran.camera.y = oldCamY; // on reprend les anciennes positions Y
                    mon_player.y = oldY;
                    collide3 = true;
                }
                if(collide3 == false){ // pour gerer les cas ou il y a collision uniquement avec X ET Y actuel (lors d une collision avec le coin d une image)
                    game.ecran.camera.x = oldCamX; // on reprend les anciennes positions
                    game.ecran.camera.y = oldCamY;
                    mon_player.x = oldX; 
                    mon_player.y = oldY;
                    collide3 = true;
                }
            }
            a = a->suiv;
        }   
    }

    // ===================== vérification collision ennemis =====================
    
    bool collide2 = false;
    while (e != NULL){ // parcour la liste d'ennemis
        SDL_Rect boxEnn = {e->hitbox.x, e->hitbox.y, e->hitbox.w-1, e->hitbox.h-1};
        if(collideRectRect(boxPlayer, boxEnn) == 1){ // si collision avec l'ennemi
            // on regarde si la collision est en X ou Y (permet de pouvoir glisser contre les murs lorsqu on appuie sur 2 touches)
            SDL_Rect hit_box = {mon_player.x+game.ecran.camera.x, oldY+oldCamY, mon_player.w-1, mon_player.h-1}; // coordonées X actuel et ancien Y
            if(collideRectRect(hit_box, boxEnn) == 1){
                game.ecran.camera.x = oldCamX; // on reprend les anciennes positions X
                mon_player.x = oldX; 
                collide2 = true;
            }
            SDL_Rect hit_box2 = {oldX+oldCamX, mon_player.y+game.ecran.camera.y, mon_player.w-1, mon_player.h-1};// coordonées Y actuel et ancien X
            if(collideRectRect(hit_box2, boxEnn) == 1){ 
                game.ecran.camera.y = oldCamY; // on reprend les anciennes positions Y
                mon_player.y = oldY;
                collide2 = true;
            }
            if(collide2 == false){ // pour gerer les cas ou il y a collision uniquement avec X ET Y actuel (lors d une collision avec le coin d une image)
                game.ecran.camera.x = oldCamX; // on reprend les anciennes positions
                game.ecran.camera.y = oldCamY;
                mon_player.x = oldX; 
                mon_player.y = oldY;
                collide2 = true;
            }
        }
        e = e->suiv;
    }
    

    return collide || collide2 ||collide3;
}

bool AjoutObjCase(Case *case1, Obj objet){ // ajoute un objet a une case, retourne true si l'objet a été placé, false sinon
    bool place = false;

    if(case1->isFilled){ // si il y a deja un objet dans la case
        if(case1->obj->id == objet->id && (case1->obj->nb + objet->nb) < 10){// ils ont le meme nom et stack pas max
            case1->obj->nb += objet->nb;
            libererObjet(objet);
            objet = NULL;
            place = true;  
        }
        else{
            place = false;
        }
            
    }
    else{ // si pas d'objet
        // on fait le transfert
        case1->obj = objet;
        replace(case1);
        case1->isFilled = true;
        place = true;
    }
    return place;
}

int recherche(int p_id, ObjCraft obj){ // retourne la quantité de ressource neccesaire au craft d'un obj
    int nb = 0;

    if(obj.id1 == p_id)
        nb = obj.nb_id1;
    if(obj.id2 == p_id)
        nb = obj.nb_id2;
    if(obj.id3 == p_id)
        nb = obj.nb_id3;

    return nb;
}

void depense(){ // depense les ressources neccessaire au craft d'un obj
    int nb = 0;
    ObjCraft obj;

    for(int j = 0; j<MAX; j++){ // parcour liste obj craftable
        if(mon_player.inv.caseObjCraft.obj->id == tab[j].idObjC){  
            obj = tab[j];        
            break;
        }
    }

    for(int i = 0; i<3; i++){// parcour inventaire craft
        if(mon_player.inv.craft[i].isFilled == true){
            nb = recherche(mon_player.inv.craft[i].obj->id, obj); // recherche de la quantité necessaire
            mon_player.inv.craft[i].obj->nb -= nb;
            if(mon_player.inv.craft[i].obj->nb == 0){
                libererObjet(mon_player.inv.craft[i].obj);
                mon_player.inv.craft[i].obj = NULL;
                mon_player.inv.craft[i].isFilled = false;
            }
        }
    }
    mon_player.xp_lvl += 1;
}

void poserInInventory(int x, int y, Case *caseSelect){// pose un objet pris a la souris dans l'inventaire
    int ligne = 0, colonne = 0;
    bool poser = false;

    // on regarde dans quelle zone la souris est 

    if(collidePtRect(x,y, mon_player.inv.rectArmure) == 1){ // inventaire armure
        poser = true;
        colonne = floor((x -  mon_player.inv.rectArmure.x) /50);
        if(mon_player.inv.armure[colonne].id != caseSelect->id){ // si les 2 cases sont differentes
            if(caseSelect->obj->type == ARMURES){
                if(AjoutObjCase(&mon_player.inv.armure[colonne], caseSelect->obj)){ // si on a réussit à l'ajouter 
                    mon_player.protection += caseSelect->obj->protection;
                    if(caseSelect->type == OBJ_A_CRAFT){
                        depense();
                    }
                    caseSelect->isFilled = false;
                    caseSelect->obj = NULL;   
                }
                else{
                    if(caseSelect->type == OBJ_A_CRAFT){
                        replace(caseSelect);
                    }
                    else{
                        echangerCase(caseSelect,&mon_player.inv.armure[colonne]);
                    }         
                }
            }
            else{
                replace(caseSelect);
            }
        }
        else{
            replace(caseSelect);
        }

    }
    else if(collidePtRect(x,y, mon_player.inv.rectCraft)==1){ // inventaire craft
        poser = true;
        colonne = floor((x -  mon_player.inv.rectCraft.x) /50);
        if(mon_player.inv.craft[colonne].id != caseSelect->id){ // si les 2 cases sont differentes
            if(AjoutObjCase(&mon_player.inv.craft[colonne], caseSelect->obj)){ // si on a réussit l'ajouter 
                if(caseSelect->type == OBJ_A_CRAFT){
                   depense();
                }
                else if(caseSelect->type == ARMURE){
                    mon_player.protection -= caseSelect->obj->protection;
                }
                caseSelect->isFilled = false;
                caseSelect->obj = NULL;
            }
            else{
                if(caseSelect->type == OBJ_A_CRAFT){
                    replace(caseSelect); 
                }
                else{
                    echangerCase(caseSelect,&mon_player.inv.craft[colonne]);
                }         
            }
        }
        else{
            replace(caseSelect);            
        }
    }
    else if(collidePtRect(x,y, mon_player.inv.rectMain)==1){
        poser = true;
        colonne = floor((x -  mon_player.inv.rectMain.x) /50);  
        if(mon_player.inv.inventairemain[colonne].id != caseSelect->id){ // si les 2 cases sont differentes
            if(AjoutObjCase(&mon_player.inv.inventairemain[colonne], caseSelect->obj)){ // si on a su l'ajouter 
                if(caseSelect->type == OBJ_A_CRAFT){
                   depense();
                }  
                else if(caseSelect->type == ARMURE){
                    mon_player.protection -= caseSelect->obj->protection;
                }
                caseSelect->isFilled = false;
                caseSelect->obj = NULL;
            }
            else{
                if(caseSelect->type == OBJ_A_CRAFT){
                    replace(caseSelect);
                }
                else{
                    echangerCase(caseSelect,&mon_player.inv.inventairemain[colonne]);
                }         
            }
        }
        else{
            replace(caseSelect);           
        }
    }
    else if(collidePtRect(x, y, mon_player.inv.rectPrin)==1){ // inventaire principal
        poser = true;
        colonne = floor((x -  mon_player.inv.rectPrin.x)/50);  
        ligne = floor((y -  mon_player.inv.rectPrin.y) /50); 
        if(mon_player.inv.inventairePrin[ligne][colonne].id != caseSelect->id){ // si les 2 cases sont differentes
            if(AjoutObjCase(&mon_player.inv.inventairePrin[ligne][colonne], caseSelect->obj)){ // si on a su l'ajouter 
                if(caseSelect->type == OBJ_A_CRAFT){
                   depense();
                }
                else if(caseSelect->type == ARMURE){
                    mon_player.protection -= caseSelect->obj->protection;
                }
                caseSelect->isFilled = false;
                caseSelect->obj = NULL;
            }
            else{
                if(caseSelect->type == OBJ_A_CRAFT){
                    replace(caseSelect);
                }
                else{
                    echangerCase(caseSelect,&mon_player.inv.inventairePrin[ligne][colonne]);
                }         
            }
        }
        else{
            replace(caseSelect);           
        }
    }
    else{
        if(collidePtRect(x,y,mon_player.inv.inventory) == 1){
            poser = true;
            replace(caseSelect);
        }
    }

    if(poser == false){ //si l'objet n'a pas été posé
        if (lacherObjet(caseSelect->obj, x, y) == true){// s il est laché
            caseSelect->obj = NULL; // on le retrire de sa case
            caseSelect->isFilled = false; // la case est vide            
        }
        else{ 
            replace(caseSelect);
        }
    }
}

bool lacherObjet(Obj objet, int x, int y){ // lache un objet au sol
    int lacher = false;
    double orientX = 0.0, orientY = 0.0;

    if(x > mon_player.inv.inventory.x + mon_player.inv.inventory.w ){//lache a droite du player
        objet->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 - objet->rect.w/2 +80;
        objet->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - objet->rect.h /2;
   }
   else if(x < mon_player.inv.inventory.x ){// lache a gauche du player
        objet->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 - objet->rect.w /2 -80;
        objet->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - objet->rect.h /2;
   }
   else  if(y > mon_player.inv.inventory.y + mon_player.inv.inventory.h ){//lache en dessous du player
        objet->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 - objet->rect.w /2;
        objet->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2+ - objet->rect.h/2 + 80;
   }
   else if(y < mon_player.inv.inventory.y){//lache au dessus du player
        objet->rect.x = game.ecran.camera.x +  mon_player.x+mon_player.w/2 - objet->rect.w /2;
        objet->rect.y = game.ecran.camera.y +  mon_player.y+mon_player.h/2 - objet->rect.h /2 -80;
   }


    if(mon_player.isInDonjon){
        Donjons mon_donjon = tab_donjon[mon_player.id_donjon];        
        if(collideDonjon(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu], objet->rect) != 1){ // si pas collision avec la map
            printf("Lache");
            lacher = true;
            mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets = ajouter_tete(mon_donjon->tab_salles[mon_donjon->ligneActu][mon_donjon->colonneActu]->liste_objets, objet);// on ajoute l'objet aux objets dans le donjon
        }
        else{
        }
    }
    else{
        if(collideRectMap(objet->rect) != 1){ // si pas collision avec la map
            lacher = true;
            liste_objets = ajouter_tete(liste_objets, objet);// on ajoute l'objet aux objets sur la map
        }
        else{
        }
    }


    
    return lacher;
}

void libererTextureInventaire(){// libere l'espace memoire des textures des inventaires
     for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j< 9; j++)
        {
            if(mon_player.inv.inventairePrin[i][j].isFilled != false){
                libererObjet(mon_player.inv.inventairePrin[i][j].obj);
            }
            SDL_DestroyTexture(mon_player.inv.inventairePrin[i][j].fond);
            mon_player.inv.inventairePrin[i][j].fond = NULL;
        }
    }
    for (int k = 0; k < 9; k++){
        if(mon_player.inv.inventairemain[k].isFilled != false){
            libererObjet(mon_player.inv.inventairemain[k].obj);
        }
        SDL_DestroyTexture(mon_player.inv.inventairemain[k].fond);
        mon_player.inv.inventairemain[k].fond = NULL;
    }
    for (int k = 0; k < 3; k++){
        if(mon_player.inv.craft[k].isFilled != false){
            libererObjet(mon_player.inv.craft[k].obj);
        }
        SDL_DestroyTexture(mon_player.inv.craft[k].fond);
        mon_player.inv.craft[k].fond = NULL;
    }
    for (int k = 0; k < 4; k++){
        if(mon_player.inv.armure[k].isFilled != false){
            libererObjet(mon_player.inv.armure[k].obj);
        }
        SDL_DestroyTexture(mon_player.inv.armure[k].fond);
        mon_player.inv.armure[k].fond = NULL;
    }
    // case obj craft
    SDL_DestroyTexture(mon_player.inv.caseObjCraft.fond);
    mon_player.inv.caseObjCraft.fond = NULL;
    if(mon_player.inv.caseObjCraft.isFilled)
        libererObjet(mon_player.inv.caseObjCraft.obj);

    SDL_DestroyTexture(mon_player.inv.fond);
}

int ajoutInventaire(Obj objet, Player *p){ // ajout d'un objet dans l inventaire principale / return 1 si on a placé l objet, 0 sinon

    bool trouver = false;
    bool trouverCase = false;
    Case *temp = NULL;
    int diff;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 9 ; j++){
            if((*p).inv.inventairePrin[i][j].isFilled == true && trouver == false){ // s il y a un objet dans la case ET objet pas encore trouvé
                if((*p).inv.inventairePrin[i][j].obj->id == objet->id ){ // verifie si c est le meme type d'objet
                    if( ((*p).inv.inventairePrin[i][j].obj->nb + objet->nb) < 10){ // si on peut le placer dans un meme stack
                        trouver = true;
                        (*p).inv.inventairePrin[i][j].obj->nb += objet->nb;
                    }
                    else{//si on doit remplir un stack et en crée un autre
                        diff = 9 - (*p).inv.inventairePrin[i][j].obj->nb;
                        (*p).inv.inventairePrin[i][j].obj->nb += diff;
                        objet->nb = objet->nb - diff;
                    }
                }
            }
            else{
                if(trouverCase == false){
                    trouverCase = true;
                    temp = &((*p).inv.inventairePrin[i][j]);
                }
            }
        }
    } 

    if(trouver == false){ // si pas trouvé
        if(temp != NULL){ // si on a trouver une place
            temp->obj = createObjet(objet->id, temp->rect.x, temp->rect.y);// on le place
            replace(temp);
            temp->obj->nb = objet->nb;
            temp->isFilled = true;
            return 1;
        }
        else // si plus de place
            return 0;
    }

    return 1;
}

bool restaurationPlayer(){ // fais manger/boire le player
    int i = 0;
    int caseSelect = -1;

    for(i=0;i<9;i++){ // détection de la case d'inventaire selectionné
        if(mon_player.inv.inventairemain[i].isSelect){
            caseSelect = i;
        }
    }

    if(mon_player.inv.inventairemain[caseSelect].isFilled){ // si remplie
        if(mon_player.inv.inventairemain[caseSelect].obj->id == 4){ // si c est de la viande
            mon_player.barre_manger += 5;
            mon_player.vie += 20;
            if(mon_player.barre_manger > mon_player.max_faim)
                mon_player.barre_manger = mon_player.max_faim;
            if(mon_player.vie > mon_player.vie_max)
                mon_player.vie = mon_player.vie_max;
        }
        if(mon_player.inv.inventairemain[caseSelect].obj->id == 4){ // si boisson
            mon_player.barre_soif +=5;
            if(mon_player.barre_soif > mon_player.max_soif)
                mon_player.barre_soif = mon_player.max_soif;
        }

        mon_player.inv.inventairemain[caseSelect].obj->nb -= 1; // consomme

        if(mon_player.inv.inventairemain[caseSelect].obj->nb == 0){
            libererObjet(mon_player.inv.inventairemain[caseSelect].obj);
            mon_player.inv.inventairemain[caseSelect].obj = NULL;
            mon_player.inv.inventairemain[caseSelect].isFilled = false;
            return true;
        }
    }
    return false;
}

void detecterBiome(){ // detecte le biome dans lequel le player se situe

    int ligne = (int)(mon_player.y + game.ecran.camera.y) /hauteur_tile;
    int colonne =(int) (mon_player.x + game.ecran.camera.x) / largeur_tile;
    
    if(mon_player.isInDonjon == false){
        if(map_test[ligne][colonne] == 85  ){
            mon_player.biome = NEIGE;
               // temperature = -15;
        }
        else if(map_test[ligne][colonne] == 10 ||map_test[ligne][colonne] == 21 ){
            mon_player.biome= PLAINE;
              //  temperature = 15;
        }
        else if(map_test[ligne][colonne] == 13 ){
            mon_player.biome = DESERT;
               // temperature = 35;
        }
    }
    else mon_player.biome = DONJON;

}

void gestionTemperature(){ // gere la temperature selon le biome
    if(mon_player.biome == NEIGE){
        if(game.journee.heure > 22 || game.journee.heure <=6){
            if(mon_player.protection == 0)mon_player.temperature =FROIDGLACIAL;
            else if(mon_player.protection == 0.5)mon_player.temperature =FROIDGLACIAL;
            else if(mon_player.protection == 1)mon_player.temperature =FROID;
            else if(mon_player.protection == 1.5)mon_player.temperature =FROID;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = -40;
        }
        
        else if(game.journee.heure > 6 &&game.journee.heure <=12){
            if(mon_player.protection == 0)mon_player.temperature =FROIDGLACIAL;
            else if(mon_player.protection == 0.5)mon_player.temperature =FROID;
            else if(mon_player.protection == 1)mon_player.temperature =FROID;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = -20;
        }
        else if(game.journee.heure > 12 &&game.journee.heure <=16){
            if(mon_player.protection == 0)mon_player.temperature =FROID;
            else if(mon_player.protection == 0.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = 0;
        }
        else if(game.journee.heure > 16 &&game.journee.heure <=22){
            if(mon_player.protection == 0)mon_player.temperature =FROIDGLACIAL;
            else if(mon_player.protection == 0.5)mon_player.temperature =FROIDGLACIAL;
            else if(mon_player.protection == 1)mon_player.temperature =FROID;
            else if(mon_player.protection == 1.5)mon_player.temperature =FROID;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = -20;
        }
    }
    else if(mon_player.biome == DESERT){
        if(game.journee.heure > 22 || game.journee.heure <=6){
            if(mon_player.protection == 0)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 0.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = 10;
        }
        else if(game.journee.heure > 6 &&game.journee.heure <=12){
            if(mon_player.protection == 0)mon_player.temperature =DESERTIQUE;
            else if(mon_player.protection == 0.5)mon_player.temperature =DESERTIQUE;
            else if(mon_player.protection == 1)mon_player.temperature =CHAUD;
            else if(mon_player.protection == 1.5)mon_player.temperature =CHAUD;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = 30;
        }
        else if(game.journee.heure > 12 &&game.journee.heure <=16){
            if(mon_player.protection == 0)mon_player.temperature =DESERTIQUE;
            else if(mon_player.protection == 0.5)mon_player.temperature =DESERTIQUE;
            else if(mon_player.protection == 1)mon_player.temperature =DESERTIQUE;
            else if(mon_player.protection == 1.5)mon_player.temperature =CHAUD;
            else if(mon_player.protection == 2)mon_player.temperature =CHAUD;
            temperature = 40;
        }
        else if(game.journee.heure > 16 &&game.journee.heure <=22){
            if(mon_player.protection == 0)mon_player.temperature =CHAUD;
            else if(mon_player.protection == 0.5)mon_player.temperature =CHAUD;
            else if(mon_player.protection == 1)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = 30;
        }
    }
    else if(mon_player.biome == PLAINE){
        if(game.journee.heure > 22 || game.journee.heure <=6){
            if(mon_player.protection == 0)mon_player.temperature =FROID;
            else if(mon_player.protection == 0.5)mon_player.temperature =FROID;
            else if(mon_player.protection == 1)mon_player.temperature =FROID;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = -10;
        }
        else if(game.journee.heure > 6 &&game.journee.heure <=12){
            if(mon_player.protection == 0)mon_player.temperature =FROID;
            else if(mon_player.protection == 0.5)mon_player.temperature =FROID;
            else if(mon_player.protection == 1)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = 10;
        }
        else if(game.journee.heure > 12 &&game.journee.heure <=16){
            if(mon_player.protection == 0)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 0.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =CHAUD;
            temperature = 25;
        }
        else if(game.journee.heure > 16 &&game.journee.heure <=22){
            if(mon_player.protection == 0)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 0.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 1.5)mon_player.temperature =AMBIANT;
            else if(mon_player.protection == 2)mon_player.temperature =AMBIANT;
            temperature = 10;
        }
    }
    
}
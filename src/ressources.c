#include "ressources.h"
#include <stdio.h>
#include <stdlib.h>
#include "craft.h"
#include "fonctions.h"
#include "player.h"
#include "game.h"
#include "map.h"
#include "menu.h"
#include "play.h"

void triIdRessource(Ressource *res){ // tri les ID de la ressource par ordre croissant (les valeurs -1 sont placé a la fin)
    if(res->idDrop1 > res->idDrop2 && res->idDrop2 != -1){
        int temp = res->idDrop1;
        res->idDrop1 = res->idDrop2;
        res->idDrop2 = temp;
    }
    else if(res->idDrop1 == -1){
        int temp = res->idDrop1;
        res->idDrop1 = res->idDrop2;
        res->idDrop2 = temp;
    }
}

int lireRecolter(){ // lis le ficher "recolte.txt" pour récupérer chaque ressource pour les trier et stocker dans "tabRessource"
    FILE *f;
    f = fopen("src/recolte.txt","r");

    if (f != NULL){
        Ressource res = {0,0,0,0};
		char buffer[256];
        int compteur = 0;
        int tailleTab = 0;

		while (fgets(buffer, 256, f) != NULL){
            if(compteur == 0){
                res.idCasse = atoi(buffer);
                 compteur++;
            }
            else if(compteur ==1){
                res.idDrop1 = atoi(buffer);
                compteur++;
            }    
            else if(compteur ==2){
                res.idDrop2 = atoi(buffer);
                compteur++;
            }    
            else if(compteur == 3){
                res.idRemplace = atoi(buffer);
                compteur++;
            }
            else if(compteur == 4){
                compteur = 0;
                triIdRessource(&res);
                insererInTabRessource(res,tailleTab);
                tailleTab++;
            }
           
		}
		if(fclose(f) != EOF);
        else fprintf(stderr, "Erreur fclose\n");
	}
    else{
        fprintf(stderr,"Fichier null\n");
        return -1;
    }
    return 1;
}

void insererInTabRessource(Ressource res,int tailleTab){ // insere une ressource dans le tableau
    int index = -1;

    if(tailleTab != 0)
        index = rechercheDicoInsereRessource(res,0,tailleTab,tailleTab);
    else 
        index = 0;

    for(int i = tailleTab-1; i >= index; i--){
        tabRessource[i+1] = tabRessource[i];
    }
    tabRessource[index] = res;

}

int rechercheDicoRessource(Ressource res ,int bInf,int bSup){ // recherche dicho d'une ressource dans le tableau, retourne sa position ou -1 si pas trouvé 
    int milieu = (int)( (bSup + bInf)/2);
 
    if(bInf <= bSup ){
        if(tabRessource[milieu].idCasse == res.idCasse){
            return milieu;
        }
        else if (tabRessource[milieu].idCasse < res.idCasse){
            return rechercheDicoRessource(res,milieu+1,bSup);
        }
        else{
            return rechercheDicoRessource(res,bInf,milieu-1);
        }

    }
         
    return -1;
}

int rechercheDicoInsereRessource(Ressource res ,int bInf,int bSup,int tailleTab){ // insere un objet craftable dans le tab de facon dichotomique
    int milieu = (int)( (bSup + bInf)/2);

     if(bInf <= bSup ){
        if(tabRessource[milieu].idCasse == res.idCasse){
            return bInf;
        }
        else if(tabRessource[milieu].idCasse < res.idCasse){ 
            return rechercheDicoInsereRessource(res,milieu+1,bSup,tailleTab);
        }
        else{
            return rechercheDicoInsereRessource(res,bInf,milieu-1,tailleTab);
        }
    }
    else{
        if(bInf > tailleTab)return tailleTab;
        return bInf;
        
    }
}

void detecterREssource(int mouseX,int mouseY){ // detecte si on peut recolter la ressource et ajoute a l'inventaire (apel lors d'un clique de souris)
    int ressouces[] = {};
    int id[] = {};
    int midPlayerX = mon_player.x + mon_player.w /2;
    int midPlayerY = mon_player.y + mon_player.h /2;
    int taille = nbLignesinFile("src/recolte.txt") / 5;
    int selectMain;

    for(int i = 0;i<9;i++){ // detection de,la case selectionné
        if( mon_player.inv.inventairemain[i].isSelect){
            selectMain = i;
        }
    }

    if(mon_player.inv.inventairemain[selectMain].isFilled){ // si case remplie
        if(distance2Pts(mouseX,mouseY,midPlayerX,midPlayerY) < 50 && minimap == false && (mon_player.inv.inventairemain[selectMain].obj->id == 1 || mon_player.inv.inventairemain[selectMain].obj->id == 2 )){ // si proche de la tile et l'objet est une pioche ou hache
            int colonne = (mouseX+game.ecran.camera.x) / largeur_tile;
            int ligne = (mouseY+game.ecran.camera.y) / hauteur_tile;
            Obj new;
            Ressource res ;
            int idMain  = mon_player.inv.inventairemain[selectMain].obj->id;  
            int id = map_test[ligne][colonne]; // recupere la tile ou le joueur a clique
            if(id > nb_tiles){ // verifie si la tile est solide
                res.idCasse = id - nb_tiles; // remet l'id de base
                int index =  rechercheDicoRessource(res,0,taille); // recherche si on peut craft
                if(index != -1){ //si c'est une ressource
                    Mix_PlayChannel(3,breakWoods,0);          
                    res = tabRessource[index];// recupere la ressource dans le tableau
                    map_test[ligne][colonne] = res.idRemplace;
                    new = createObjet(res.idDrop1,mouseX,mouseY);
                    if(mon_player.inv.inventairemain[selectMain].obj->id == 1){
                        new->nb = random(1, 4)*2; // coef *2 hache
                    }
                    else{
                        new->nb = random(1, 4);
                    }

                    ajoutInventaire(new,&mon_player);
                    if(res.idDrop2 != -1){// si l'objet drop 2 ressource
                        new = createObjet(res.idDrop2,mouseX,mouseY);
                        new->nb = (int)(rand()%3 + 1);
                        if(mon_player.inv.inventairemain[selectMain].obj->id == 2){
                          new->nb = random(1, 4)*2; // coef *2 pioche
                        }
                        else{
                            new->nb = random(1, 4);
                        }
                        ajoutInventaire(new,&mon_player);
                    }
                }
            }
        }
    }
}
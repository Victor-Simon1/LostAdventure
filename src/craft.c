#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "craft.h"
#include "game.h"
#include "fonctions.h"
#include "donjon.h"

int nbObjetinFIle(char nomFichier[]){ // renvoie le nb d'objet dans un fichier
    return nbLignesinFile(nomFichier) / 5;
}

int lectureFile(char nomFichier[]){ // lis le fichier d'objet craftables et les inserent dans le tableau
    int nbObj = nbObjetinFIle(nomFichier);
    if(nbObj < MAX){
        FILE *f;
        ObjCraft obj;
        f = fopen(nomFichier,"r");
        char buffer[256];
        int compteur=0;
        int i = 0;
        int tailleTab = 0;
        char delim[] = ",";
        char *ptr = NULL;

        if(f != NULL){
            while(fgets(buffer,256,f) != NULL){
               // printf("buffer %s --- compteur %d\n",buffer,compteur);
                ptr = strtok(buffer, delim);
                if(compteur ==0){
                    obj.idObjC = atoi(buffer);
                }
                else if(compteur ==1){
                    obj.id1 = atoi(ptr);
                    ptr = strtok(NULL, delim);
                    obj.nb_id1 = atoi(ptr);
                }
                else if(compteur ==2){
                    obj.id2 = atoi(ptr);
                    ptr = strtok(NULL, delim);
                    obj.nb_id2 = atoi(ptr);
                }  
                else if(compteur ==3){
                    obj.id3 = atoi(ptr);
                    ptr = strtok(NULL, delim);
                    obj.nb_id3 = atoi(ptr);
                }    
                else if(compteur == 4 ) {
                    compteur = -1;
                    trieIdObjet(&obj);
                    insererInTab(obj,tailleTab);
                    tailleTab++;
                    i++;
                }
                compteur++;
            }
            if(fclose(f)!=EOF);
            else fprintf(stderr,"ERREUR FCLOSE");
        }
        return nbObj;
    }
    else{
        fprintf(stderr,"Fichier trop grand : MAX %d \n nbObjet %d\n",MAX,nbObj);
        return -1;
    }
}

void insererInTab(ObjCraft obj,int tailleTab){ // insere un objet craftable dans le tableau
    int index = -1;

    if(tailleTab != 0)
        index = rechercheDicoInserer(obj,0,tailleTab,tailleTab);
    else 
        index = 0;

    for(int i = tailleTab-1; i >= index; i--){
        tab[i+1] = tab[i];
    }
    tab[index] = obj;

}

int rechercheDico(ObjCraft obj ,int bInf,int bSup){ // recherche dicho d'un objet craftable dans le tableau  
    int milieu = (int)( (bSup + bInf)/2);

    if(bInf <= bSup ){
        if(tab[milieu].id1 == obj.id1){
            if(tab[milieu].id2 == obj.id2){
                if(tab[milieu].id3 == obj.id3){
                    return milieu;
                }
                else if(tab[milieu].id3 <obj.id3){
                    return rechercheDico(obj,milieu+1,bSup);
                }
                else{
                    return rechercheDico(obj,bInf,milieu-1);
                }
            }
            else if(tab[milieu].id2< obj.id2){
                return  rechercheDico(obj,milieu+1,bSup);
            }
            else{
                return rechercheDico(obj,bInf,milieu-1);
            }
        }
        else if(tab[milieu].id1 < obj.id1){ 
            return rechercheDico(obj,milieu+1,bSup);
        }
        else{
            return rechercheDico(obj,bInf,milieu-1);
        }
    }
    return -1;
}

int rechercheDicoInserer(ObjCraft obj ,int bInf,int bSup,int tailleTab){ // recherche dichotomique une position pour inserer un objet craftable
    int milieu = (int)( (bSup + bInf)/2);
     if(bInf <= bSup ){
        if(tab[milieu].id1 == obj.id1){
            if(tab[milieu].id2 == obj.id2){
                if(tab[milieu].id3 == obj.id3){ //objet trouvé
                    return bInf;
                }
                else if(tab[milieu].id3 <obj.id3){
                    return rechercheDicoInserer(obj,milieu+1,bSup,tailleTab);
                }
                else{
                    return rechercheDicoInserer(obj,bInf,milieu-1,tailleTab);
                }
            }
            else if(tab[milieu].id2< obj.id2){
                 return rechercheDicoInserer(obj,milieu+1,bSup,tailleTab);
            }
            else{
                return rechercheDicoInserer(obj,bInf,milieu-1,tailleTab);
            }
        }
        else if(tab[milieu].id1 < obj.id1){ 
            return rechercheDicoInserer(obj,milieu+1,bSup,tailleTab);
        }
        else{
            return rechercheDicoInserer(obj,bInf,milieu-1,tailleTab);
        }
    }
    else{

        if(bInf > tailleTab)
            return tailleTab;

        return bInf;
        
    }
    
   // return -1;
}

void trieIdObjet(ObjCraft *obj){ // trie par ordre croissant des ID et leur quantité
    int temp,temp2;
    
    while(obj->id1 > obj->id2 || obj->id2 > obj->id3){

        if(obj->id1 > obj->id2 ){
            temp = obj->id1;
            obj->id1 = obj->id2;
            obj->id2 = temp;

            temp2 = obj->nb_id1;
            obj->nb_id1 = obj->nb_id2;
            obj->nb_id2 = temp2;
        }
        if(obj->id2 > obj->id3){
            temp = obj->id2;
            obj->id2 = obj->id3;
            obj->id3 = temp;

            temp2 = obj->nb_id2;
            obj->nb_id2 = obj->nb_id3;
            obj->nb_id3 = temp2;
        }
    }
    for ( int i = 0; i < 2; i++){
        if(obj->id1 == -1){

            temp = obj->id1;
            obj->id1 = obj->id2;
            obj->id2 = temp;

            temp2 = obj->nb_id1;
            obj->nb_id1 = obj->nb_id2;
            obj->nb_id2 = temp2;

            temp = obj->id2;
            obj->id2 = obj->id3;
            obj->id3 = temp;

            temp2 = obj->nb_id2;
            obj->nb_id2 = obj->nb_id3;
            obj->nb_id3 = temp2;
        }
    }
}

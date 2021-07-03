#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "fonctions.h"
#include "game.h"
#include "tirs.h"



Tirs createTir(Direction p_dir, Type p_type, int pX, int pY, int p_spd){ // crée et renvoie un tir
    Tir *mon_tir = NULL;
    mon_tir = (Tirs)malloc(sizeof(Tir));

    mon_tir->distance = 0;
    mon_tir->distance_max = 200;
    mon_tir->suiv = NULL;
    mon_tir->prec = NULL;
    mon_tir->direction = p_dir;
    mon_tir->type = p_type;

    if(p_dir == D_UP){
        mon_tir->img = LoadTexture2(game.ecran.renderer, "src/gfx/fleche_haut.png");
        mon_tir->vx = 0;
        mon_tir->vy = -p_spd;
    }
        
    else if(p_dir == D_DOWN){
        mon_tir->img = LoadTexture2(game.ecran.renderer, "src/gfx/fleche_bas.png");
        mon_tir->vx = 0;
        mon_tir->vy = p_spd;
    }
        
    else if(p_dir == D_RIGHT){
        mon_tir->img = LoadTexture2(game.ecran.renderer, "src/gfx/fleche_droite.png");
        mon_tir->vx = p_spd;
        mon_tir->vy = 0;
    }
        
    else{
        mon_tir->img = LoadTexture2(game.ecran.renderer, "src/gfx/fleche_gauche.png");
        mon_tir->vx = -p_spd;
        mon_tir->vy = 0;
    }
        

    SDL_QueryTexture(mon_tir->img, NULL, NULL, &mon_tir->w, &mon_tir->h);// recupére la largeur/hauteur de l'image
    mon_tir->x = pX - mon_tir->w/2;
    mon_tir->y = pY - mon_tir->h/2;

    return mon_tir;
}

Tirs ajouter_teteTirs(Tirs C){ // ajoute un tir en tete de liste

    C->suiv = Liste_tirs;
    C->prec = NULL;
    if(Liste_tirs)
        Liste_tirs->prec = C;

    return C;
}

Tirs supprimTirs(Tirs tir){ // supprime un tir de la liste
    Tirs temp = tir->suiv;

    if(tir->prec)
         (tir->prec)->suiv = tir->suiv;
    else
        Liste_tirs =  tir->suiv;

    if(tir->suiv)
        (tir->suiv)->prec = tir->prec;

    libererTir(tir);
    return temp;
}

void libererTir(Tirs tir){ // libere un tir
    tir->suiv = NULL;
    tir->prec = NULL;
    SDL_DestroyTexture(tir->img);
    tir->img = NULL;
    free(tir);
    tir = NULL;
}

void libereListeTirs(Tirs tir){ // libere tous les tirs de la liste
    Tirs temp, c = tir;

    while(c){
        temp = c->suiv;
        libererTir(c);
        c = temp;
    }
}

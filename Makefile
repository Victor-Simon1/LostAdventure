prog : player.o load.o main.o donjon.o fonctions.o map.o  gui.o craft.o ennemis.o menu.o play.o ressources.o settings.o tirs.o animaux.o
	gcc -o ./bin/Lost_Adventure player.o load.o main.o donjon.o  fonctions.o map.o gui.o craft.o ennemis.o menu.o play.o ressources.o settings.o tirs.o animaux.o -lmingw32 -Llib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

main.o : src/main.c
	gcc src/main.c -c -Iinclude  -o main.o

player.o : src/player.c
	gcc src/player.c -c -Iinclude  -o player.o

load.o : src/load.c
	gcc src/load.c -c -Iinclude  -o load.o

fonctions.o : src/fonctions.c
	gcc src/fonctions.c -c -Iinclude  -o fonctions.o

map.o : src/map.c
	gcc src/map.c -c -Iinclude  -o map.o

gui.o : src/gui.c
	gcc src/gui.c -c -Iinclude  -o gui.o

donjon.o : src/donjon.c
	gcc src/donjon.c -c -Iinclude  -o donjon.o

craft.o : src/craft.c
	gcc src/craft.c -c -Iinclude  -o craft.o

ennemis.o : src/ennemis.c
	gcc src/ennemis.c -c -Iinclude  -o ennemis.o

menu.o : src/menu.c
	gcc src/menu.c -c -Iinclude  -o menu.o

play.o : src/play.c
	gcc src/play.c -c -Iinclude  -o play.o

ressources.o : src/ressources.c
	gcc src/ressources.c -c -Iinclude  -o ressources.o

settings.o : src/settings.c
	gcc src/settings.c -c -Iinclude  -o settings.o

tirs.o : src/tirs.c
	gcc src/tirs.c -c -Iinclude  -o tirs.o

animaux.o : src/animaux.c
	gcc src/animaux.c -c -Iinclude  -o animaux.o

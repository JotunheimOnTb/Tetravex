ALLEGRO = -Wall `pkg-config --cflags --libs allegro-5.0 allegro_dialog-5.0 allegro_primitives-5.0 allegro_ttf-5.0 allegro_font-5.0 allegro_image-5.0`


TETRAVEX : MAIN.o MENU.o GAME.o
		g++ MAIN.o GAME.o MENU.o -o TETRAVEX $(ALLEGRO)

MAIN.o: MAIN.cc HEADER_MASTER.h HEADER_GAME.h HEADER_MAIN.h
	 	g++ -c MAIN.cc $(ALLEGRO)

MENU.o: MENU.cc HEADER_MASTER.h 
		g++ -c MENU.cc $(ALLEGRO)
 
GAME.o: GAME.cc HEADER_MASTER.h HEADER_GAME.h
		g++ -c GAME.cc $(ALLEGRO)

.PHONY: clean debug

clean:
		rm -f *.o

debug:
		g++ -o TETRAVEX MAIN.cc GAME.cc MENU.cc $(ALLEGRO) -D DEBUG_MODE

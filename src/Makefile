ALLEGRO = -Wall `pkg-config --cflags --libs allegro-5.0 allegro_dialog-5.0 allegro_primitives-5.0 allegro_ttf-5.0 allegro_font-5.0 allegro_image-5.0`


TETRAVEX : MAIN.o MENU.o LOGICA_GIOCO.o
		g++ MAIN.o LOGICA_GIOCO.o MENU.o -o TETRAVEX $(ALLEGRO)

MAIN.o: MAIN.cc HEADER_PRINCIPALE.h HEADER_LOGICA_GIOCO.h HEADER_MAIN.h
	 	g++ -c MAIN.cc $(ALLEGRO)

MENU.o: MENU.cc HEADER_PRINCIPALE.h 
		g++ -c MENU.cc $(ALLEGRO)
 
LOGICA_GIOCO.o: LOGICA_GIOCO.cc HEADER_PRINCIPALE.h HEADER_LOGICA_GIOCO.h
		g++ -c LOGICA_GIOCO.cc $(ALLEGRO)

.PHONY: clean debug

clean:
		rm -f *.o

debug:
		g++ -o TETRAVEX MAIN.cc LOGICA_GIOCO.cc MENU.cc $(ALLEGRO) -D DEBUG_MODE

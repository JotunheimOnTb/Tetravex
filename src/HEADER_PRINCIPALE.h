
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>

using namespace std;
/**
* con l'enum Mouse cerchiamo di distinguere il click destro del mouse dal click sinistro.
*/
enum MOUSE {LEFT, RIGHT};
enum LOG_LEVELS
	{
		LOG = 1,
		ERROR = 2
	};

const float FPS = 60; /**< FPS, valore dei frame al secondo che deve mostrare il programma (utilizzato da allegro)*/
const int WIDTH = 850;/**< WIDTH, larghezza in pixel dello schermo del programma*/
const int HEIGHT = 550;/**< HEIGTH, altezza in pixel dello schermo del programma*/
const int DIM_CASELLA = 64; /**< DIM_CASELLA, dimensione in pixel delle ceselle del gioco*/
const int DIST_MATRICI = 400;/**< DIST_MATRICI, distanza delle matrici delle caselle in pixel */


/**
*DEBUG_MODE viene chiamata con tramite makefile debug, verra stampate su terminale tutte i passi del programma per verificare eventuali errori.
*/
#ifdef DEBUG_MODE
	#define DBM(testo, lv) {if (lv & maschera) cout<<testo<<endl; }
#else
	#define DBM(testo, lv)

#endif







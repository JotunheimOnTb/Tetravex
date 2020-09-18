/**
*	Il nuovomenu, è la prima funzione chiamata dal ::main, la quale permette la selezione della difficolta del gioco o di uscire.
*	Questa funzione non prende input e restituisce un valore intero che sara la "difficolta" del gioco che successivamente sarà applicata 
*	alle altre funzioni.
*	
*	
*	@mainpage TETRAVEX
*
* 	
*	@author Daniele Salerno
*
*	@file 
*	File per la gestione del menu' del gioco.
*
*/

#include "HEADER_PRINCIPALE.h"

extern unsigned int maschera;

ALLEGRO_DISPLAY* display;        /**< variabile dello schermo, gernera lo schermo dove vengono caricate le immagini*/
ALLEGRO_TIMER* timer;           /**< variabile del tempo, ogni volta che si riclica genera un evento*/
ALLEGRO_EVENT_QUEUE* event_queue; /**< variabile dove vengono messi in coda gli eventi*/
enum NOME_IMMAGINE{menubase,menuNP,menuESCI,scelta,due,tre,quattro};
/**
* La funzione nuovamenu come primo passo inizializza le variabili di allegro che userà: caricamento su display delle immagini, utilizzo del 
* mouse e tastiera, timer, inizializza una coda in cui verranno caricati gli eventi(come ad esempio input da tastiera o mouse).
* In base al posizionameno e ai click del mouse verra caricata una determinata interfaccia(l'interfaccia della nuova partita o esci o di 
* scelta difficoltà).
* Infine la funzione restituira la difficolta del gioco in base all'immagine finale selezionata. 
*/
int nuovomenu()
{
	
	if(!al_init()) /*Inizializza allegro*/{
      DBM("failed to initialize allegro!",ERROR);
      return -1;
    }
	al_set_new_display_flags(ALLEGRO_WINDOWED);/* Rende il display Windowed*/
    display = al_create_display(WIDTH, HEIGHT);/* Setta la dimenzione del display*/
    if (!display)/*Controlla se il display è stato creato corretamente */
	{
        DBM("failed to create display",ERROR);
		return -1;
	}
    if(!al_init_image_addon())/* Addon che permette il caricamento delle immagini nel display*/ 
	{
    	DBM("failed to initialize allegro image addon!",ERROR);
    	return -1;
    }
    if(!al_install_mouse())/*Installiamo il mouse*/ 
	{
    	DBM("failed to initialize the mouse!",ERROR);
  		return -1;
    }
	if (!al_install_keyboard())/*Installimo la tastiera*/
    {
		DBM("failed to install keyboard",ERROR);
		return -1;	
	}

	al_set_target_bitmap(al_get_backbuffer(display)); /*Setta il bersaglio dei futuri disegni al display*/	

	ALLEGRO_BITMAP *sfondi[7];/* Array delle immagini che caricheremo*/
	for(int i=0; i < 7; i++)/* Inizializzazione dell' array a NULL*/
	{
		sfondi[i]=NULL;
	}
/*
* Carichiamo le immagini nell'arrey
*/
	sfondi[menubase]=al_load_bitmap("../images/MENU.bmp"); 
    	sfondi[menuNP]=al_load_bitmap("../images/Menu-nuovapartita.bmp");
	sfondi[menuESCI]=al_load_bitmap("../images/MENU-esci.bmp");
	sfondi[scelta]=al_load_bitmap("../images/base.bmp");
	sfondi[due]=al_load_bitmap("../images/2x2.bmp");
	sfondi[tre]=al_load_bitmap("../images/3x3.bmp");
	sfondi[quattro]=al_load_bitmap("../images/4x4.bmp");

	
	int mouse_x1 = WIDTH/2;
	int mouse_y1 = HEIGHT/2;

	bool redraw = true;			
   	bool done = true;			
	bool nuova_partita = false;	
	
	timer = al_create_timer(1.0 / FPS); /*Inizzializza il timer con "FPS" eventi al secondo*/
    if(!timer) 
	{
      DBM("failed to create timer!",ERROR);
      return -1;
    }
	al_start_timer(timer); /* Facciamo partire il timer*/

	event_queue = al_create_event_queue(); /* Inizializziamo la coda degli eventi*/
    if(!event_queue)
	{
     DBM("failed to create event_queue!",ERROR);
	}
	/*
	* Settiamo che gli eventi che inseriremo nella coda event_queue verranno dal display, timer mouse e tastiera
	 */
	al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	bool prova[7];
	for (int i = 0 ; i < 7 ; i++)
	{
		prova[i] = false;		
	}
	
	/*
	* Questo ciclo carica la prima interfaccia del gioco, se il giocatore sposta il mouse su determinate cordinate
	* verranno caricate le immagini di "nuova partita" o "esci". Se si verifica un evento del click del mouse all'interno
	* di quelle cordinate verra caricato le immagine di selezione della difficolta ( in caso di click in "nuova partita")
	* o uscirà dalla funzione (in caso di click in "esci").
	*/
	while(done && !nuova_partita)
    {
		
      	ALLEGRO_EVENT ev;
      	al_wait_for_event(event_queue, &ev);
 
		if(ev.type == ALLEGRO_EVENT_TIMER) 
		{
         	redraw = true;
     	}
      	else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         	done = false;
      	}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{	
			mouse_x1 = ev.mouse.x;
         	mouse_y1 = ev.mouse.y;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			//bottone sinistro mouse
			if(ev.mouse.button & 1)								
			{								
				if(mouse_x1>295 && mouse_x1<577 && mouse_y1>266 && mouse_y1<294)
				{
					DBM("nuova partita ",LOG);
					prova[1] = true;
				}
				else if(mouse_x1>386 && mouse_x1<465 && mouse_y1>360 && mouse_y1<390)
				{
					DBM("uscita ",LOG);
					prova[2] = true;				
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
      	{
			if(ev.keyboard.keycode== ALLEGRO_KEY_ESCAPE)
				done = false;
      	}

		//if di disegno
		if(redraw && al_is_event_queue_empty(event_queue)) 
		{	
			
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
			for (int i = 0 ; i < 7 ; i++){
				if (prova[1])
				{
					nuova_partita = true;
				}
				if (prova[2])
				{
					done = false;
				}
				
			}
			
			if(mouse_x1>295 && mouse_x1<577 && mouse_y1>266 && mouse_y1<294)
			{
				al_draw_bitmap(sfondi[menuNP],0,0,0);
				al_flip_display();	
		
			}
			else if(mouse_x1>386 && mouse_x1<465 && mouse_y1>360 && mouse_y1<390)
			{	
	
				al_draw_bitmap(sfondi[menuESCI],0,0,0);
				al_flip_display();
			}
			else
			{	
				al_draw_bitmap(sfondi[menubase],0,0,0);			
				al_flip_display();
			}
		}
	al_flip_display();
	}

	for (int i = 0 ; i < 7 ; i++)
	{
		prova[i] = false;		
	}
	
	/*
	* Si entra in questo ciclo dopo il click su "nuova partita", si caricherà l'interfaccia della scelta difficoltà, e mostrerà
	* le tre opzioni. Come nel ciclo precedente se viene spostato il mouse sulle scritte riguardanti le difficolta e verra 
	* effettuato un click al suo interno, la funzione "nuovomenu" ritornerà al main la difficolta del gioco.
	*/
	while(nuova_partita || done)
	{
		ALLEGRO_EVENT ev;
    	al_wait_for_event(event_queue, &ev);
 
		if(ev.type == ALLEGRO_EVENT_TIMER) 
		{
         	redraw = true;
     	}
      	else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         	done = false;
      	}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		{	
			mouse_x1 = ev.mouse.x;
         	mouse_y1 = ev.mouse.y;
		}
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			//bottone sinistro mouse
			if(ev.mouse.button & 1)								
			{								
				if(mouse_x1>326 && mouse_x1<536 && mouse_y1>238 && mouse_y1<268)
				{
					DBM("partita 2X2 ",LOG);
					al_destroy_bitmap(sfondi[menubase]);
					al_destroy_bitmap(sfondi[menuNP]);
					al_destroy_bitmap(sfondi[menuESCI]);
					al_destroy_bitmap(sfondi[scelta]);
					al_destroy_bitmap(sfondi[due]);
					al_destroy_bitmap(sfondi[tre]);
					al_destroy_bitmap(sfondi[quattro]);
					return 2;
				}
				else if(mouse_x1>328 && mouse_x1<540 && mouse_y1>314 && mouse_y1<344)
				{
					DBM("partita 3X3 ",LOG);
					al_destroy_bitmap(sfondi[menubase]);
					al_destroy_bitmap(sfondi[menuNP]);
					al_destroy_bitmap(sfondi[menuESCI]);
					al_destroy_bitmap(sfondi[scelta]);
					al_destroy_bitmap(sfondi[due]);
					al_destroy_bitmap(sfondi[tre]);
					al_destroy_bitmap(sfondi[quattro]);						
					return 3;				
				}
				else if(mouse_x1>321 && mouse_x1<526 && mouse_y1>396 && mouse_y1<426)
				{
					DBM("partita 4X4 ",LOG);
					al_destroy_bitmap(sfondi[menubase]);
					al_destroy_bitmap(sfondi[menuNP]);
					al_destroy_bitmap(sfondi[menuESCI]);
					al_destroy_bitmap(sfondi[scelta]);
					al_destroy_bitmap(sfondi[due]);
					al_destroy_bitmap(sfondi[tre]);
					al_destroy_bitmap(sfondi[quattro]);
					return 4;			
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
      	{
			if(ev.keyboard.keycode== ALLEGRO_KEY_ESCAPE)
				done = false;
      	}

		//if di disegno
		if(redraw && al_is_event_queue_empty(event_queue)) 
		{	
		
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
			
			if(mouse_x1>326 && mouse_x1<536 && mouse_y1>238 && mouse_y1<268)
			{
				al_draw_bitmap(sfondi[due],0,0,0);
				al_flip_display();
				
			}
			else if(mouse_x1>328 && mouse_x1<540 && mouse_y1>314 && mouse_y1<344)
			{
				al_draw_bitmap(sfondi[tre],0,0,0);
				al_flip_display();
				
			}
			else if(mouse_x1>321 && mouse_x1<526 && mouse_y1>396 && mouse_y1<426)
			{
				al_draw_bitmap(sfondi[quattro],0,0,0);
				al_flip_display();
				
			}
			else
			{
				
				al_draw_bitmap(sfondi[scelta],0,0,0);
				al_flip_display();
			}
		}



	al_flip_display();
	}
	al_destroy_bitmap(sfondi[menubase]);
	al_destroy_bitmap(sfondi[menuNP]);
	al_destroy_bitmap(sfondi[menuESCI]);
	al_destroy_bitmap(sfondi[scelta]);
	al_destroy_bitmap(sfondi[due]);
	al_destroy_bitmap(sfondi[tre]);
	al_destroy_bitmap(sfondi[quattro]);
	
	return -1;


}

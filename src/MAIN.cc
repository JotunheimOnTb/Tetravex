/*
* 	Nome Gioco: TETRAVEX
*/
/**
*	@mainpage TETRAVEX
*	Il programma rappresenta un puzzle game numerico di nome Tetravex.
*	Lo scopo del gioco è quello di mettere le caselle numerate ai lati del quadrato, in alto in basso destra e sinistra,
*	in modo tale da non contrastare con le caselle adiacenti.
*	Per l'utilizzo di immagini e input da mouse abbiamo utilizzato le funzioni di libreria di Allegro 5.
*
* 	Autori:
*	@author Daniele Salerno
*
*	@file 
*	File contenente la funzione principale del gioco
*/

#include "HEADER_PRINCIPALE.h"
#include "HEADER_LOGICA_GIOCO.h"
#include "HEADER_MAIN.h"

/**
*Difficolta del gioco
*/
int DIFFICOLTA;
ALLEGRO_FONT *font = NULL;
extern ALLEGRO_EVENT_QUEUE* event_queue;
extern ALLEGRO_TIMER* timer;
extern ALLEGRO_DISPLAY* display;

unsigned int maschera = 0 ;//~0

/**
* La funzione "game loop" gestisce la partita vera e propria.
* Carica uno sfondo in base alla difficolta scelta, gestisce gli input/eventi del mouse(movimento e click), disegna le piastre, gestisce le 
* collisioni tra le caselle quando vengono spostate.
*/
void game_loop(Caselle **cas, PiastreBianche **pbd, PiastreBianche **pbs)
{
	
	ALLEGRO_BITMAP *sfondo_partita = NULL;
	int counter = 0;	/* se va a DIFFICOLTA*DIFFICOLTA la partita finisce*/
    int pos_x=0; 		/* pos x del mouse*/
    int pos_y=0;		/* pos y del mouse*/
	int sup_x = 0;		/* supporto per pos x del mouse*/
    int sup_y = 0;		/* supporto per pos y del mouse*/
	bool done = false;	/* se va a true si esce dal while principale*/
	bool mouse[2] = {false, false};/*tasto destro e sinistro del mouse*/

    bool redraw = true;
    al_start_timer(timer);

	if (DIFFICOLTA == 2)
	{
		sfondo_partita = al_load_bitmap("../images/s2x2.png");	
	}
	else if (DIFFICOLTA == 3)
	{
		sfondo_partita = al_load_bitmap("../images/s3x3.png");	
	}
	else
	{
		sfondo_partita = al_load_bitmap("../images/s4x4.png");	
	}

    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 
        if (event.type == ALLEGRO_EVENT_TIMER) { 		//evento costante det dal timer. Logica da gestire
			if (mouse[LEFT])
			{			
				sup_x = pos_x;
				sup_y = pos_y;
			}
			if (!mouse[LEFT])
			{	
				//Deselezione generale	
				for (int i = 0 ; i < DIFFICOLTA ; i++)
				{	
					for (int j = 0 ; j < DIFFICOLTA ; j++)
						cas[i][j].selezione = false;
				}
			}
			redraw = true;
        }
		else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)		//eventi mouse
		{
			if(event.mouse.button & 1){								//bottone sinistro mouse
				selezione(cas, pos_x, pos_y);
				mouse[LEFT] = true;
			}
		}
		else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)		//eventi mouse
		{
			if(event.mouse.button & 1)								//bottone sinistro mouse
				{
				collisione(cas,pbd,pbs,pos_x,pos_y);
				mouse[LEFT] = false;
				}
		}
		else if(event.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pos_x = event.mouse.x;
			pos_y = event.mouse.y;
		}
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {		//evento che gestisce la pressione di un tasto
        	if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {	//controllo su quale tasto è stato cliccato
                done = true;
            }
            //get_user_input();
        }
 		
		//nuovo if
        if (redraw && al_is_event_queue_empty(event_queue)) {	//evento che si alternerà con ALLEGRO EVENT TIME
			
			//ricolora il display
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
			al_draw_bitmap(sfondo_partita,0,0,0);
            
			if (maschera & LOG){
				al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/2, 20, ALLEGRO_ALIGN_CENTRE,"MOUSE x: %d", pos_x);//stampa testo con variabile
				al_draw_textf(font, al_map_rgb(255, 255, 255), WIDTH/2, 40, ALLEGRO_ALIGN_CENTRE,"MOUSE y: %d", pos_y);//stampa testo con variabile
			}
			//disegna le piastrelle bianche vuote dx & sx
			for (int i = 0 ; i < DIFFICOLTA ; i++)
			{	
				for (int j = 0 ; j < DIFFICOLTA ; j++) 
				{
					al_draw_rounded_rectangle(pbd[i][j].x,pbd[i][j].y,pbd[i][j].x+DIM_CASELLA,pbd[i][j].y+DIM_CASELLA,1,1,al_map_rgb(255, 255, 255),1);
					al_draw_rounded_rectangle(pbs[i][j].x,pbs[i][j].y,pbs[i][j].x+DIM_CASELLA,pbs[i][j].y+DIM_CASELLA,1,1,al_map_rgb(100, 255, 255),1);
				}
			}

			for (int i = 0 ; i < DIFFICOLTA ; i++)
			{
				for (int j = 0 ; j < DIFFICOLTA ; j++) 
				{
					if (!cas[i][j].selezione) al_draw_bitmap(cas[i][j].image, cas[i][j].x, cas[i][j].y, 0);//se collide ins coord collis,se non collide posiz orig
				}	
			}
			for (int i = 0 ; i < DIFFICOLTA ; i++)
			{
				for (int j = 0 ; j < DIFFICOLTA ; j++) 
				{
					if (cas[i][j].selezione) al_draw_bitmap(cas[i][j].image, sup_x, sup_y, 0);//segue il mouse
				}	
			}

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));

			if (counter == DIFFICOLTA*DIFFICOLTA)
			{
				al_show_native_message_box(al_get_current_display(), "TETRAVEX","Partita Finita", "complimenti!", 					"OK",ALLEGRO_MESSAGEBOX_OK_CANCEL);
				done = true;
			}
			
			counter = 0;
			for (int i = 0 ; i < DIFFICOLTA ; i++)
			{
				for (int j = 0 ; j < DIFFICOLTA ; j++) 
				{
					if (pbs[i][j].occ) 
						counter++;
				}
			}
        }
    }
}
/**
* La funzione shoutdown chiude gli oggeti creati usando ALLEGRO per una corretta chiusura del programma.
*/
void shutdown(Caselle **cas, PiastreBianche **pbd, PiastreBianche **pbs) //grafica
{
    if (timer)
        al_destroy_timer(timer);
 
    if (display)
        al_destroy_display(display);
 
    if (event_queue)
        al_destroy_event_queue(event_queue);

	for(int i = 0; i < DIFFICOLTA ; i++)
	{
		for(int j = 0; j < DIFFICOLTA ; j++)
		{
			al_destroy_bitmap(cas[i][j].image);
		}
	}


	for(int i = 0; i < DIFFICOLTA ; i++)
	{
		delete cas[i];
		delete pbd[i];
		delete pbs[i];
	}
	delete cas;
	delete pbd;
	delete pbs;
}
/**
*La funzione "new_game" all'inizio inizializza il gioco. Carica tutti le immagini dei numeri in una array e 
*crea effettivamente il campo da gioco: crea delle caselle random e poi le mescola. 
*Successivamente carica le immagini presente nelle array nelle caselle.
*/
void new_game(Caselle **cas)//logica_gioco
{
	DBM("INIZIO NEW GAME ",LOG);
	ALLEGRO_BITMAP *imm_num[40];
	imm_num[0] = al_load_bitmap("../images/1n.png");
	imm_num[1] = al_load_bitmap("../images/1e.png");
	imm_num[2] = al_load_bitmap("../images/1s.png");
	imm_num[3] = al_load_bitmap("../images/1o.png");

	imm_num[4] = al_load_bitmap("../images/2n.png");
	imm_num[5] = al_load_bitmap("../images/2e.png");
	imm_num[6] = al_load_bitmap("../images/2s.png");
	imm_num[7] = al_load_bitmap("../images/2o.png");

	imm_num[8] = al_load_bitmap("../images/3n.png");
	imm_num[9] = al_load_bitmap("../images/3e.png");
	imm_num[10] = al_load_bitmap("../images/3s.png");
	imm_num[11] = al_load_bitmap("../images/3o.png");

	imm_num[12] = al_load_bitmap("../images/4n.png");
	imm_num[13] = al_load_bitmap("../images/4e.png");
	imm_num[14] = al_load_bitmap("../images/4s.png");
	imm_num[15] = al_load_bitmap("../images/4o.png");

	imm_num[16] = al_load_bitmap("../images/5n.png");
	imm_num[17] = al_load_bitmap("../images/5e.png");
	imm_num[18] = al_load_bitmap("../images/5s.png");
	imm_num[19] = al_load_bitmap("../images/5o.png");

	imm_num[20] = al_load_bitmap("../images/6n.png");
	imm_num[21] = al_load_bitmap("../images/6e.png");
	imm_num[22] = al_load_bitmap("../images/6s.png");
	imm_num[23] = al_load_bitmap("../images/6o.png");
	
	imm_num[24] = al_load_bitmap("../images/7n.png");
	imm_num[25] = al_load_bitmap("../images/7e.png");
	imm_num[26] = al_load_bitmap("../images/7s.png");
	imm_num[27] = al_load_bitmap("../images/7o.png");

	imm_num[28] = al_load_bitmap("../images/8n.png");
	imm_num[29] = al_load_bitmap("../images/8e.png");
	imm_num[30] = al_load_bitmap("../images/8s.png");
	imm_num[31] = al_load_bitmap("../images/8o.png");

	imm_num[32] = al_load_bitmap("../images/9n.png");
	imm_num[33] = al_load_bitmap("../images/9e.png");
	imm_num[34] = al_load_bitmap("../images/9s.png");
	imm_num[35] = al_load_bitmap("../images/9o.png");

	imm_num[36] = al_load_bitmap("../images/0n.png");
	imm_num[37] = al_load_bitmap("../images/0e.png");
	imm_num[38] = al_load_bitmap("../images/0s.png");
	imm_num[39] = al_load_bitmap("../images/0o.png");

	struct neso{
		int n;
		int e;
		int s;
		int o;
	};
	//crezione matrice dinamica
	neso **c = new neso*[DIFFICOLTA];
	for (int i = 0 ; i < DIFFICOLTA ; i++){
		c[i] = new neso[DIFFICOLTA];		
	}
	srand (time(NULL));

	int counter=0;//INUTILE

	//creazione partita random	
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			if (i == 0 && j == 0)
			{	
				counter++;
				c[0][0].n = rand() % 10;
				c[0][0].e =	rand() % 10;
				c[0][0].s = rand() % 10;
				c[0][0].o =	rand() % 10;
			}
			else if (i==0)
			{	
				counter++;
				c[i][j].n = rand() % 10;
				c[i][j].e = rand() % 10;
				c[i][j].s = rand() % 10;
				c[i][j].o = c[i][j-1].e;	
			}
			else if (i > 0 && j==0)
			{
				counter++;
				c[i][j].n = c[i-1][j].s;
				c[i][j].e = rand() % 10;
				c[i][j].s = rand() % 10;
				c[i][j].o = rand() % 10;
			}
			else{
				counter++;
				c[i][j].n = c[i-1][j].s;
				c[i][j].e = rand() % 10;
				c[i][j].s = rand() % 10;
				c[i][j].o = c[i][j-1].e;	
			}			
		}
	}
	
	//Randomizziamo la partita
	int ri=0, rj=0, app=0;
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			ri = rand() % DIFFICOLTA;
			rj = rand() % DIFFICOLTA; 
			
			//nord
			app = c[i][j].n;
			c[i][j].n = c[ri][rj].n;
			c[ri][rj].n = app;

			//est
			app = c[i][j].e;
			c[i][j].e = c[ri][rj].e;
			c[ri][rj].e = app;
	
			//sud
			app = c[i][j].s;
			c[i][j].s = c[ri][rj].s;
			c[ri][rj].s = app;

			//ovest
			app = c[i][j].o;
			c[i][j].o = c[ri][rj].o;
			c[ri][rj].o = app;
			
		}
	}
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{	
			cas[i][j].image = al_create_bitmap(DIM_CASELLA, DIM_CASELLA); //errore qui
			al_set_target_bitmap(cas[i][j].image);
			//nord
			switch(c[i][j].n)
			{
				case 1:
					al_draw_bitmap(imm_num[0],0,0,0);
					cas[i][j].card[NORD] = 1;
					break;
				case 2:
					al_draw_bitmap(imm_num[4],0,0,0);
					cas[i][j].card[NORD] = 2;
					break;
				case 3:
					al_draw_bitmap(imm_num[8],0,0,0);
					cas[i][j].card[NORD] = 3;
					break;
				case 4:
					al_draw_bitmap(imm_num[12],0,0,0);
					cas[i][j].card[NORD] = 4;
					break;
				case 5:
					al_draw_bitmap(imm_num[16],0,0,0);
					cas[i][j].card[NORD] = 5;
					break;
				case 6:
					al_draw_bitmap(imm_num[20],0,0,0);
					cas[i][j].card[NORD] = 6;
					break;
				case 7:
					al_draw_bitmap(imm_num[24],0,0,0);
					cas[i][j].card[NORD] = 7;
					break;
				case 8:
					al_draw_bitmap(imm_num[28],0,0,0);
					cas[i][j].card[NORD] = 8;
					break;
				case 9:
					al_draw_bitmap(imm_num[32],0,0,0);
					cas[i][j].card[NORD] = 9;
					break;
				case 0:
					al_draw_bitmap(imm_num[36],0,0,0);
					cas[i][j].card[NORD] = 0;
					break;
			}
			//est
			switch(c[i][j].e)
			{
				case 1:
					al_draw_bitmap(imm_num[1],0,0,0);
					cas[i][j].card[EST] = 1;
					break;
				case 2:
					al_draw_bitmap(imm_num[5],0,0,0);
					cas[i][j].card[EST] = 2;
					break;
				case 3:
					al_draw_bitmap(imm_num[9],0,0,0);
					cas[i][j].card[EST] = 3;
					break;
				case 4:
					al_draw_bitmap(imm_num[13],0,0,0);
					cas[i][j].card[EST] = 4;
					break;
				case 5:
					al_draw_bitmap(imm_num[17],0,0,0);
					cas[i][j].card[EST] = 5;
					break;
				case 6:
					al_draw_bitmap(imm_num[21],0,0,0);
					cas[i][j].card[EST] = 6;
					break;
				case 7:
					al_draw_bitmap(imm_num[25],0,0,0);
					cas[i][j].card[EST] = 7;
					break;
				case 8:
					al_draw_bitmap(imm_num[29],0,0,0);
					cas[i][j].card[EST] = 8;
					break;
				case 9:
					al_draw_bitmap(imm_num[33],0,0,0);
					cas[i][j].card[EST] = 9;
					break;
				case 0:
					al_draw_bitmap(imm_num[37],0,0,0);
					cas[i][j].card[EST] = 0;
					break;
			}
			//sud
			switch(c[i][j].s)
			{
				case 1:
					al_draw_bitmap(imm_num[2],0,0,0);
					cas[i][j].card[SUD] = 1;
					break;
				case 2:
					al_draw_bitmap(imm_num[6],0,0,0);
					cas[i][j].card[SUD] = 2;
					break;
				case 3:
					al_draw_bitmap(imm_num[10],0,0,0);
					cas[i][j].card[SUD] = 3;
					break;
				case 4:
					al_draw_bitmap(imm_num[14],0,0,0);
					cas[i][j].card[SUD] = 4;
					break;
				case 5:
					al_draw_bitmap(imm_num[18],0,0,0);
					cas[i][j].card[SUD] = 5;
					break;
				case 6:
					al_draw_bitmap(imm_num[22],0,0,0);
					cas[i][j].card[SUD] = 6;
					break;
				case 7:
					al_draw_bitmap(imm_num[26],0,0,0);
					cas[i][j].card[SUD] = 7;
					break;
				case 8:
					al_draw_bitmap(imm_num[30],0,0,0);
					cas[i][j].card[SUD] = 8;
					break;
				case 9:
					al_draw_bitmap(imm_num[34],0,0,0);
					cas[i][j].card[SUD] = 9;
					break;
				case 0:
					al_draw_bitmap(imm_num[38],0,0,0);
					cas[i][j].card[SUD] = 0;
					break;
			}
			//ovest
			switch(c[i][j].o)
			{
				case 1:
					al_draw_bitmap(imm_num[3],0,0,0);
					cas[i][j].card[OVEST] = 1;
					break;
				case 2:
					al_draw_bitmap(imm_num[7],0,0,0);
					cas[i][j].card[OVEST] = 2;
					break;
				case 3:
					al_draw_bitmap(imm_num[11],0,0,0);
					cas[i][j].card[OVEST] = 3;
					break;
				case 4:
					al_draw_bitmap(imm_num[15],0,0,0);
					cas[i][j].card[OVEST] = 4;					
					break;
				case 5:
					al_draw_bitmap(imm_num[19],0,0,0);
					cas[i][j].card[OVEST] = 5;
					break;
				case 6:
					al_draw_bitmap(imm_num[23],0,0,0);
					cas[i][j].card[OVEST] = 6;
					break;
				case 7:
					al_draw_bitmap(imm_num[27],0,0,0);
					cas[i][j].card[OVEST] = 7;
					break;
				case 8:
					al_draw_bitmap(imm_num[31],0,0,0);
					cas[i][j].card[OVEST] = 8;
					break;
				case 9:
					al_draw_bitmap(imm_num[35],0,0,0);
					cas[i][j].card[OVEST] = 9;
					break;
				case 0:
					al_draw_bitmap(imm_num[39],0,0,0);
					cas[i][j].card[OVEST] = 0;
					break;
			}
		}
	}
	//liberazione memoria
	for (int i = 0 ; i < DIFFICOLTA ; i++){
		delete c[i];	
	}
	delete c;
	for (int i = 0 ; i < 40 ; i++)
	{
		al_destroy_bitmap(imm_num[i]);
	}
	DBM("FINE NEW GAME ",LOG);
}
/**
* La funzione init inizializza e installa le librerie di ALLEGRO che andremo ad utilizzare.
* Crea effettivamente le caselle base dove verranno inseriti i numeri e crea le piastre di destra e di sinistra.
*/
void init(Caselle **cas, PiastreBianche **pbd, PiastreBianche **pbs)
{
	if (!al_init())
	{
        DBM("failed to initialize allegro",ERROR);
		return;
	}
	
	if(!al_init_image_addon()) 
	{
        DBM("failed to initialize allegro!",ERROR);
		return;
    }
 
    if (!al_install_keyboard())
	{
        DBM("failed to install keyboard",ERROR);
		return;
 	}
    timer = al_create_timer(1.0 / FPS);
    if (!timer)
	{
        DBM("failed to create timer",ERROR);
		return;
	}
	if (!al_init_ttf_addon())
	{
        DBM("failed to install file ttf",ERROR);
		return;
 	}
	al_init_font_addon();
	if (!al_install_mouse())
	{
        DBM("failed to install mouse",ERROR);
		return;
 	}
	if (!al_init_primitives_addon())
	{
        DBM("failed to install al_init_primitives_addon",ERROR);
		return;
 	}
    event_queue = al_create_event_queue();
    if (!event_queue){
        DBM("failed to create event queue",LOG);
		return;
	}

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

	if (maschera & LOG)
		font = al_load_ttf_font("../fonts/pirulen.ttf",14,0 );

	new_game(cas);

	//inizializzazione piastre destra
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			pbd[i][j].x = pbd[0][0].x + (DIM_CASELLA*j);
			pbd[i][j].y = pbd[0][0].y + (DIM_CASELLA*i);

			if (i==0 && j==0)
			{
				pbd[0][0].x=WIDTH/3*2;
    			pbd[0][0].y=HEIGHT/6;
			}
		}
	}
	int id = 0; //ID stampato sulle caselle 

	//inizializzazione generale delle caselle e piastre
    for(int i = 0 ; i < DIFFICOLTA ; i++)
    {
		for (int j = 0 ; j < DIFFICOLTA ; j++){
			
			cas[i][j].id = id;
			cas[i][j].x = pbd[i][j].x;
			cas[i][j].y = pbd[i][j].y;
			cas[i][j].selezione = false;
	

			if (maschera & LOG)
			{
				al_set_target_bitmap(cas[i][j].image);
				al_draw_textf(font, al_map_rgb(10, 10, 10), DIM_CASELLA/2, DIM_CASELLA/2, ALLEGRO_ALIGN_CENTRE,"%d", id);
			}

			cas[i][j].destra = true;

			//inizializzazione piastrelle bianche dx
			pbd[i][j].id = id;
			pbd[i][j].id_casella = id;
			pbd[i][j].occ = true;

			//inizializzazione piastrelle bianche sx	
			pbs[i][j].id = id;
			pbs[i][j].id_casella = 17;//vuota
			pbs[i][j].occ = false;
			pbs[i][j].x = pbd[i][j].x - DIST_MATRICI;// distanza tra le due matrici
			pbs[i][j].y = pbd[i][j].y;
			pbs[i][j].card[NORD] = 17; //17 => null
			pbs[i][j].card[EST] = 17;
			pbs[i][j].card[SUD] = 17;
			pbs[i][j].card[OVEST] = 17;
	
			id++;
		}
    }

    al_set_target_bitmap(al_get_backbuffer(display));	//cambia il bersaglio di disegni futuri sul display
    al_flip_display();
}
/**
* Funzione principale
* esso chiama la funzione nuovo menu, che restituirà la DIFFICOLTA del gioco, dichiara le caselle e le piastre e alloca 
* le matrici dinamiche relative a esse,
* richiama init per inizializzare e game loop per far partire il gioco.
* Infine richiama la funzione shoutdown per una corretta chiusura del programma.
*/
int main(int argc, char *argv[])/**************MAIN**********************/
{	
	if (argc > 1 && *argv[1] == '1')
		maschera = 1;
	else if (argc > 1 && *argv[1] == '2')
		maschera = 2;
	else if (argc > 1 && *argv[1] == '3')
		maschera = 3;

	DBM("Abbiamo selezionato il livello di LOG",LOG);
	DBM("Abbiamo selezionato il livello di ERROR",ERROR);

	DIFFICOLTA = nuovomenu(); //
	if (DIFFICOLTA == -1) return 0; //error

	/*dichiarazioni caselle*/
	Caselle **cas = new Caselle*[DIFFICOLTA];
	PiastreBianche **pbs = new PiastreBianche*[DIFFICOLTA];
	PiastreBianche **pbd = new PiastreBianche*[DIFFICOLTA];

	/*allocazione matrici dinamiche*/
	for (int i = 0 ; i < DIFFICOLTA ; i++){
		cas[i] = new Caselle[DIFFICOLTA];
		pbd[i] = new PiastreBianche[DIFFICOLTA];
		pbs[i] = new PiastreBianche[DIFFICOLTA];		
	}

	init(cas,pbd,pbs);
	game_loop(cas,pbd,pbs);

	shutdown(cas,pbd,pbs);

	DBM("fine gioco e uscita pulita ",1);
	
	return 0;
}


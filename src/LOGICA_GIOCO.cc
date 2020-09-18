/**
*	
*	@mainpage TETRAVEX
* 	
*	@author Daniele Salerno
*	@author Salvatore Campanella
*
*	@file All'interno di logica gioco vengono fatti tutti i controlli delle posizioni,attuali e precedenti, e della 
* 	selezione delle caselle di gioco.
*/


#include "HEADER_PRINCIPALE.h"
#include "HEADER_LOGICA_GIOCO.h"


extern int DIFFICOLTA; 

/**
*maschera dei livelli
*/

extern unsigned int maschera;


/**
* In questa funzione viene effettuato un controllo. Se il mouse si trova sopra una particolare casella durante un determinato evento (click 
* del mouse) quella cartella sarà selezionabile 
*/
void selezione(Caselle **cas,int pos_x,int pos_y)
{	
	int id=0;
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j=0 ; j < DIFFICOLTA ; j++)
		{
			
		
			if(pos_x > cas[i][j].x && pos_x < cas[i][j].x+DIM_CASELLA && pos_y > cas[i][j].y && pos_y < cas[i][j].y+DIM_CASELLA)
			{
				DBM("stai cliccando sulla casella "<<id,LOG);
				cas[i][j].selezione = true;
			}
		id++;
		}																																																	
	}
}

/**
* Questa funziona serve per farci ritornare il valare ii e jj, quando una casella viene spostata da una posizione ad un altra vengono salvate le vecchie posizione dell'ultima casella selezionata. Il primo blocco controlla se sono state spostate caselle nella tabella di destra, il secondo blocco controlla se sono state spostate caselle di sinistra.
*/
void provenienza(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int old_x, int old_y, int &ii, int &jj)
{

/*
* blocco di destra
*/
	int id = 0;
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			if (old_x >= pbd[i][j].x && old_x < pbd[i][j].x + DIM_CASELLA && old_y >= pbd[i][j].y && old_y < pbd[i][j].y + DIM_CASELLA)
			{	
				DBM("provenienza dalla destra id= "<<id,LOG);
				ii = i;
				jj = j;
				return;
			}
			id++;
		}

	}
/*
* blocco di sinistra
*/
	id = 0;
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			if (old_x >= pbs[i][j].x && old_x < pbs[i][j].x + DIM_CASELLA && old_y >= pbs[i][j].y && old_y < pbs[i][j].y + DIM_CASELLA)
			{
				DBM("provenienza dalla sinistra id="<<id,LOG);
				ii = i;
				jj = j;
				return;
			}
			id++;
		}

	}

	DBM("ERRORE PROVENIENZA, NON DROVEBBERE MAI SUCCEDERE ",LOG);
	return ;		


}
/**
* La funzione ::controllo restituisce un booleano. Questo booleano serve alla funzione "::collisione" a vedere se è possibile 
* effettuare lo spostamento di una casella dalla piastra di destra alla piastra sinistra.
* Viene effettuato il controllo se i numeri all'interno delle caselle sono in contrasto con con le caselle adiacenti.
* Esempio: se il lato di destro della casella è diverso dal lato sinistro della casella adiacenti,
* oppure al suo fianco non ce una casella vuota.
*/

bool controllo(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int i, int j, int indice_i, int indice_j)
{
	DBM("INIZIO CONTROLLO ",LOG);
	DBM("ARGOMENTI PASSATI: i= "<<i<<",j= "<<j<<",indice_i = "<<indice_i<<"indice_j = "<<indice_j,LOG);
	DBM("cas["<<indice_i<<"]["<<indice_j<<"].id="<<cas[indice_i][indice_j].id,LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[NORD]=" << cas[indice_i][indice_j].card[NORD],LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[EST]=" << cas[indice_i][indice_j].card[EST],LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[SUD]=" << cas[indice_i][indice_j].card[SUD],LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[OVEST]=" << cas[indice_i][indice_j].card[OVEST],LOG);

	
	if (i==0 && j==0)								//angolo alto sinistra
	{
		DBM("angolo alto sinistra. Ci serve sapere:",LOG);
		DBM("pbs[i+1][j].card[NORD]  = " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i][j+1].card[OVEST]  = " << pbs[i][j+1].card[OVEST],LOG);

		if((cas[indice_i][indice_j].card[SUD]==pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD] == 17) && 			
		   (cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST] == 17))
			return true;
		else return false;
	}
	else if(i == 0 && j == DIFFICOLTA-1)			//angolo alto destra
	{	
		DBM("angolo alto destra. Ci serve sapere:",LOG);
		DBM("pbs[i+1][j].card[NORD] =" << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i][j-1].card[EST] =" << pbs[i][j - 1].card[EST],LOG);

		if((cas[indice_i][indice_j].card[SUD]==pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17) && 
		   (cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST] ==17))
			return true;
		else return false;
	}
	else if(i == DIFFICOLTA-1 && j == 0)			//angolo basso sinistra
	{
		DBM("angolo basso sinistra. Ci serve sapere:",LOG);
		DBM("pbs[i+1][j].card[SUD] =" << pbs[i-1][j].card[SUD],LOG);
		DBM("pbs[i+1][j].card[OVEST] =" << pbs[i][j+1].card[OVEST],LOG);

		if((cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17) && 
		   (cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17))
			return true;
		else return false;
	}
	else if(i == DIFFICOLTA-1 && j == DIFFICOLTA-1)	//angolo basso destra
	{	
		DBM("angolo basso destra. Ci serve sapere:",LOG);
		DBM("pbs[i-1][j].card[SUD] =" << pbs[i-1][j].card[SUD],LOG);
		DBM("pbs[i-1][j].card[EST] =" << pbs[i - 1][j].card[EST],LOG);

		if((cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17) &&  
		   (cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]== 17))
			return true;
		else return false;
	}
	else if(i == 0)									//restante prima riga
	{
		DBM("RESTANTE PRIMA RIGA. Ci serve sapere:",LOG);
		DBM("pbs[i+1][j].card[OVEST] =" << pbs[i + 1][j].card[OVEST],LOG);
		DBM("pbs[i-1][j].card[EST] =" << pbs[i - 1][j].card[EST],LOG);
		DBM("pbs[i+1][j].card[NORD] =" << pbs[i + 1][j].card[NORD],LOG);

		if(	(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST] ==17 ) &&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST] == 17) &&
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD] == 17))
			return true;
		else return false;
	}
	else if(i == DIFFICOLTA-1)						//restante ultima riga
	{	
		DBM("RESTANTE ULTIMA RIGA. Ci serve sapere:",LOG);
		DBM("pbs[i][j+1].card[OVEST] =" << pbs[i][j+1].card[OVEST],LOG);
		DBM("pbs[i][j-1].card[EST]  =" << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i-1][j].card[SUD]  =" << pbs[i - 1][j].card[SUD],LOG);

		if(	(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]==17) &&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17) &&
			(cas[indice_i][indice_j].card[NORD] == pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17 ))
			return true;
		else return false;
	}
	else if(j == 0)									//restante prima colonna
	{
		DBM("RESTANTE prima colonna. Ci serve sapere:",LOG);
		DBM("pbs[i][j+1].card[OVEST]=" << pbs[i][j + 1].card[OVEST],LOG);
		DBM("pbs[i+1][j].card[NORD] = "<<pbs[i+1][j].card[NORD],LOG);
		DBM("pbs[i-1][j].card[SUD]  = " << pbs[i - 1][j].card[SUD],LOG);

		if(	(cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17)&&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17)&&
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17))
			return true;
		else return false;
	}
	else if(j == DIFFICOLTA-1)						//restante ultima colonna
	{
		DBM("RESTANTE ultima colonna. Ci serve sapere:",LOG);
		DBM("pbs[i][j-1].card[EST] =" << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i+1][j].card[NORD]= " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i-1][j].card[SUD] =" <<pbs[i-1][j].card[SUD],LOG);

		if(	(cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17)&&
			(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]==17)&&
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17))
			return true;
		else return false;
	}
	else											//siamo in mezzo
	{
		DBM("siamo in mezzo. Ci serve sapere:",LOG);
		DBM("pbs[i][j-1].card[EST]  = " << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i+1][j].card[NORD] = " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i-1][j].card[SUD]  =" << pbs[i - 1][j].card[SUD],LOG);
		DBM("pbs[i][j+1].card[OVEST]= " << pbs[i][j + 1].card[OVEST],LOG);

		if(	(cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17)&&
			(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]==17)&& 
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17)&&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17))
			return true;
		else return false;
	}


}
/**
* La funzione "controllo2" verifica se è possibile invece effettuare il controllo di una casella dalla piastra sinistra alla piastra sinistra.
* Se lo spostamento non è possibile la funzione ::collisione riposiziona la casella nello stato precedente.
*/
bool controllo2(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int i, int j, int indice_i, int indice_j)
{
	DBM("INIZIO CONTROLLO2",LOG);
	DBM("ARGOMENTI PASSATI: i="<<i<<" j = "<<j<<" indice_i = "<<indice_i<<" indice_j = "<<indice_j,LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].id=" << cas[indice_i][indice_j].id,LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[NORD]=" << cas[indice_i][indice_j].card[NORD],LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[EST]=" << cas[indice_i][indice_j].card[EST],LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[SUD]=" << cas[indice_i][indice_j].card[SUD],LOG);
	DBM("cas[" << indice_i << "][" << indice_j << "].card[OVEST]=" << cas[indice_i][indice_j].card[OVEST],LOG)
	
	int n,e,s,o;
	int vi=0,vj=0;
	for (int k = 0 ; k < DIFFICOLTA ; k++)
	{	
		for(int l= 0 ; l < DIFFICOLTA ; l++)
		{
			if (pbs[k][l].id_casella == cas[indice_i][indice_j].id)
			{
				DBM("LA PIASTRA vecchia =>" << pbs[k][l].id,LOG);
				n = pbs[k][l].card[NORD];
				pbs[k][l].card[NORD] = 17;
				e = pbs[k][l].card[EST];
				pbs[k][l].card[EST] = 17;
				s = pbs[k][l].card[SUD];
				pbs[k][l].card[SUD] = 17;
				o =	pbs[k][l].card[OVEST];
				pbs[k][l].card[OVEST] = 17;
				DBM("n="<<n<<"e="<<e<<"s="<<s<<"o="<<o,LOG);
				vi = k;
				vj = l;
			}
		}
	}
	
	
	if (i==0 && j==0)								//angolo alto sinistra
	{
		DBM("angolo alto sinistra. Ci serve sapere:",LOG);
		DBM("pbs[i+1][j].card[NORD]  =" << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i][j+1].card[OVEST] =" << pbs[i][j + 1].card[OVEST],LOG);

		if((cas[indice_i][indice_j].card[SUD]==pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD] == 17) && 			
		   (cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST] == 17))
			return true;
	}
	else if(i == 0 && j == DIFFICOLTA-1)			//angolo alto destra
	{
		DBM("angolo alto destra. Ci serve sapere:",LOG)
		DBM("pbs[i+1][j].card[NORD] = " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i][j-1].card[EST]  =" << pbs[i][j - 1].card[EST],LOG);

		if((cas[indice_i][indice_j].card[SUD]==pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17) && 
		   (cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST] ==17))
			return true;
	}
	else if(i == DIFFICOLTA-1 && j == 0)			//angolo basso sinistra
	{
		DBM("angolo basso sinistra. Ci serve sapere:",LOG);
		DBM("pbs[i-1][j].card[SUD]   = " << pbs[i - 1][j].card[SUD],LOG);
		DBM("pbs[i][j+1].card[OVEST] = " << pbs[i][j + 1].card[OVEST],LOG);

		if((cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17) && 
		   (cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17))
			return true;
	}
	else if(i == DIFFICOLTA-1 && j == DIFFICOLTA-1)	//angolo basso destra
	{
		DBM("angolo basso destra. Ci serve sapere:",LOG);
		DBM("pbs[i-1][j].card[SUD] =" << pbs[i - 1][j].card[SUD],LOG);
		DBM("pbs[i][j-1].card[EST] = " << pbs[i][j - 1].card[EST],LOG);

		if((cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17) &&  
		   (cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]== 17))
			return true;
	}
	else if(i == 0)									//restante prima riga
	{
		DBM("RESTANTE PRIMA RIGA. Ci serve sapere:",LOG);
		DBM("pbs[i][j+1].card[OVEST]= " << pbs[i][j + 1].card[OVEST],LOG);
		DBM("pbs[i][j+1].card[OVEST]= " << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i][j-1].card[EST]  =" << pbs[i + 1][j].card[NORD],LOG);

		if(	(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST] ==17 ) &&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST] == 17) &&
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD] == 17))
			return true;
	}
	else if(i == DIFFICOLTA-1)						//restante ultima riga
	{
		DBM("RESTANTE ULTIMA RIGA. Ci serve sapere:",LOG);
		DBM("pbs[i][j+1].card[OVEST]=" << pbs[i][j + 1].card[OVEST],LOG);
		DBM("pbs[i][j-1].card[EST]  =" << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i-1][j].card[SUD]  =" << pbs[i - 1][j].card[SUD],LOG);

		if(	(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]==17) &&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17) &&
			(cas[indice_i][indice_j].card[NORD] == pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17 ))
			return true;
	}
	else if(j == 0)									//restante prima colonna
	{
		DBM("RESTANTE prima colonna. Ci serve sapere:",LOG);
		DBM("pbs[i][j+1].card[OVEST]= " << pbs[i][j + 1].card[OVEST],LOG);
		DBM("pbs[i+1][j].card[NORD] = " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i-1][j].card[SUD]  = " << pbs[i - 1][j].card[SUD],LOG);

		if(	(cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17)&&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17)&&
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17))
			return true;
	}
	else if(j == DIFFICOLTA-1)						//restante ultima colonna
	{
		DBM("RESTANTE ultima colonna. Ci serve sapere:",LOG);
		DBM("pbs[i][j-1].card[EST] = " << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i+1][j].card[NORD]= " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i-1][j].card[SUD] = " << pbs[i - 1][j].card[SUD],LOG);


		if(	(cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17)&&
			(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]==17)&&
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17))
			return true;

	}
	else											//siamo in mezzo
	{
		DBM("siamo in mezzo. Ci serve sapere:",LOG);
		DBM("pbs[i][j-1].card[EST]  = " << pbs[i][j - 1].card[EST],LOG);
		DBM("pbs[i+1][j].card[NORD] = " << pbs[i + 1][j].card[NORD],LOG);
		DBM("pbs[i-1][j].card[SUD]  = " << pbs[i - 1][j].card[SUD],LOG);
		DBM("pbs[i][j+1].card[OVEST]= " << pbs[i][j + 1].card[OVEST],LOG);

		if(	(cas[indice_i][indice_j].card[NORD]==pbs[i-1][j].card[SUD] || pbs[i-1][j].card[SUD]==17)&&
			(cas[indice_i][indice_j].card[OVEST]==pbs[i][j-1].card[EST] || pbs[i][j-1].card[EST]==17)&& 
			(cas[indice_i][indice_j].card[SUD] == pbs[i+1][j].card[NORD] || pbs[i+1][j].card[NORD]==17)&&
			(cas[indice_i][indice_j].card[EST]==pbs[i][j+1].card[OVEST] || pbs[i][j+1].card[OVEST]==17))
			return true;
	}

	pbs[vi][vj].card[NORD] = n;
	pbs[vi][vj].card[EST] = e;
	pbs[vi][vj].card[SUD] = s;
	pbs[vi][vj].card[OVEST] = o;
	return false;
}
/**
* La funzione collisione inzialmente controlla se è stata selezionata una casella.
* Sè è stata selezionata una casella, controlla fa un controllo se la casella collide con la piastra.
* In caso affermativo la case viene spostata e salvata nel rispetto slot.
*/
void collisione(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int pos_x,int pos_y)
{		
	int old_x = 0;
	int old_y = 0;

	int indice_i = 0;	//lavoreremo sulla casella con indice riga i
	int indice_j = 0;	//lavoreremo sulla casella con indice colonna j	

	int ii = 0,jj = 0;
	
	bool flag_sel = false;
	
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			if (cas[i][j].selezione)
			{
				DBM("LA CASELLA SELEZIONATA È CAS["<<i<<"]["<<j<<"]",LOG);
				indice_i = i;
				indice_j = j;
				old_x = cas[i][j].x;
				old_y = cas[i][j].y;
				flag_sel = true;
			}
		}
	}
	
	if (!flag_sel) return;
	
	//collisione matrice sinistra o a piastrelle bianche
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{	
		for (int j = 0 ; j < DIFFICOLTA ; j++)
		{
			if(pos_x > pbs[i][j].x && pos_x < pbs[i][j].x+DIM_CASELLA && pos_y > pbs[i][j].y && pos_y < pbs[i][j].y+DIM_CASELLA && !pbs[i][j].occ)
			{

				DBM("collisione con piastra sinistra " << pbs[i][j].id,LOG);
				/*
				* Caso di spostamento della casella da destra a sinistra
				*/
				if (cas[indice_i][indice_j].destra){
					
					if (!controllo(cas,pbd,pbs,i,j,indice_i, indice_j)) return;

					DBM("collisione con piastra sinistra " << pbs[i][j].id,LOG);
					DBM("da destra a sinistra",LOG);

					cas[indice_i][indice_j].x = pbs[i][j].x ;
					cas[indice_i][indice_j].y = pbs[i][j].y ;

					
					cas[indice_i][indice_j].destra = false;					
					pbs[i][j].occ = true;							
					pbs[i][j].id_casella = cas[indice_i][indice_j].id;

					provenienza(cas,pbd,pbs,old_x,old_y,ii,jj);
					pbd[ii][jj].occ = false;
					pbd[ii][jj].id_casella = 17;
					
					//Aggiornamento cardinalità piastre sinistra
					pbs[i][j].card[NORD] = cas[indice_i][indice_j].card[NORD];
					pbs[i][j].card[EST] = cas[indice_i][indice_j].card[EST];
					pbs[i][j].card[SUD] = cas[indice_i][indice_j].card[SUD];
					pbs[i][j].card[OVEST] = cas[indice_i][indice_j].card[OVEST];
				}
				else{
					if (!controllo2(cas,pbd,pbs,i,j,indice_i, indice_j)) return;
					/*
					* Caso di spostamento della casella da sinistra a sinistra
					*/
					DBM("collisione con piastra sinistra",LOG);
					DBM("da sinistra a sinistra",LOG);

					cas[indice_i][indice_j].x = pbs[i][j].x ;
					cas[indice_i][indice_j].y = pbs[i][j].y ;

					pbs[i][j].occ = true;
					pbs[i][j].id_casella = cas[indice_i][indice_j].id;
					
					provenienza(cas,pbd,pbs,old_x,old_y,ii,jj);
					pbs[ii][jj].occ = false;

					//Aggiornamento cardinalità piastre sinistra vecchia
					pbs[ii][jj].card[NORD] = 17;
					pbs[ii][jj].card[EST] = 17;
					pbs[ii][jj].card[SUD] = 17;
					pbs[ii][jj].card[OVEST] = 17;

					pbs[ii][jj].id_casella = 17;

					//Aggiornamento cardinalità piastre sinistra nuova
					pbs[i][j].card[NORD] = cas[indice_i][indice_j].card[NORD];
					pbs[i][j].card[EST] = cas[indice_i][indice_j].card[EST];
					pbs[i][j].card[SUD] = cas[indice_i][indice_j].card[SUD];
					pbs[i][j].card[OVEST] = cas[indice_i][indice_j].card[OVEST];
					
			
				}
				return ;
			}
		}
	}

	//collisione con matrice destra o matrice iniziale
	for (int i = 0 ; i < DIFFICOLTA ; i++)
	{	
		for (int j = 0  ; j < DIFFICOLTA ; j++)	
		{
		
			if(pos_x > pbd[i][j].x && pos_x < pbd[i][j].x + DIM_CASELLA && pos_y > pbd[i][j].y && pos_y < pbd[i][j].y + DIM_CASELLA && !pbd[i][j].occ)
			{
				DBM("collisione con piastra destra" << pbd[i][j].id,LOG);

				cas[indice_i][indice_j].x = pbd[i][j].x ;
				cas[indice_i][indice_j].y = pbd[i][j].y ;

				if (!cas[indice_i][indice_j].destra){
					/*
					* Caso di spostamento della casella da sinistra a destra
					*/
					DBM("da sinistra a destra",LOG);

					cas[indice_i][indice_j].destra = true;
					pbd[i][j].occ = true;
					pbd[i][j].id_casella = cas[indice_i][indice_j].id;

					provenienza(cas,pbd,pbs,old_x,old_y,ii,jj);
					pbs[ii][jj].occ = false; 
					pbs[ii][jj].id_casella = 17;

					//aggionamento piastre sinistra vecchia
					pbs[ii][jj].card[NORD] = 17;
					pbs[ii][jj].card[EST] = 17;
					pbs[ii][jj].card[SUD] = 17;
					pbs[ii][jj].card[OVEST] = 17;
				}
				else{
					/*
					* Caso di spostamento della casella da destra a destra
					*/
					DBM("da destra a destra",LOG);

					provenienza(cas,pbd,pbs,old_x,old_y,ii,jj);
					pbd[ii][jj].occ = false; 
					pbd[ii][jj].id_casella = 17;
				
					pbd[i][j].occ = true;
					pbd[i][j].id_casella = cas[indice_i][indice_j].id;
				
				}		
				return ;
			}
		}
	}
}


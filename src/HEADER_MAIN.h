#include <cstdlib> 	//rand
#include <ctime>   	//time

//prototipi
/**
*Prototipi delle funzioni presenti nei vai file.
*/
void abort_game(const char* message);
void selezione(Caselle **cas, int pos_x, int pos_y);
void collisione(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int pos_x,int pos_y);
bool controllo2(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int i, int j, int indice_i, int indice_j);
bool controllo(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int i, int j, int indice_i, int indice_j);//const cas
void provenienza(Caselle **cas,PiastreBianche **pbd, PiastreBianche **pbs,int old_x, int old_y, int &ii, int &jj);//const cas
int nuovomenu();






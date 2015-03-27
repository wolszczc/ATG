#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "atg.h"

n_gram stworz_n_gram(n_gram *gram, n_gram *gram2, int ile_slow, int nr_n_gramu, int los){
	int i;
	char slowo_tym[1024];
	int l_liter_prefiks; 
        int l_liter_sufiks;

	gram->prefiks =(char**) malloc (ile_slow * sizeof(gram->prefiks));
	if(gram->prefiks == NULL){
                fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                exit(EXIT_FAILURE);
        }
	for(i = 0; i<ile_slow;i++){
		l_liter_prefiks = strlen(gram2[los].prefiks[i]);
		*gram = stworz_prefiks( gram, l_liter_prefiks, i);
		strcpy(gram->prefiks[i],gram2[los].prefiks[i]);				
	/*	printf("%s ",gram[nr_n_gramu].prefiks[i]);*/
	}
	if(gram2[los].zliczenia_prefiks != 1){
/*		slowo_tym = wyszukaj(drzewo, gram[a]);
*/		l_liter_sufiks = strlen(slowo_tym);
		*gram = stworz_sufiks( gram,l_liter_sufiks);
		strcpy(gram->sufiks, slowo_tym);		
	}
	else{
		l_liter_sufiks = strlen(gram2[los].sufiks);
       		*gram = stworz_sufiks( gram,l_liter_sufiks);
	      	strcpy(gram->sufiks,gram2[los].sufiks);
	 
	}

	return *gram;
}
/*
n_gram stworz_n_gram(n_gram *gram, atg_t *atg, int ile_slow, int nr_n_gramu, int los){
	int i;
	int a;
	int l_liter_prefiks; 
	int l_liter_sufiks;

	gram->prefiks =(char**) malloc (ile_slow * sizeof(gram->prefiks));
	if(gram->prefiks == NULL){
		fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                exit(EXIT_FAILURE);
	}

	for(i = 0; i<ile_slow;i++){
		a = (los + i - ile_slow);
		l_liter_prefiks = strlen(atg[a].slowo);
		gram[nr_n_gramu] = stworz_prefiks( gram, l_liter_prefiks, i);
		strcpy(gram[nr_n_gramu].prefiks[i],atg[a].slowo);				
	}
	l_liter_sufiks = strlen(atg[los].slowo);		
	gram[nr_n_gramu] = stworz_sufiks( gram,l_liter_sufiks);
	strcpy(gram[nr_n_gramu].sufiks,atg[los].slowo);
	
	return *gram;

}
*/
n_gram stworz_dane_o_n_gramie(n_gram *gram, atg_t *atg, int ile_slow, int nr_slowa ){
	int i;
	int j = 0;
	int l_liter_prefiks;
        int l_liter_sufiks;

	gram->prefiks =(char**) malloc (ile_slow * sizeof(gram->prefiks));
	if(gram->prefiks == NULL){
                fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                exit(EXIT_FAILURE);
	}
	for(i = 0; i < ile_slow; i++){
		l_liter_prefiks = strlen(atg[nr_slowa + i].slowo);
       	        *gram = stworz_prefiks( gram, l_liter_prefiks, i);
       	        strcpy(gram->prefiks[i], atg[nr_slowa + i].slowo);

	/*	printf("%s ",gram[nr_slowa].prefiks[i]);*/
	}
	/*nie zmieniaj wartości i! */
	l_liter_sufiks = strlen(atg[nr_slowa+i].slowo);
        *gram = stworz_sufiks( gram,l_liter_sufiks);
        strcpy(gram->sufiks,atg[nr_slowa + i].slowo);

	gram->zliczenia_prefiks = 1;
	gram->zliczenia_sufiks = 1;
	gram->index = nr_slowa;
/*	gram->wielkosc_tab_wsk_sufiksow = 2;*/
	gram->wsk_na_sufiks = NULL;

	return *gram;
}

n_gram oblicz_prawdopodobienstwo(n_gram *gram, int l_n_gramow){
	double prefiks = gram->zliczenia_prefiks;
	double sufiks = gram->zliczenia_sufiks;
	double l_n_g = l_n_gramow;
	gram->p_wystapienia_prefiksu = prefiks / l_n_g;
	gram->p_wystapienia_sufiksu = sufiks/l_n_g;
	gram->p_wystapienia_n_gramu = (gram->p_wystapienia_prefiksu * gram->p_wystapienia_sufiksu);
	return *gram;
}
/*
void realloc_wsk_na_sufiks(n_gram *gram, int size){
	gram->wsk_na_sufiks = (int*) realloc(gram->wsk_na_sufiks, 2 * size * sizeof *gram->wsk_na_sufiks);
        if (gram->wsk_na_sufiks == NULL){
                fprintf(stderr,"Błąd przy realokacji (brak pamięci!)\n");
                exit(EXIT_FAILURE);
        }
}
*/
/*
void kopiuj(int *tab_1, int *tab_2, int liczba_elementow){
	int i;
	for(i = 0; i< liczba_elementow; i++)
		tab_1[i] = tab_2[i]; 
}
*/
n_gram stworz_prefiks(n_gram *gram, int l_liter_prefiks, int nr_slowa){
	if(gram != NULL){
		gram->prefiks[nr_slowa] =(char*) malloc(l_liter_prefiks * sizeof(gram->prefiks));
		if(gram->prefiks[nr_slowa] == NULL){
                	fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                	exit(EXIT_FAILURE);
		}
		return *gram;
	}
	else{
                fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                exit(EXIT_FAILURE);
        }

}
n_gram stworz_sufiks(n_gram *gram, int l_liter_sufiks){
	if(gram != NULL){
		gram->sufiks =(char*) malloc(l_liter_sufiks * sizeof(gram->sufiks));
		if(gram->sufiks == NULL){
                	fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                	exit(EXIT_FAILURE);
		}
		return *gram;
	}
	else{
                fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                exit(EXIT_FAILURE);
        }
}

void zwolnij_n_gram(n_gram *gram, int wielkosc_tab_strukt, int rzad_n_gramu){
	int i,j;
	for(j = wielkosc_tab_strukt - 1; j>0; j--){
		for(i = rzad_n_gramu - 1; i>=0; i--)
			free(gram[j].prefiks[i]);
		free(gram[j].sufiks);
		free(gram[j].prefiks);
	}
	free(gram);
}

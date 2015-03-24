#include "atg.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

atg_t *czytaj_slowa(FILE *in, int *l_slow, int *start_size, atg_t *atg){
	
	int dlugosc_slowa_tym;
	int i;
	char slowo_tym[100]; /* słowo tymczasowe- przechwouje słowa z wejścia*/

	i = *l_slow;
	if( atg == NULL){
		fprintf(stderr, "Brak danych\n");
		exit(EXIT_FAILURE);
	}

	if (in != NULL){
		while(fscanf(in,"%s",slowo_tym) != EOF){
			dlugosc_slowa_tym = strlen(slowo_tym);
			if(dlugosc_slowa_tym > 100){
				fprintf(stderr, "Zbyt długie słowa (dopuszczalna długość słowa = 100)\n");
				exit(EXIT_FAILURE);
			}
			if(i >= *start_size - 10){
				atg = realloc_tab_slow(atg,*start_size);
				*start_size *= 2;
			}

			atg[i] = stworz_slowo(atg, dlugosc_slowa_tym);
			atg[i].l_liter = dlugosc_slowa_tym;
			strcpy(atg[i].slowo,slowo_tym);
			atg[i].index = (i + 1);
			/*************sprawdzenie tablicy słów***********/
			/*printf("%s %d %d\n",atg[i].slowo, atg[i].l_liter,atg[i].index);*/	
			/*************sprawdzenie***********/
			i++;
			slowo_tym[0] = '\0';	
			printf("%s\n\n",atg[0].slowo);
		}
		*l_slow = i - 1;
		
	}else{
		exit(EXIT_FAILURE);
	}
	
	return atg;	
}

void zapisz_slowa(atg_t *atg, FILE *in, int *l_slow){
	int i;
	int j;
	for(i = 0; i<= *l_slow; i++){
		fprintf(in,"%s ", atg[i].slowo);
/*		if(j == 10){
			fprintf(in,"\n");
			j = 0;
		}
	*/	j++;	
	}
}

atg_t stworz_slowo(atg_t *atg, int liczba_liter){

	if(atg != NULL){
		atg->slowo = malloc(sizeof(atg->slowo) * liczba_liter);
		return *atg;
	}
	else{
		fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
		exit(EXIT_FAILURE);
	}
}


atg_t *realloc_tab_slow(atg_t *atg, int size){
	atg_t *atg_r = (atg_t*) realloc(atg, 2 * size * sizeof *atg);
	if (atg_r != NULL){
		return atg_r;	
	}else{
		fprintf(stderr,"Błąd przy realokacji (brak pamięci!)\n");
		return NULL;;
	}
}

void zwolnij_pamiec(atg_t *atg, int l_slow){
	int i;
	for(i = l_slow ; i > 0; i--){
		free(atg[i].slowo);
	}
	free(atg);
}



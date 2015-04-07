#include "atg.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

atg_t *czytaj_slowa(FILE *in, int *l_slow, int *start_size, atg_t *atg){
	
	int dlugosc_slowa_tym;
	int i;
	char slowo_tym[1024]; /* słowo tymczasowe- przechwouje słowa z wejścia*/

	i = *l_slow;
	if( atg == NULL){
		fprintf(stderr, "Brak danych\n");
		exit(EXIT_FAILURE);
	}

	if (in != NULL){
		while(fscanf(in,"%s", slowo_tym) != EOF){
			dlugosc_slowa_tym = strlen(slowo_tym);
			if(dlugosc_slowa_tym >= 1024){
				fprintf(stderr, "Zbyt długie słowa (dopuszczalna długość słowa = 1024)\n");
				exit(EXIT_FAILURE);
			}
			if(i >= *start_size - 10){
				atg = realloc_tab_slow(atg, *start_size);
				*start_size *= 2;
			}

			atg[i] = stworz_slowo(atg, dlugosc_slowa_tym);
			atg[i].l_liter = dlugosc_slowa_tym;
			strcpy(atg[i].slowo, slowo_tym);
			atg[i].index = (i);
			/*************sprawdzenie tablicy słów***********/
			/*printf("%s %d %d\n",atg[i].slowo, atg[i].l_liter,atg[i].index);*/	
			/*************sprawdzenie***********/
			i++;
			slowo_tym[0] = '\0';	
			/*printf("%s\n\n",atg[0].slowo);*/
		}
		*l_slow = i - 1;
		
	}else{
		exit(EXIT_FAILURE);
	}
	
	return atg;	
}


n_gram *czytaj_slowa_posrednie(FILE *in, int l_n_gramow, int *start_size, n_gram *gram, int inn){
	
	int dlugosc_slowa_tym;
	int i = 0;
	int j,j2;
	int a;
	char slowo_tym[1024]; /* słowo tymczasowe- przechwouje słowa z wejścia*/

	if( gram == NULL){
		fprintf(stderr, "Brak danych\n");
		exit(EXIT_FAILURE);
	}

	if (in != NULL){
		for(j = 1; j<l_n_gramow; j++){
                        gram[j].prefiks =(char**) malloc (inn * sizeof(gram->prefiks));
                        if(gram[j].prefiks == NULL){
				fprintf(stderr,"Błąd przy alokacji (brak pamięci!)\n");
                                exit(EXIT_FAILURE);
                        }
			for(j2 = 0; j2 < inn; j2++){
				fscanf(in,"%s", slowo_tym);
				dlugosc_slowa_tym = strlen(slowo_tym);
				if(dlugosc_slowa_tym >= 1024){
                	                fprintf(stderr, "Zbyt długie słowa (dopuszczalna długość słowa = 1024)\n");
        	                        exit(EXIT_FAILURE);
	                        }
				gram[j] = stworz_prefiks( &gram[j], dlugosc_slowa_tym, j2);	
				strcpy(gram[j].prefiks[j2], slowo_tym);
				slowo_tym[0] = '\0';
			}
		}
		for(j = 1; j<l_n_gramow; j++){
			fscanf(in,"%s", slowo_tym);
			dlugosc_slowa_tym = strlen(slowo_tym);
                        if(dlugosc_slowa_tym >= 1024){
        	                fprintf(stderr, "Zbyt długie słowa (dopuszczalna długość słowa = 1024)\n");
	                        exit(EXIT_FAILURE);
                        }
			gram[j] = stworz_sufiks(&gram[j],dlugosc_slowa_tym);
			strcpy(gram[j].sufiks, slowo_tym);
			slowo_tym[0] = '\0';
		}
		for(j = 1; j<l_n_gramow; j++){
			fscanf(in,"%d", &dlugosc_slowa_tym);
			gram[j].zliczenia_prefiks = dlugosc_slowa_tym;	
			gram[j].index = j;
			gram[j].wsk_na_sufiks = NULL;
			gram[j].wsk_na_sufiks =(int*) malloc(gram[j].zliczenia_prefiks *sizeof(int));
		}
		for(j = 1; j<l_n_gramow; j++){
                        fscanf(in,"%d", &dlugosc_slowa_tym);
                        gram[j].zliczenia_sufiks = dlugosc_slowa_tym;
                }
		for(j = 1; j<l_n_gramow; j++){
			if(gram[j].zliczenia_prefiks > 1){
				for(j2 = 0;j2<gram[j].zliczenia_prefiks; j2++){
					fscanf(in,"%d",&a);
					gram[j].wsk_na_sufiks[j2] = a;
				}
			}
		}

	}else{
		exit(EXIT_FAILURE);
	}
	
	return gram;	
}

void zapisz_slowa(atg_t *atg, FILE *in, int *l_slow){
	int i;
	int j;
	for(i = 1; i< *l_slow; i++){
		fprintf(in,"%s ", atg[i].slowo);
		if((i%10) == 8)
			fprintf(in,"\n");
/*		if(j == 10){
			fprintf(in,"\n");
			j = 0;
		}
	*/	j++;	
	}
}

void zapisz_prawdopodobienstow_n_gram(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie){
	int i;
	int j;
	for(i = 1;i<liczba_n_gramow;i++){
		for(j = 0; j<liczba_slow_w_prefiksie;j++)
			fprintf(in,"%s ",gram[i].prefiks[j]);
		fprintf(in," %.10f\t %.10f\n %s  %.10f\n\n",gram[i].p_wystapienia_prefiksu, gram[i].p_wystapienia_n_gramu , gram[i].sufiks, gram[i].p_wystapienia_sufiksu);
	}

}

void zapisz_dane_statystyczne_n_gram(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie){
	int i;
        int j;
        for(i = 1;i<liczba_n_gramow;i++){
                for(j = 0; j<liczba_slow_w_prefiksie;j++)
                        fprintf(in,"%s ",gram[i].prefiks[j]);
                fprintf(in," %d\n %s  %d\n\n",gram[i].zliczenia_prefiks, gram[i].sufiks, gram[i].zliczenia_sufiks);
        }
}

void zapisz_dane_plik_posredni(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie){
	int i;
        int j;
        for(i = 1;i<liczba_n_gramow;i++){
                for(j = 0; j<liczba_slow_w_prefiksie;j++)
                        fprintf(in,"%s ",gram[i].prefiks[j]);
                fprintf(in,"\n");
	}	
	for(i = 1;i<liczba_n_gramow;i++)
                fprintf(in,"%s ",gram[i].sufiks);
	fprintf(in,"\n");
        for(i = 1;i<liczba_n_gramow;i++)
                fprintf(in,"%d ",gram[i].zliczenia_prefiks);
	fprintf(in,"\n");
	for(i = 1;i<liczba_n_gramow;i++)
                fprintf(in,"%d ",gram[i].zliczenia_sufiks);
	fprintf(in,"\n");
	for(i = 1;i<liczba_n_gramow;i++){
		if(gram[i].zliczenia_prefiks > 1)
			for(j = 0;j<gram[i].zliczenia_prefiks;j++)
				fprintf(in,"%d ",gram[i].wsk_na_sufiks[j]);	
	}
		
}

/*
void zapisz_dane_posrednie(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie){
	int i;
        int j;
        for(i = 1;i<liczba_n_gramow;i++){
                for(j = 0; j<liczba_slow_w_prefiksie;j++)
                        fwrite(gram[i].prefiks[j], sizeof(gram[i].prefiks[j][0]), liczba_n_gramow, in);
                fwrite(in," %d\n %s  %d\n\n",gram[i].zliczenia_prefiks, gram[i].sufiks, gram[i].zliczenia_sufiks);
        }
}
*/
void zapisz_wylosowane_n_gramy(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie){
	int i;
        int j;
        for(i = 1;i<liczba_n_gramow;i++){
                for(j = 0; j<liczba_slow_w_prefiksie;j++)
                        fprintf(in,"%s ",gram[i].prefiks[j]);
                fprintf(in,"\n%s\n\n",gram[i].sufiks);
        }
}

void zapisz_przegenerowany_tekst(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie){
	int i;
	int j = 0;
	for(i = 1; i<liczba_n_gramow; i++){
		for(j; j<liczba_slow_w_prefiksie; j++){
			fprintf(in,"%s ",gram[i].prefiks[j]);
		}
		j = liczba_slow_w_prefiksie;
		fprintf(in,"%s ",gram[i].sufiks);
		if((i%10) == 8)
			fprintf(in,"\n");
	}
}

void zapisz_konkretna_liczbe_slow(n_gram *gram, FILE *in, int liczba_slow, int liczba_slow_w_prefiksie){
	int i;
	int j;
	int k;
	for(i = 1; i < liczba_slow ; i++){
		for(j;j<liczba_slow_w_prefiksie; j++){
			fprintf(in,"%s ",gram[i].prefiks[j]);
			k++;
                }
                j = liczba_slow_w_prefiksie;
                fprintf(in,"%s ",gram[i].sufiks);
		k++;
		if(k == liczba_slow)
			break;
	}
}


void zapisz_konkretna_liczbe_akapitow(n_gram *gram, FILE *in, int liczba_akapitow, int liczba_slow_w_prefiksie){
	int i;
        int j;
/*	int k;
*/	for(i = 1; i < liczba_akapitow ; i++){
                for(j;j<liczba_slow_w_prefiksie; j++){
                        fprintf(in,"%s ",gram[i].prefiks[j]);
/*			k++;
*/		}
                j = liczba_slow_w_prefiksie;
                fprintf(in,"%s ",gram[i].sufiks);
/*		k++;
                if(k == 100){
                        fprintf(in,"\n");
			k = 0;
		}
*/	}


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
	for(i = l_slow - 1; i > 0; i--){
		free(atg[i].slowo);
	}
	free(atg);
}



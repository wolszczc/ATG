#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atg.h"


drzewo_t wstaw_do_drzewa(drzewo_t t, n_gram *gram, int _rzad_n_gramu,  int cmp(const void *, const void *, int rzad_n_gramu)){
	int i;
	static int j;
	static int tab[1024];
        if(t == NULL){
                wezel_t *wezel = malloc(sizeof *wezel);
                wezel->gram_drzewo = gram;
                wezel->lewy = wezel->prawy = NULL;
                return wezel;
        }
        else if( cmp( t->gram_drzewo, gram, _rzad_n_gramu) > 0 ){
                t->lewy = wstaw_do_drzewa( t->lewy, gram, _rzad_n_gramu ,cmp);
                return t;
        }
        else if( cmp( t->gram_drzewo, gram, _rzad_n_gramu) < 0){
                t->prawy = wstaw_do_drzewa( t->prawy, gram,_rzad_n_gramu ,cmp);
                return t;
        }
        else{
		t->gram_drzewo->zliczenia_prefiks++;
		gram->zliczenia_prefiks = t->gram_drzewo->zliczenia_prefiks;
		if(strcmp( t->gram_drzewo->sufiks, gram->sufiks) == 0){
			t->gram_drzewo->zliczenia_sufiks++;
			gram->zliczenia_sufiks = t->gram_drzewo->zliczenia_sufiks;
		}

/*		realloc_wsk_na_sufiks(t->gram_drzewo, t->gram_drzewo->zliczenia_prefiks);
*/

/*		tab[j] = t->gram_drzewo->wsk_na_sufiks[j] = t->gram_drzewo->index;
		tab[j+1] = t->gram_drzewo->wsk_na_sufiks[j+1] = gram->index;
*/	
/*		realloc_wsk_na_sufiks(gram, gram->zliczenia_prefiks);*/

/*		gram->wsk_na_sufiks[j] = t->gram_drzewo->index;
		gram->wsk_na_sufiks[j+1] = gram->index;
*/	
			
/*		kopiuj(t->gram_drzewo->wsk_na_sufiks, gram->wsk_na_sufiks, gram->zliczenia_prefiks);
*/		
/*		kopiuj( gram->wsk_na_sufiks, tab, j);
		kopiuj( t->gram_drzewo->wsk_na_sufiks, tab, j);
*/
/*		printf("%d %d %d\n",t->gram_drzewo->zliczenia_prefiks,gram->zliczenia_prefiks,j);
		j = j+1;
*/
/*		for(i = 0; i<gram->zliczenia_sufiks; i++)
			printf("%d  %d aaa\n",gram->wsk_na_sufiks[i] ,t->gram_drzewo->index);
		for(i = 0; i<t->gram_drzewo->zliczenia_sufiks; i++)
			printf("%d  %d bbb\n",t->gram_drzewo->wsk_na_sufiks[i] ,gram->index);

		printf("\n");
*/
	
		t->srodek = wstaw_do_drzewa( t->srodek, gram, _rzad_n_gramu ,cmp); 
/*		j = 0;
		tab[0] = '\n';
*/		return t;
        }

}

void szukaj_n_gramow(drzewo_t t, n_gram *gram, int _rzad_n_gramu,  int cmp(const void *, const void *, int rzad_n_gramu)){
	if(t != NULL){
		static int i;
		if( cmp( t->gram_drzewo, gram, _rzad_n_gramu) > 0 ){
     	        	szukaj_n_gramow( t->lewy, gram, _rzad_n_gramu ,cmp);
       	 	}
        	else if( cmp( t->gram_drzewo, gram, _rzad_n_gramu) < 0){
                	szukaj_n_gramow( t->prawy, gram,_rzad_n_gramu ,cmp);
        	}
        	else{
			if(t->gram_drzewo->wsk_na_sufiks == NULL)
				t->gram_drzewo->wsk_na_sufiks = (int*) malloc((t->gram_drzewo->zliczenia_prefiks )* sizeof * t->gram_drzewo->wsk_na_sufiks);
			gram->wsk_na_sufiks[i] = t->gram_drzewo->index;
			i++;	

			szukaj_n_gramow( t->srodek, gram, _rzad_n_gramu ,cmp);

		}

		i = 0;
	}
}


int cmp(const void *a, const void *b, int rzad_n_gramu){
	int i;
        n_gram *aa = (n_gram*)a;
        n_gram *bb = (n_gram*)b;
	for(i = 0;i < rzad_n_gramu; i++){
		if(strcmp(aa->prefiks[i], bb->prefiks[i]) < 0)
                	return -1;
        	else if(strcmp(aa->prefiks[i], bb->prefiks[i]) > 0)
                	return 1;
	}
	return 0;
}


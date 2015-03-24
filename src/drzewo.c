#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atg.h"


drzewo_t wstaw_do_drzewa(drzewo_t t, n_gram *gram, int _rzad_n_gramu,  int cmp(const void *, const void *, int rzad_n_gramu)){
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
                return t;
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


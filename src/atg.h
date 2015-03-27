#ifndef _AGT_H_
#define _ATG_H_
#include <stdio.h>

typedef struct _atg_t{
	char *slowo;
	int index;	
	int zliczenia;
	int l_liter;
}atg_t;

typedef struct _n_gram{
	char **prefiks;
	char *sufiks;
	int index;
	int zliczenia_prefiks;
	int zliczenia_sufiks;
	int *wsk_na_sufiks;
/*	int wielkosc_tab_wsk_sufiksow;*/
	double p_wystapienia_prefiksu;
	double p_wystapienia_sufiksu;
	double p_wystapienia_n_gramu;
	double pmi;
}n_gram;

typedef struct _drzewo_t{
	n_gram *gram_drzewo;
	struct _drzewo_t *lewy;
	struct _drzewo_t *prawy;
	struct _drzewo_t *srodek;
}wezel_t, *drzewo_t;

/*czytnik*/
atg_t *czytaj_slowa(FILE *in, int * l_slow, int * start_size, atg_t *atg);

void zapisz_slowa(atg_t *atg, FILE *in, int *l_slow);

void zapisz_prawdopodobienstow_n_gram(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie);

void zapisz_dane_statystyczne_n_gram(n_gram *gram, FILE *in, int liczba_n_gramow, int liczba_slow_w_prefiksie);

atg_t stworz_slowo(atg_t *agt, int liczba_liter);

atg_t *realloc_tab_slow(atg_t *atg, int size);

void zwolnij_pamiec(atg_t *atg, int l_slow);

/*drzewo*/
drzewo_t wstaw_do_drzewa(drzewo_t t, n_gram *gram, int _rzad_n_gramu,  int cmp(const void *, const void *, int rzad_n_gramu));

void szukaj_n_gramow(drzewo_t t, n_gram *gram, int _rzad_n_gramu,  int cmp(const void *, const void *, int rzad_n_gramu));

int cmp(const void *a, const void *b, int rzad_n_gramu);

/*atg*/
n_gram stworz_n_gram(n_gram *gram, n_gram *gram2, int ile_slow, int nr_n_gramu, int los);

n_gram stworz_dane_o_n_gramie(n_gram *gram, atg_t *atg, int ile_slow, int nr_slowa );

n_gram oblicz_prawdopodobienstwo(n_gram *gram, int l_n_gramow);
/*
void realloc_wsk_na_sufiks(n_gram *gram, int size);

void kopiuj(int *tab_1, int *tab_2, int liczba_elementow);
*/
n_gram stworz_prefiks(n_gram *gram, int l_liter_prefiks, int nr_slowa);

n_gram stworz_sufiks(n_gram *gram, int l_liter_sufiks);

void zwolnij_n_gram(n_gram *gram, int wielkosc_tab_strukt, int rzad_n_gramu);

#endif


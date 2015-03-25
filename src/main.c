#include "atg.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

char *usage =
	"\nUżycie: %s\n"
	"./ATG –p plik1.txt plik2.txt plik3.txt ... \n"
	"./ATG -d plik1.txt plik2.txt plik3.txt ... \n" 
	"./ATG -m plik.txt  –g -s/-a [liczba słów/akapitów] -n [n-gramy] –l [ilość n-gramów]\n"
	"./ATG –g -s/-a [liczba słów/akapitów] -n [n-gramy] –l [ilość n-gramów]\n"
	"Legenda:\n"
	"-p pobiera do programu pliki z tekstem bazowym ( w formacie txt) domyślnie czyta z (../dane/cały_tekst.txt)\n"
	"-d dopisanie danych do pliku pośredniego\n"
	"-m pobiera plik pośredni ( w formacie txt)\n";
char *usage2 =
	"-g generuje tekst\n"
	"-s parametr służący do generacji tekstu o zadanej przez użytkownika liczbie słów\n"
	"-a parametr służący do generacji tekstu o zadanej przez użytkownika liczbie akapitów\n"
	"-n rząd n-gramu ( domyśnie 2 )\n" 
	"-l liczba wypisanych n-gramów ( domyśnie 100 )\n"
	"-h pomoc\n";

int main (int argc, char **argv){
	int i,j,a;
	int l_slow = 1; /*ile jest wszystkich słów */
	int start_size = 100;
	int zmienna_spr_p = 0;
	int zmienna_spr_d = 0;
	
	int opt;  /* switch */ 
	char *inp = NULL;
	char *ind = NULL;
	char *inm = NULL;
	char *progname = argv[0];
	int inn = 2;
	int inl = 100;

	atg_t *atg = NULL;
	drzewo_t drzewo = NULL;
	n_gram *gram = NULL;
	n_gram *gram2 = NULL;
	
/*	gram2->zliczenia_prefiks = 0;
	gram2->zliczenia_sufiks = 0;
*/	
	while((opt = getopt (argc,argv,"p:d:m:g:s:a:n:l:h")) !=-1){ 
		switch(opt){
		case 'p': /*pobiera do programu pliki z tekstem bazowym*/
			inp = optarg;
			zmienna_spr_p = 1;
			break;
		case 'd': /*dopisanie danych do pliku pośredniego*/
			ind = optarg;
			zmienna_spr_d = 1;
			break;
		case 'm': /*pobiera plik pośredni*/
			inm = optarg;
			break;
		case 'n': /*rząd n-gramu*/
			inn = atoi(optarg);
			break;
		case 'l': /*liczba wypisanych n-gramów*/ 
			inl = atoi(optarg);
			break;
		case 'h': /*pomoc*/
			fprintf(stderr, usage, progname);
			fprintf(stderr, usage2, progname);
			exit(EXIT_FAILURE);
		default:
			fprintf(stderr,"-h pomoc\n");
			exit(EXIT_FAILURE);
		}		
	}

	
	if( optind < argc && zmienna_spr_p == 0 && zmienna_spr_d == 0 ) {/*może być z tym problem, jak będzie kłopot z argumentami wywalić*/
		fprintf( stderr, "\nZłe parametry!\n" );
		for( ; optind < argc; optind++ )
			fprintf( stderr, "\t\"%s\"\n", argv[optind] );
		fprintf( stderr, usage, progname );
		fprintf(stderr, usage2, progname);
		exit( EXIT_FAILURE );
	}
	

	if(inp != NULL){ /***czyta z podanych plików i zapisuje słowa w strukturze*/
		FILE *out;
		atg = (atg_t*)malloc(sizeof (atg_t) *start_size);
		if( atg == NULL){
        	        fprintf(stderr, "Za mało pamięci\n");
                	exit(EXIT_FAILURE);
        	}
		for(j = 2; j < argc; j++){
			FILE *in = fopen(argv[j],"r");
			if( in == NULL)
				fprintf(stderr,"Nie mogę odczytać pliku: %s\n", argv[j]);
			
			atg = czytaj_slowa(in, &l_slow, &start_size,atg);
			l_slow++;

			/*for(i = 1;i< l_slow; i++){
				printf("%s %d %d\n ",atg[i].slowo,atg[i].index,atg[i].l_liter);
			}*/
			fclose(in);
		}
		out = fopen("../dane/cały_tekst.txt","w");
		zapisz_slowa( atg, out, &l_slow);
		fclose(out);
	}

	if(ind != NULL){ /***dopisuje dane do pliku****/
		FILE *in = fopen("../dane/cały_tekst.txt","r");
		FILE *out;
		atg = (atg_t*)malloc(sizeof (atg_t) *start_size);
		if( atg == NULL){
        	        fprintf(stderr, "Za mało pamięci\n");
                	exit(EXIT_FAILURE);
        	}
		if( in == NULL)
                                fprintf(stderr,"Nie mogę odczytać pliku: %s\n", ind);
		atg = czytaj_slowa(in, &l_slow, &start_size,atg);
		l_slow++;

		for(j = 2; j < argc; j++){
                        FILE *in2 = fopen(argv[j],"r");
                        if( in == NULL)
                                fprintf(stderr,"Nie mogę odczytać pliku: %s\n", argv[j]);

                        atg = czytaj_slowa(in2, &l_slow, &start_size, atg);
			l_slow++;

                        /*for(i = 1;i< l_slow; i++){  
                                printf("%s %d %d\n ",atg[i].slowo,atg[i].index,atg[i].l_liter);
                        }*/
                        fclose(in2);
                }
		fclose(in);
		out = fopen("../dane/cały_tekst.txt","w");
		zapisz_slowa( atg, out, &l_slow);
		fclose(out);
	}

	if(inp == NULL && ind == NULL){ /*czyta z pliku domyślnego*/
		FILE *in = fopen("../dane/cały_tekst.txt","r");
		atg = (atg_t*)malloc(sizeof (atg_t) *start_size);
		if( atg == NULL){
       		        fprintf(stderr, "Za mało pamięci\n");
       	        	exit(EXIT_FAILURE);
       		}
	
		if( in == NULL)
			fprintf(stderr,"Nie mogę odczytać pliku: %s\n", argv[j]);
		atg = czytaj_slowa(in, &l_slow, &start_size, atg);
		l_slow++;
		/*for(i = 1;i< l_slow; i++){ 
			printf("%s %d %d\n ",atg[i].slowo, atg[i].index, atg[i].l_liter);
		}*/
		fclose(in);
	}

		
/******************************tworzy tablice n_gramów*************************************************/
	gram2 =(n_gram*)malloc(sizeof (n_gram) * (l_slow - inn + 1));
	if( gram2 == NULL){
                fprintf(stderr, "Za mało pamięci\n");
                exit(EXIT_FAILURE);
        }
	for(i = 1; i<l_slow - inn; i++){
		gram2[i] = stworz_dane_o_n_gramie(gram2, atg, inn, i);
		/**//*wypisywanie pomocnicze */
		/*for(j=0;j<inn;j++)
			printf("%s ",gram2[i].prefiks[j]);
		printf("\n%s\n\n",gram2[i].sufiks);*/
		/**//*wypisywanie*/
	}
/******************************tworzy tablice n_gramów*************************************************/

/******************************drzewo******************************************************************/
	for(i = 1; i< l_slow - inn ; i++){
		drzewo = wstaw_do_drzewa(drzewo, &gram2[i], inn, cmp);
	}
/*	for(i = 1; i< l_slow - inn ; i++){
		drzewo = popraw_dane_statystyczne(drzewo, &gram2[i], inn, cmp);
	}

*/
	for(i = 1; i< l_slow - inn; i++){
              for(j=0;j<inn;j++)
                        printf("%s ",gram2[i].prefiks[j]);
                printf("  %d",gram2[i].zliczenia_prefiks);
                printf("\n%s",gram2[i].sufiks);
                printf("  %d\n\n",gram2[i].zliczenia_sufiks);
        }
	

/******************************drzewo******************************************************************/
/******************************prawdopodobieństwo******************************************************************/
/*	for(i = 1; i<l_slow - inn; i++){
		gram2[i] = oblicz_prawdopodobienstwo(&gram2[i], l_slow - inn - 1);

		for(j=0;j<inn;j++)
			printf("%s ",gram2[i].prefiks[j]);
		printf(" %.25f\n%s %.25f\n\n",gram2[i].p_wystapienia_prefiksu ,gram2[i].sufiks, gram2[i].p_wystapienia_sufiksu);

	}

*/

/******************************tworzy tablice n_gramów, wylosowanych***********************************/
/*	gram =(n_gram*)malloc(sizeof (n_gram) * inl);
	if( gram == NULL){
		fprintf(stderr, "Za mało pamięci\n");
                exit(EXIT_FAILURE);
	}
	srand(time(NULL));
	for(i = 0; i<inl ;i++){
		a = (rand() % l_slow) - inn;
	        while(a < inn)
        	        a = (rand() % l_slow) - inn;
		gram[i] = stworz_n_gram(gram, atg, inn, i, a);
*/	/*	for(j = 0;j<inn;j++){
			printf("%s ",gram[i].prefiks[j]);	
		}
		printf("\n%s\n\n",gram[0].sufiks);	
	}*/
/******************************tworzy tablice n_gramów, wylosowanych***********************************/

/*	zwolnij_pamiec(atg,l_slow);	*/
/*	zwolnij_n_gram(gram2, l_slow - inn , inn);*/ /* nie działa*/
/*	zwolnij_n_gram(gram, inl, inn);*/
/*	free(drzewo);	*/
return 0;
}


#include "atg.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>

	/*author:
	 *Cezary Wolszczak
	 *Elektryczny
	 *Informatyka
	 *2014/2015
	 */

char *usage =
	"\nUżycie: %s\n"
	"./ATG –p plik1.txt plik2.txt plik3.txt ... \n"
	"./ATG -d plik1.txt plik2.txt plik3.txt ... \n" 
	"./ATG -m plik.txt  –g [rodzaj operacji] -s/-a [liczba słów/akapitów] -n [n-gramy] –l [ilość n-gramów]\n"
	"./ATG –g [rodzaj operacji] -s/-a [liczba słów/akapitów] -n [n-gramy] –l [ilość n-gramów]\n"
	"Legenda:\n"
	"-p pobiera do programu pliki z tekstem bazowym ( w formacie txt) domyślnie czyta z (../dane/cały_tekst.txt)\n"
	"-d dopisanie danych do pliku pośredniego\n";
char *usage2 =
	"-m pobiera plik pośredni ( w formacie txt)\n"
	"-g generuje tekst (1 - generuj cały tekst, 2 - losowo generuje wybraną liczbę n_gramów, 3 - generuj z parametrem -s lub -a, domyślnie 1)\n"
	"-n rząd n-gramu ( domyśnie 2 )\n" 
	"-l liczba wypisanych n-gramów ( domyśnie 100 )\n"
	"-s parametr służący do generacji tekstu o zadanej przez użytkownika liczbie słów\n"
	"-a parametr służący do generacji tekstu o zadanej przez użytkownika liczbie akapitów\n"
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
	int ing = 1;
	int ins = 0;
	int ina = 0;
	
	FILE *in_n_gram;
	FILE *in_staty1;
	FILE *in_staty2;

	atg_t *atg = NULL;
	drzewo_t drzewo = NULL;
	n_gram *gram = NULL;
	n_gram *gram2 = NULL;
	n_gram *gram3 = NULL;
	n_gram *gram4 = NULL;
	n_gram *gram5 = NULL;
	
/*	gram2->zliczenia_prefiks = 0;
	gram2->zliczenia_sufiks = 0;
*/	
	while((opt = getopt (argc,argv,"p:d:m:g:n:l:s:a:h")) !=-1){ 
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
		case 'g': /*generacja tekstu*/
			ing = atoi(optarg);
			break;
		case 'n': /*rząd n-gramu*/
			inn = atoi(optarg);
			break;
		case 'l': /*liczba wypisanych n-gramów*/ 
			inl = atoi(optarg);
			break;
		case 's':/*liczba słów do generacji*/
			ins = atoi(optarg);
			break;
		case 'a':/*liczba akapitów do generacji*/
			ina = atoi(optarg);
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

	if((ing !=1) && (ing !=2) && (ing !=3)){
		fprintf(stderr,"Zły parametr g, możliwy do wyboru 1 lub 2\n");
		exit( EXIT_FAILURE );
	}
	
	if(ina < 0){
		fprintf(stderr,"Parametr a musi być większy od zera\n");
                exit( EXIT_FAILURE );
        }
	if(ins < 0){
		fprintf(stderr,"Parametr s musi być większy od zera\n");
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

	if(ins > l_slow){/*sprawdzenie parametru s*/
		fprintf(stderr,"Błąd! Parametr \"s\". Za duża liczba słów(s > całkowitej liczby słów)\n");
		 exit(EXIT_FAILURE);
                }
		
/******************************tworzy podstawową tablice n_gramów************************************/
	gram2 =(n_gram*)malloc(sizeof (n_gram) * (l_slow - inn + 1));
	if( gram2 == NULL){
                fprintf(stderr, "Za mało pamięci\n");
                exit(EXIT_FAILURE);
        }
	for(i = 1; i<l_slow - inn; i++){
		gram2[i] = stworz_dane_o_n_gramie(gram2, atg, inn, i);
		/**//*wypisywanie pomocnicze */
/*		for(j=0;j<inn;j++)
			printf("%s ",gram2[i].prefiks[j]);
		printf("\n%s %d\n\n",gram2[i].sufiks,gram2[i].index);
*/		/**//*wypisywanie*/
	}
/******************************tworzy podstawową tablice n_gramów*************************************/
/******************************drzewo+zapis statów do pliku********************************************/
	for(i = 1; i< l_slow - inn ; i++){
		drzewo = wstaw_do_drzewa(drzewo, &gram2[i], inn, cmp);
	}
	for(i = 1; i< l_slow - inn ; i++){
		if(gram2[i].zliczenia_prefiks > 1)
			szukaj_n_gramow(drzewo, &gram2[i], inn, cmp);
	}

	
	in_n_gram = fopen("../gen/podstawowe_n_gramy/podstawowe_n_gramy.txt","w");
	zapisz_wylosowane_n_gramy(gram2, in_n_gram, l_slow - inn - 1, inn);
	fclose(in_n_gram);

	in_staty1 = fopen("../gen/podstawowe_n_gramy/dane_statystyczne_o_n_gramie_podstawowym.txt","w");
	if(in_staty1 == NULL){
		fprintf(stderr,"Nie mogę zapisać do pliku \"dane_statystyczne_o_n_gramie_podstawowym.txt\" ");
		exit(EXIT_FAILURE);
	}
	
	zapisz_dane_statystyczne_n_gram(gram2, in_staty1, l_slow - inn, inn); /*zapisanie danych statystycznych(liczba wystąpień prefiksów i sufisków) przed generacją */
	fclose(in_staty1);
/*	for(i = 1; i< l_slow - inn; i++){
              for(j=0;j<inn;j++)
                        printf("%s ",gram2[i].prefiks[j]);
                printf("  %d",gram2[i].zliczenia_prefiks);
                printf("\n%s",gram2[i].sufiks);
                printf("  %d\n\n",gram2[i].zliczenia_sufiks);
        }
*/	

/******************************drzewo+zapis statów do pliku********************************************/
/******************************prawdopodobieństwo 1 + zapis********************************************/
	for(i = 1; i<l_slow - inn; i++){
		gram2[i] = oblicz_prawdopodobienstwo(&gram2[i], l_slow - inn - 1);

/*		for(j=0;j<inn;j++)
			printf("%s ",gram2[i].prefiks[j]);
		printf(" %.25f\n%s %.25f\n %.25f\n\n",gram2[i].p_wystapienia_prefiksu ,gram2[i].sufiks, gram2[i].p_wystapienia_sufiksu, gram2[i].p_wystapienia_n_gramu);
*/
	}
	
	in_staty2 = fopen("../gen/podstawowe_n_gramy/prawdopodobienstwo_wystąpienia_n_gram_podstawowy.txt","w");
	if(in_staty2 == NULL){
		fprintf(stderr,"Nie mogę zapisać do pliku \"prawdopodobienstwo_wystąpienia_n_gram_podstawowy.txt\" ");
		exit(EXIT_FAILURE);
	}

	zapisz_prawdopodobienstow_n_gram(gram2, in_staty2, l_slow - inn, inn); /*zapisanie prawdopodobieństwa wystąpienia poszczególnych prefiksów i sufiksów przed generacją*/
	fclose(in_staty2);

/******************************prawdopodobieństwo 1 + zapis********************************************/
/******************************tworzy tablice n_gramów w oparciu o liczbe słów*************************/
	if (ing == 3){
		if(ins != 0){
			FILE *in = fopen("../gen/częściowy_tekst/tekst_wygenerowany_z_podanej_liczby_slow.txt","w");
			gram4 =(n_gram*)malloc(sizeof (n_gram) * (ins + 1));
	                if( gram4 == NULL){
                	        fprintf(stderr, "Za mało pamięci\n");
        	                exit(EXIT_FAILURE);
        	        }

			for(i = 1; i<ins + 1 ;i++){
                      		gram4[i] = stworz_n_gram(gram4, gram2, inn, i, i);
	                      	for(j = 0;j<inn;j++){
                                	printf("%s ",gram4[i].prefiks[j]);       
                        	}
                        	printf("\n%s\n\n",gram4[i].sufiks);      
	              		gram4[i] = oblicz_prawdopodobienstwo(&gram4[i], ins);
                	}

			zapisz_konkretna_liczbe_slow(gram4, in, ins + 1, inn);
			fclose(in);
		}

		if(ina != 0){
			FILE *in2 = fopen("../gen/częściowy_tekst/tekst_wygenerowany_z_podanej_liczby_akapitów.txt","w");
			ina *= 100;
			if(ina > l_slow){
				fprintf(stderr, "Błąd! Parametr \"a\". Proszę podać mniejszą liczbę akapitów( a > liczby słów)\n");
                                exit(EXIT_FAILURE);
			}
                        gram5 =(n_gram*)malloc(sizeof (n_gram) * (ina + 1));
                        if( gram5 == NULL){
                                fprintf(stderr, "Za mało pamięci\n");
                                exit(EXIT_FAILURE);
                        }

                        for(i = 1; i<ina + 1 ;i++){
                                gram5[i] = stworz_n_gram(gram5, gram2, inn, i, i);
                                for(j = 0;j<inn;j++){
                                        printf("%s ",gram5[i].prefiks[j]);
                                }
                                printf("\n%s\n\n",gram5[i].sufiks);
                                gram5[i] = oblicz_prawdopodobienstwo(&gram5[i], ina);
                        }

                        zapisz_konkretna_liczbe_akapitow(gram5, in2, ina + 1, inn);
                        fclose(in2);
                }

	}
/******************************tworzy tablice n_gramów w oparciu o liczbe słów*************************/
/******************************tworzy tablice n_gramów, wylosowanych***********************************/
	else if(ing == 2){/*losowo wybiera n_gramy*/
		FILE *in = fopen("../gen/losowe_n_gramy/wylosowane_n_gramy.txt","w");
		FILE *in2 = fopen("../gen/losowe_n_gramy/dane_statystyczne_wylosowanych_n_gramow.txt","w");
		FILE *in3 = fopen("../gen/losowe_n_gramy/prawdopodobieństwo_wystąpienia_wylosowanego_n_gramu.txt","w");
		gram3 =(n_gram*)malloc(sizeof (n_gram) * inl);
		if( gram3 == NULL){
			fprintf(stderr, "Za mało pamięci\n");
       		        exit(EXIT_FAILURE);
		}
		srand(time(NULL));
		for(i = 1; i<inl ;i++){
			a = (rand() % l_slow) - inn;
		        while(a < inn)
        		        a = (rand() % l_slow) - inn;
		
			gram3[i] = stworz_n_gram(gram3, gram2, inn, i, a);
/*			for(j = 0;j<inn;j++){
				printf("%s ",gram3[i].prefiks[j]);	
			}
			printf("\n%s\n\n",gram3[i].sufiks);		
*/		
		gram3[i] = oblicz_prawdopodobienstwo(&gram3[i], inl);
		}
		zapisz_prawdopodobienstow_n_gram(gram3, in3, inl, inn);
		fclose(in3);
		zapisz_dane_statystyczne_n_gram(gram3, in2, inl, inn);
		fclose(in2);
		zapisz_wylosowane_n_gramy(gram3, in, inl, inn);
		fclose(in);
	}

/******************************tworzy tablice n_gramów, wylosowanych***********************************/
/******************************tworzy tablice n_gramów, wszystko***************************************/
	else if(ing == 1){/*tworzy nowy tekst z n_gramów*/
		FILE *in = fopen("../gen/przegenerowany_tekst/przegenerowany_tekst.txt","w");
		FILE *in2 = fopen("../gen/przegenerowany_tekst/dane_statystyczne_tekstu_po_generacji.txt","w");
		FILE *in3 = fopen("../gen/przegenerowany_tekst/prawdopodobieństwo_wystąpienia_n_gramu_w_tekscie.txt","w");
		gram =(n_gram*)malloc((l_slow - inn) *sizeof (n_gram));
		if( gram == NULL){
			fprintf(stderr, "Za mało pamięci\n");
	                exit(EXIT_FAILURE);
		}
		for(i = 1; i<l_slow - inn ;i++){
			gram[i] = stworz_n_gram(gram, gram2, inn, i, i);
/*			for(j = 0;j<inn;j++){
				printf("%s ",gram[i].prefiks[j]);	
			}
			printf("\n%s\n\n",gram[i].sufiks);	
*/		gram[i] = oblicz_prawdopodobienstwo(&gram[i], l_slow - inn - 1);
		}
		
		zapisz_przegenerowany_tekst(gram, stdout, l_slow - inn, inn);/*pomocnicze wyoisywanie*/

		zapisz_prawdopodobienstow_n_gram(gram, in3, l_slow - inn, inn);
		fclose(in3);
		zapisz_dane_statystyczne_n_gram(gram, in2, l_slow - inn, inn);
		fclose(in2);
		zapisz_przegenerowany_tekst(gram, in, l_slow - inn, inn);
		fclose(in);	
	}
/******************************tworzy tablice n_gramów, wszystko***************************************/
	zwolnij_pamiec(atg,l_slow);	
	zwolnij_n_gram2(gram2, l_slow - inn , inn); 
	if(ing == 1)
		zwolnij_n_gram(gram, l_slow - inn, inn);
	if(ing == 2)
		zwolnij_n_gram(gram3, inl , inn);
/*	if(ing == 3)
		zwolnij_n_gram3(gram3, ins + 1, inn);
*/
/*	free(drzewo);	*/
return 0;
}


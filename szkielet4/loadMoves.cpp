#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include "loadMoves.h"

int roszada(bool l, bool typ){  //typ true - krotka roszada, l - true(biale) czy false(czarne)
	if ((typ == true) && (l == true))
		return 1;
	else if ((typ == false) && (l == true))
		return 2;
	else if ((typ == true) && (l == false))
		return 3;
	else if ((typ == false) && (l == false))
		return 4;
return 0;
}

int col(char c){
	switch (c){
		case 'a': return 0; break;
		case 'b': return 1; break;
		case 'c': return 2; break;
		case 'd': return 3; break;
		case 'e': return 4; break;
		case 'f': return 5; break;
		case 'g': return 6; break;
		case 'h': return 7; break;
		default: printf("Bledna wspolrzedna!"); break;
	}
	return 8;
}

void takeInput(int fromx, int fromy, int tox, int toy, char promotion, bool szach, bool mat, int roszada, std::vector <movements> &moves){
	movements tmp;
	tmp.fromx = fromx;
	tmp.fromy = fromy;
	tmp.tox = tox;
	tmp.toy = toy;
	tmp.promotion = promotion;
	tmp.szach = szach;
	tmp.mat = mat;
	tmp.roszada = roszada;

	moves.push_back(tmp);
}

bool szachMat(bool choice, char c){
	if (((choice == false) && (c == '+')) || ((choice == true) && (c == '#')))
		return true;
	return false;	
}

char promotion(bool licz, char c){
	if ((licz == true) && (c == 'N'))
		return 'n';
return c;
}

void loadFile(std::vector <movements> &moves){
	FILE *fp;
	char a[8];
	bool licznik = true; //biale / czarne
	if ((fp = fopen("test.pgn", "r")) == NULL){
		printf("Blad otwarcia pliku!\n");
		system("PAUSE");
		exit(1);
	}

	while (!feof(fp)){
		fscanf(fp, "%s", a);

		if (strcmp(a, "1-0") == 0) {						//Ostatni element tablicy - fromx= 0 (remis), =1 (biale wygraly), =2 (czarne)
			takeInput(-1, 0, 0, 0, 0, false, false, 0, moves);
			break;
		}
		else if (strcmp(a, "0-1") == 0){
			takeInput(-2, 0, 0, 0, 0, false, false, 0, moves);
			break;
		}
		else if (strcmp(a, "1/2-1/2") == 0){
			takeInput(-3, 0, 0, 0, 0, false, false, 0, moves);
			break;
		}

		if ((strlen(a) == 5) && (strcmp(a, "O-O-O")== 0))
			takeInput(0, 0, 0, 0, 0, false, false, roszada(licznik, false), moves);				//d³uga roszada
		else if (strlen(a) == 5)
			takeInput(col(a[0]), a[1] - '1', col(a[3]), a[4] - '1', 0, false, false, 0, moves);	//Wypelnianie tablicy
		else if ((strlen(a) == 6) && ((a[5] == '+') || (a[5] == '#')))
				takeInput(col(a[0]), a[1] - '1', col(a[3]), a[4] - '1', 0, szachMat(false,a[5]), szachMat(true,a[5]),0, moves);
		else if ((strlen(a) == 6) && !((a[5] == '+') || (a[5] == '#')))
				takeInput(col(a[0]), a[1] - '1', col(a[3]), a[4] - '1', promotion(licznik, a[5]), false, false,0, moves);
		else if (strlen(a) == 7)
			takeInput(col(a[0]), a[1] - '1', col(a[3]), a[4] - '1', promotion(licznik, a[5]), szachMat(false, a[6]), szachMat(true, a[6]), 0, moves);
		else if ((strlen(a) == 3) && ((strcmp(a, "O-O")) == 0))
			takeInput(0,0,0,0,0, false, false, roszada(licznik, true), moves);		//krótka roszada

		licznik = !licznik;
	}	
	fclose(fp);
}
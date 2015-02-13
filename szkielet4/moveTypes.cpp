#include "moveTypes.h"
#include <iostream>

//ZnajdŸ figurê znajduj¹c¹ siê na danym polu
chessMan* findChessMen(int x, int y, chessMan *figury[32], field pola[8][8]) {
	chessMan *result = NULL;
	for(int i = 0; i < 32; i++)
		if(figury[i]->cheeck(pola[x][y])) {
			result = figury[i];
			break;
		}
	return result;
}

//Procedura wykonania normalnego ruchu
void normalMove(chessMan *figury[32], chessMan *moving[2], chessMan **beating, field pola[8][8], model *modele[11], movements it) {
	//Która figura wykonuje ruch?
	moving[0] = findChessMen(it.fromx, it.fromy, figury, pola);
	//Ustaw pole, do którego nastêpuje ruch
	moving[0]->setDest(pola[it.tox][it.toy]);
	//Czy nastepuje bicie?
	*beating = findChessMen(it.tox, it.toy, figury, pola);
	//Jeœli tak, to zbij figurê
	if(*beating != NULL) {
		(*beating)->beat();
	}
	//Czy ma miejsce promocja figury?
	if(it.promotion != 0)
		switch (it.promotion) {
		case 'Q':
			moving[0]->promote(modele[10]);
			break;
		case 'R':
			moving[0]->promote(modele[5]);
			break;
		case 'B':
			moving[0]->promote(modele[8]);
			break;
		case 'n':
			moving[0]->promote(modele[6]);
			break;
		case 'N':
			moving[0]->promote(modele[7]);
			break;
		default:
			break;
		}
}

//Procedura wykonania roszady
void castling(chessMan *figury[32], chessMan *moving[2], field pola[8][8], movements it) {
	switch (it.roszada) {
	case 1:
		moving[0] = figury[17];
		moving[0]->setDest(pola[5][0]);
		moving[1] = figury[28];
		moving[1]->setDest(pola[6][0]);
		break;
	case 2:
		moving[0] = figury[16];
		moving[0]->setDest(pola[3][0]);
		moving[1] = figury[28];
		moving[1]->setDest(pola[2][0]);
		break;
	case 3:
		moving[0] = figury[19];
		moving[0]->setDest(pola[5][7]);
		moving[1] = figury[29];
		moving[1]->setDest(pola[6][7]);
		break;
	case 4:
		moving[0] = figury[18];
		moving[0]->setDest(pola[3][7]);
		moving[1] = figury[29];
		moving[1]->setDest(pola[2][7]);
		break;
	default:
		break;
	}
}
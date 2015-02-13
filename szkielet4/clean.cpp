#include "clean.h"

//Zwolnienie modeli
void cleanModels(model *modele[11]) {
	for(int i = 0; i < 10; i++)
		delete modele[i];
}

//Zwolnienie szachownicy
void cleanBoard(board *szachownica[4]) {
	for(int i = 0; i < 4; i++)
		delete szachownica[i];
}

//Zwolnienie pionków
void cleanChesseMen(chessMan *figury[32]) {
	for(int i = 0; i < 32; i++)
		delete figury[i];
}

//Zwolnienie tekstur
void cleanTextures(GLuint tex[6]) {
	glDeleteTextures(6, tex);
}
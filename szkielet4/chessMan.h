#ifndef CHESSMAN_H
#define CHESSMAN_H

#include "model.h"

//Struktura pola
struct field {
	float x;
	float z;
};

class chessMan {
public:
	chessMan(model *inModel, field pole, GLuint *inTex, GLuint *spec); //Konstruktor
	void draw(ShaderProgram *shaderProgram, bool reflectMode); //Metoda rysuj¹ca
	bool move(); //Funkcja wykonuj¹ca ruch
	void setDest(field dest); //Metoda ustawiaj¹ca pole, do którego pod¹¿a figura
	bool cheeck(field pole); //Funkcja sprawdzaj¹ca, czy jakiœ pionek stoi na danym polu
	void beat(); //Metoda ustawiaj¹ca figurê jako zbit¹ (seter beated)
	bool isBeated(); //Funkcja sprawdzaj¹ca, czy figura jest obecnie bita (geter beated)
	void promote(model *newModel); //Metoda odpowiadaj¹ca za promocjê figury
	void decAlpha(); //Metoda odpowiadaj¹ca za zanikanie figury bitej
private:
	model *manModel; //WskaŸnik na model
	glm::mat4 matM; //Macierz modelu dla danej figury
	GLuint *tex; //WskaŸnik na teksturê
	GLuint *texSpec; //WskaŸnik na teksturê odbicia (???)
	field currField; //Obecne pole, na którym znajduje siê figura
	field destField; //Pole, do którego pod¹¿a figura
	float toMoveX; //Ruch na jednostkê czasu po osi X
	float toMoveZ; //Ruch na jednostkê czasu po osi Z
	int iteration; //Ile iteracji animacji ju¿ wykonano
	float alpha; //Wspó³czynnik przeŸroczystoœci figury (wartoœæ kana³u alfa)
	bool beated; //Czy figura zosta³a zbita
};

#endif
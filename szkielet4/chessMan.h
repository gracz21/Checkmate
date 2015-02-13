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
	void draw(ShaderProgram *shaderProgram, bool reflectMode); //Metoda rysuj�ca
	bool move(); //Funkcja wykonuj�ca ruch
	void setDest(field dest); //Metoda ustawiaj�ca pole, do kt�rego pod��a figura
	bool cheeck(field pole); //Funkcja sprawdzaj�ca, czy jaki� pionek stoi na danym polu
	void beat(); //Metoda ustawiaj�ca figur� jako zbit� (seter beated)
	bool isBeated(); //Funkcja sprawdzaj�ca, czy figura jest obecnie bita (geter beated)
	void promote(model *newModel); //Metoda odpowiadaj�ca za promocj� figury
	void decAlpha(); //Metoda odpowiadaj�ca za zanikanie figury bitej
private:
	model *manModel; //Wska�nik na model
	glm::mat4 matM; //Macierz modelu dla danej figury
	GLuint *tex; //Wska�nik na tekstur�
	GLuint *texSpec; //Wska�nik na tekstur� odbicia (???)
	field currField; //Obecne pole, na kt�rym znajduje si� figura
	field destField; //Pole, do kt�rego pod��a figura
	float toMoveX; //Ruch na jednostk� czasu po osi X
	float toMoveZ; //Ruch na jednostk� czasu po osi Z
	int iteration; //Ile iteracji animacji ju� wykonano
	float alpha; //Wsp�czynnik prze�roczysto�ci figury (warto�� kana�u alfa)
	bool beated; //Czy figura zosta�a zbita
};

#endif
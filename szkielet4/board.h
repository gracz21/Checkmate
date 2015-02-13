#ifndef BOARD_H
#define BOARD_H

#include "model.h"

class board {
public:
	board(model *inModel, GLuint *inTex, GLuint *spec); //Konstruktor
	void draw(ShaderProgram *shaderProgram, float alpha, bool reflectionMode); //Metoda rysuj�ca
private:
	model *boardModel; //Wka�nik na model
	glm::mat4 matM; //Macierz modelu
	GLuint *tex; //Wka�nik na tekstur�
	GLuint *texSpec; //Wska�nik na tekstur� odbicia (???)
};

#endif
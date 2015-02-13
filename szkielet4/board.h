#ifndef BOARD_H
#define BOARD_H

#include "model.h"

class board {
public:
	board(model *inModel, GLuint *inTex, GLuint *spec); //Konstruktor
	void draw(ShaderProgram *shaderProgram, float alpha, bool reflectionMode); //Metoda rysujπca
private:
	model *boardModel; //Wkaünik na model
	glm::mat4 matM; //Macierz modelu
	GLuint *tex; //Wkaünik na teksturÍ
	GLuint *texSpec; //Wskaünik na teksturÍ odbicia (???)
};

#endif
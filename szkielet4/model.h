#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp"
#include <vector>
#include "shaderprogram.h"


class model {
public:
	GLuint bufVertices; //Uchwyt na bufor VBO przechowujący wektor współrzędnych wierzchołków
	GLuint bufTex; //Uchwyt na bufor VBO przechowujący wektor koordynatów teksturowania
	GLuint bufNormals;//Uchwyt na bufor VBO przechowujący wektor wektorów normalnych
	GLuint vao; //Obiekt VAO dla danego modelu
	int indicesSize; //Liczba wierzchołków
	model(char* fileName, ShaderProgram *shaderProgram); //Konstruktor (wyczytanie .obj, utworzenie VBO i VAO)
	~model(); //Destruktor (usunięcie VBO i VAO)
};

#endif

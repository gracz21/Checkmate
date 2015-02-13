#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp"
#include <vector>
#include "shaderprogram.h"


class model {
public:
	GLuint bufVertices; //Uchwyt na bufor VBO przechowuj¹cy wektor wspó³rzêdnych wierzcho³ków
	GLuint bufTex; //Uchwyt na bufor VBO przechowuj¹cy wektor koordynatów teksturowania
	GLuint bufNormals;//Uchwyt na bufor VBO przechowuj¹cy wektor wektorów normalnych
	GLuint vao; //Obiekt VAO dla danego modelu
	int indicesSize; //Liczba wierzcho³ków
	model(char* fileName, ShaderProgram *shaderProgram); //Konstruktor (wyczytanie .obj, utworzenie VBO i VAO)
	~model(); //Destruktor (usuniêcie VBO i VAO)
};

#endif

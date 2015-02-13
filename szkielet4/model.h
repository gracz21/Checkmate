#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp"
#include <vector>
#include "shaderprogram.h"


class model {
public:
	GLuint bufVertices; //Uchwyt na bufor VBO przechowuj�cy wektor wsp�rz�dnych wierzcho�k�w
	GLuint bufTex; //Uchwyt na bufor VBO przechowuj�cy wektor koordynat�w teksturowania
	GLuint bufNormals;//Uchwyt na bufor VBO przechowuj�cy wektor wektor�w normalnych
	GLuint vao; //Obiekt VAO dla danego modelu
	int indicesSize; //Liczba wierzcho�k�w
	model(char* fileName, ShaderProgram *shaderProgram); //Konstruktor (wyczytanie .obj, utworzenie VBO i VAO)
	~model(); //Destruktor (usuni�cie VBO i VAO)
};

#endif

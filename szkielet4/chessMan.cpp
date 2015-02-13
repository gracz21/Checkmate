#include "chessMan.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

//Liczba iteracji w animowaniu pionka
const int nIt = 20;

//Kontruktor
chessMan::chessMan(model *inModel, field pole, GLuint *inTex, GLuint *spec) {
	manModel = inModel;
	matM = glm::translate(glm::mat4(1.0f), glm::vec3(pole.x,0.0f,pole.z));
	tex = inTex;
	texSpec = spec;
	beated = false;
	currField = pole;
	alpha = 1.0f;
}

//Metoda rysowania pionka
void chessMan::draw(ShaderProgram *shaderProgram, bool reflectMode) {
	//aktywacja tekstur
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *tex);
	if(!reflectMode) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *texSpec);
	}	
	//aktywacja macierzy M danego modelu
	if(reflectMode) {
		glm::mat4 tmpM = glm::scale(matM, glm::vec3(1.0f,-1.0f,1.0f));
		tmpM = glm::translate(tmpM, glm::vec3(0,0.55f,0));
		glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(tmpM));
	} else {
		glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(matM));
		//ustawienie wspó³czynnika plamki odbicia
		glUniform1f(shaderProgram->getUniformLocation("shininess"), 50);
		//ustawienie wartoœci kana³u alfa (przeŸroczystoœæ przy zbijaniu)
		glUniform1f(shaderProgram->getUniformLocation("alpha"), alpha);
	}
	
	//aktywacja VAO danego modelu
	glBindVertexArray(manModel->vao);

	//narysowanie modelu
	glDrawArrays(GL_TRIANGLES, 0, manModel->indicesSize);
}

//Metoda animacji pionka
bool chessMan::move() {
	matM = glm::translate(matM, glm::vec3(float(toMoveX/nIt), 0.0f, float(toMoveZ/nIt)));
	iteration++;
	if(iteration == nIt) {
		currField = destField;
		return true;
	}
	else return false;
}

//Ustaw pole, do którego nastêpuje ruch
void chessMan::setDest(field dest) {
	destField = dest;
	toMoveX = destField.x - currField.x;
	toMoveZ = destField.z - currField.z;
	iteration = 0;
}

//Sprawdzanie, czy jakiœ pionek stoi na danym polu
bool chessMan::cheeck(field pole) {
	if((currField.x == pole.x) && (currField.z == pole.z) && !beated)
		return true;
	else return false;
}

//Ustawienie pionka jako zbitego
void chessMan::beat() {
	beated = true;
}

bool chessMan::isBeated() {
	return beated;
}

void chessMan::promote(model *newModel) {
	manModel = newModel;
}

void chessMan::decAlpha() {
	alpha -= 1.00/nIt;
}
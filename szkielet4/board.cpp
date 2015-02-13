#include "board.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

board::board(model *inModel, GLuint *inTex, GLuint *spec) {
	boardModel = inModel;
	matM = glm::mat4(1.0f);
	tex = inTex;
	texSpec = spec;
}

void board::draw(ShaderProgram *shaderProgram, float alpha, bool reflectionMode) {
	//aktywacja tekstur
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *tex);
	if(!reflectionMode) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *texSpec);
	}
	//aktywacja macierzy M danego modelu
	glUniformMatrix4fv(shaderProgram->getUniformLocation("M"), 1, false, glm::value_ptr(matM));
	if(!reflectionMode) {
		glUniform1f(shaderProgram->getUniformLocation("shininess"), 25);
		glUniform1f(shaderProgram->getUniformLocation("alpha"), alpha);
	}
	
	//aktywacja VAO danego modelu
	glBindVertexArray(boardModel->vao);

	//narysowanie modelu
	glDrawArrays(GL_TRIANGLES, 0, boardModel->indicesSize);
}
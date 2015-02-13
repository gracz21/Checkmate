#include "font.h"
#include <iostream>

using namespace std;

font::font(char *fileName) {
	FT_Library library;
	cout << "Inicjalizacjia biblioteki FreeType..." << endl;
	if (FT_Init_FreeType(&library))
		cout << "Nie mo¿na zainicjowac biblioteki FreeType!" << endl;
	cout << "Biblioteka FreeType zainicjalizowana!" << endl << "Wczytywanie czcionki " << fileName << "..." << endl;
	if(FT_New_Face(library, fileName, 0, &face))
		cout << "Nie mozna otworzyc czcionki" << endl;
	cout << "Czcionka wczytana!" << endl;
	FT_Set_Pixel_Sizes(face, 0, 250);
	g = face->glyph;
}

void font::draw(const char *text, float x, float y, float sx, float sy, ShaderProgram *shaderProgram) {
	const char *p;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D,tex);
	glUniform1i(shaderProgram->getUniformLocation("textureMap0"), 0);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenBuffers(1, &vbo);
	glEnableVertexAttribArray(shaderProgram->getAttribLocation("coord"));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(shaderProgram->getAttribLocation("coord"), 4, GL_FLOAT, GL_FALSE, 0, 0);

	for (p = text; *p; p++) {
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;

		glTexImage2D (GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0,GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		float x2 = x + g->bitmap_left * sx;
		float y2 = -y - g->bitmap_top * sy;
		float w = g->bitmap.width * sx;
		float h = g->bitmap.rows * sy;

		GLfloat box[4][4] = {
			{x2,     -y2    , 0, 0},
			{x2 + w, -y2    , 1, 0},
			{x2,     -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}

	glDisableVertexAttribArray(shaderProgram->getAttribLocation("coord"));
	glDeleteTextures(1, &tex);
}

font::~font() {
	glDeleteBuffers(1, &vbo);
}
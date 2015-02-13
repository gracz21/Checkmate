#ifndef FONT_H
#define FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "glm\glm.hpp"
#include "shaderprogram.h"

class font {
public:
	font(char *fileName); //Konstruktor (inicjalizacja biblioteki + wczytanie czcionki)
	void draw(const char *text, float x, float y, float sx, float sy, ShaderProgram *shaderProgram); //Metoda renderuj¹ca czcionkê
	~font(); //Dekonstruktor (usuwanie vbo)
private:
	FT_Face face;
	FT_GlyphSlot g;
	GLuint vbo;
	GLuint tex;
};

#endif
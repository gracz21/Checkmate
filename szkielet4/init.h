#ifndef INIT_H
#define INIT_H

#include "board.h"
#include "texture.h"
#include "chessMan.h"
#include "shaderprogram.h"

void setupShaders(ShaderProgram **shaderProgram, ShaderProgram **simple, ShaderProgram **sfont);
void setupChessSet(model *modele[11], 
				   GLuint tex[3], 
				   board *szachownica[4], 
				   field pola[8][8], 
				   chessMan *figury[32], 
				   chessMan *moving[2], 
				   chessMan **beating, 
				   ShaderProgram *shaderProgram);

#endif
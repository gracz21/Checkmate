#ifndef FREE_H
#define FREE_H

#include "board.h"
#include "chessMan.h"
#include "shaderprogram.h"
#include "loadMoves.h"

void cleanModels(model *modele[11]);
void cleanBoard(board *szachownica[4]);
void cleanChesseMen(chessMan *figury[32]);
void cleanTextures(GLuint tex[4]);

#endif
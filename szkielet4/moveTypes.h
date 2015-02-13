#ifndef MOVETYPES_H
#define MOVETYPES_H

#include "loadMoves.h"
#include "chessMan.h"

void normalMove(chessMan *figury[32], chessMan *moving[2], chessMan **beating, field pola[8][8], model *modele[11], movements it);
void castling(chessMan *figury[32], chessMan *moving[2], field pola[8][8], movements it);

#endif
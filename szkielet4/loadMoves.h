#ifndef movements_h
#define movements_h

#include <vector>

struct movements {
	int fromx;
	int fromy;
	int tox;
	int toy;
	char promotion;
	bool szach;
	bool mat;
	int roszada;
};

void loadFile(std::vector <movements> &moves); // £adowanie pliku z zapisem rozgrywki

#endif
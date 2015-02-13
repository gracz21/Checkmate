#include "init.h"

//krawedz pola szachownicy 0.48462 !!!
const float krawedz = 0.48462f;

//Wczytuje vertex shader i fragment shader i ³¹czy je w program cieniuj¹cy
void setupShaders(ShaderProgram **shaderProgram, ShaderProgram **simple, ShaderProgram **sfont) {
	*shaderProgram = new ShaderProgram("shaders\\regular\\vshader.txt", NULL, "shaders\\regular\\fshader.txt");
	*simple = new ShaderProgram("shaders\\simple\\vshader.txt", NULL, "shaders\\simple\\fshader.txt");
	*sfont = new ShaderProgram("shaders\\font\\vshader.txt", NULL, "shaders\\font\\fshader.txt");
}

//Wczytanie modeli
void setupModels(model *modele[11], ShaderProgram *shaderProgram) {
	modele[0] = new model("models\\chessBoard\\podstawa.obj", shaderProgram);
	modele[1] = new model("models\\chessBoard\\biale.obj", shaderProgram);
	modele[2] = new model("models\\chessBoard\\czarne.obj", shaderProgram);
	modele[3] = new model("models\\chessBoard\\ramka.obj", shaderProgram);
	modele[4] = new model("models\\chessMen\\pionek.obj", shaderProgram);
	modele[5] = new model("models\\chessMen\\wieza.obj", shaderProgram);
	modele[6] = new model("models\\chessMen\\skoczek_b.obj", shaderProgram);
	modele[7] = new model("models\\chessMen\\skoczek_c.obj", shaderProgram);
	modele[8] = new model("models\\chessMen\\goniec.obj", shaderProgram);
	modele[9] = new model("models\\chessMen\\krol.obj", shaderProgram);
	modele[10] = new model("models\\chessMen\\hetman.obj", shaderProgram);
}

//Wczytanie tekstur
void setupTextures(GLuint tex[6]) {
	tex[0] = readTexture("textures\\wood.tga");
	tex[1] = readTexture("textures\\white.tga");
	tex[2] = readTexture("textures\\black.tga");
	tex[3] = readTexture("textures\\black_spec.tga");
	tex[4] = readTexture("textures\\white_spec.tga");
	tex[5] = readTexture("textures\\wood_spec.tga");
}

//Wczytanie modelu szachownicy
void setupBoard(board *szachownica[4], model *modele[11], GLuint tex[6]) {
	szachownica[0] = new board(modele[0], &tex[0], &tex[5]);
	szachownica[1] = new board(modele[1], &tex[1], &tex[4]);
	szachownica[2] = new board(modele[2], &tex[2], &tex[3]);
	szachownica[3] = new board(modele[3], &tex[2], &tex[3]);
}

//Obliczanie wspó³rzêdnych pól szachownicy
void setupFields(field pola[8][8]) {
	int i, j; //i - kolumna, j - wiersz; np. pola[0][1] = a2
	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++) {
			pola[j][i].x = (3.5 - j)*krawedz;
			pola[j][i].z = (-3.5 + i)*krawedz;
		}
}

//Wczytanie pionków
void setupChessMen(model *modele[11], GLuint tex[6], field pola[8][8], chessMan *figury[32]) {
	int i;

	//Wczytanie bialych pionków
	for(i = 0; i < 8; i++)
		figury[i] = new chessMan(modele[4], pola[i][1], &tex[1], &tex[4]);

	//Wczytywanie czarnych pionków
	for(i = 8; i < 16; i++)
		figury[i] = new chessMan(modele[4], pola[i-8][6], &tex[2], &tex[3]);
	
	//Wczytywanie bialych wie¿
	figury[16] = new chessMan(modele[5], pola[0][0], &tex[1], &tex[4]);
	figury[17] = new chessMan(modele[5], pola[7][0], &tex[1], &tex[4]);

	//Wczytywanie czarnych wiez
	figury[18]= new chessMan(modele[5], pola[0][7], &tex[2], &tex[3]);
	figury[19]= new chessMan(modele[5], pola[7][7], &tex[2], &tex[3]);

	//Wczytywanie bialych skoczkow
	figury[20]= new chessMan(modele[6], pola[1][0], &tex[1], &tex[4]);
	figury[21]= new chessMan(modele[6], pola[6][0], &tex[1], &tex[4]);

	//Wczytywanie czarnych skoczkow
	figury[22]= new chessMan(modele[7], pola[1][7], &tex[2], &tex[3]);
	figury[23]= new chessMan(modele[7], pola[6][7], &tex[2], &tex[3]);

	//Wczytanie bia³ych goñców
	figury[24]= new chessMan(modele[8], pola[2][0], &tex[1], &tex[4]);
	figury[25]= new chessMan(modele[8], pola[5][0], &tex[1], &tex[4]);

	//Wczytanie czarnych goñców
	figury[26]= new chessMan(modele[8], pola[2][7], &tex[2], &tex[3]);
	figury[27]= new chessMan(modele[8], pola[5][7], &tex[2], &tex[3]);
	
	//Wczytaie bia³ego króla
	figury[28]= new chessMan(modele[9], pola[4][0], &tex[1], &tex[4]);

	//Wczytanie czarnego króla
	figury[29]= new chessMan(modele[9], pola[4][7], &tex[2], &tex[3]);

	//Wczytanie bia³ego hetmana
	figury[30]= new chessMan(modele[10], pola[3][0], &tex[1], &tex[4]);

	//Wczytanie czarnego hetmana
	figury[31]= new chessMan(modele[10], pola[3][7], &tex[2], &tex[3]);
}

//Procedura inicjalizacji pocz¹tkowej szachownicy
void setupChessSet(model *modele[11], GLuint tex[6], board *szachownica[4], field pola[8][8], chessMan *figury[32], chessMan *moving[2], chessMan **beating, ShaderProgram *shaderProgram) {
	setupModels(modele, shaderProgram);
	setupTextures(tex);
	setupBoard(szachownica, modele, tex);
	setupFields(pola);
	setupChessMen(modele, tex, pola, figury);

	//Inicjalizacja wkaŸników
	for(int i = 0; i < 2; i++)
		moving[i] = NULL;
	*beating = NULL;
}
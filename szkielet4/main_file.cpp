#include <iostream>
#include <stdio.h>
#include "texture.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "board.h"
#include "chessMan.h"
#include "loadMoves.h"
#include "moveTypes.h"
#include "init.h"
#include "clean.h"
#include "lights.h"
#include "font.h"

//Namespace'y
using namespace std;
using namespace glm;

//Macierze
glm::mat4 matP; //rzutowania
glm::mat4 matV; //widoku
glm::mat4 fontM; //modelu dla napisów

//Ustawienia okna i rzutowania
int windowPositionX = 100;
int windowPositionY = 100;
int windowWidth = 800;
int windowHeight = 600;
float cameraAngle = 45.0f;

//Zmienne do animacji
float speed_x = 0; 
float speed_y = 0; 
int lastTime = 0;
float angle_x = 0;
float angle_y = 0;
float zoom = 1.0f;

//Uchwyty na shadery
ShaderProgram *shaderProgram; //WskaŸnik na obiekt reprezentuj¹cy program cieniuj¹cy.
ShaderProgram *simple; //Proste cieniowanie na potrzeby odbiæ
ShaderProgram *sfont; //Kolorowanie cznionek

//Uchwyty na tekstury
GLuint tex[6];

//Modele
model *modele[11]; //Tablica modeli
board *szachownica[4]; //Tablica szachownicy (podstawa, pola bia³e/czarne i obwódka)
chessMan *figury[32]; //Tablica pionków
chessMan *moving[2]; //WskaŸniki, które figury siê poruszaj¹ (1 przy normalnym ruchu, 2 przy roszadzie)
chessMan *beating; //WskaŸnik na fugrê, która jest obecnie bita

//Tablica wspó³rzêdnych pól szachownicy
field pola[8][8];

//Ruchy
vector <movements> moves; //Wektor ruchów...
int poz; //...i aktualna pozycja w tym wektorze

//Wektor po³o¿enia kamery
glm::vec3 camera = glm::vec3(0.0f,4.0f,-5.0f);

//WskaŸnik na cznionkê
font *czcionka;

//Procedura rysuj¹ca odbicia obiektów w podstawie szachownicy
void drawReflection() {
	//W³¹czenie programu cieniuj¹cego (proste cieniowanie odbiæ)
	simple->use();

	//Wgranie macierzy widoku i perspektywy
	glUniformMatrix4fv(simple->getUniformLocation("P"), 1, false, glm::value_ptr(matP));
	glUniformMatrix4fv(simple->getUniformLocation("V"), 1, false, glm::value_ptr(matV));

	//Ustawienie samplerów tekstur
	glUniform1i(simple->getUniformLocation("textureMap0"), 0);

	//Przygotowanie do rysowania odbiæ
	glEnable(GL_STENCIL_TEST); //W³¹czenie testowania bufora szablonowego
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //Chwilowe wy³aczenie pisania do buforu kolorów
	glEnable(GL_CULL_FACE); //W³aczenie ukrywania tylnych œcian
	glDepthMask(GL_FALSE); //Chwilowe wy³aczenie pisania do Z-buforu

	//Ustawienie funkcji dla buforu szabloniowego
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//Wyczyszczenie bufora szablonowego
	//glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Szachownica w buforze (przyciêcie odbiæ)
	szachownica[0]->draw(simple, 1.0f, true);

	//Ponowne w³¹czenie pisania do buforu kolorów i Z-buforu
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	//Ustawienie nowej funckji buforu szablonowego dla odbiæ pionków
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	//glStencilMask(0x00);

	//Odbicia pionków
	int i;
	for(i = 0; i < 32; i++)
		if(!figury[i]->isBeated())
			figury[i]->draw(simple, true);

	glDisable(GL_STENCIL_TEST); //Wy³aczenie testowania bufora szablonowego
	glDisable(GL_CULL_FACE); //Wy³¹czenie ukrywania tylnych œcian

	//Posprz¹tanie po sobie
	glBindVertexArray(0);
}

//Procedura rysuj¹ca obiekty
void drawObject() {
	//W³¹czenie programu cieniuj¹cego (w³aœciwe cieniowanie dla obiektów sceny)
	shaderProgram->use();

	//Wgranie macierzy widoku i perspektywy
	glUniformMatrix4fv(shaderProgram->getUniformLocation("P"), 1, false, glm::value_ptr(matP));
	glUniformMatrix4fv(shaderProgram->getUniformLocation("V"), 1, false, glm::value_ptr(matV));
	
	//Ustawienie Ÿróde³ œwiat³a
	glUniform4fv(shaderProgram->getUniformLocation("lpoz"), 4, lights);

	//Ustawienie samplerów tekstur
	glUniform1i(shaderProgram->getUniformLocation("textureMap0"), 0);
	glUniform1i(shaderProgram->getUniformLocation("textureMap1"), 1);

	//Rysowanie szachownicy
	glEnable(GL_BLEND); //W³aczenie mieszania kolorów
	glEnable(GL_CULL_FACE); //W³aczenie ukrywania tylnych œcian
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Ustawienie funkcji mieszania kolorów
	szachownica[0]->draw(shaderProgram, 0.8f, false); //Rysowanie czêœciowo przeŸroczystej podstawy
	//Rysowanie pozosta³ej czêœci szachownicy
	int i;
	for(i = 1; i < 4; i++)
		szachownica[i]->draw(shaderProgram, 1.0f, false);
	glDisable(GL_BLEND); //Wy³¹czenie mieszania kolorów
	glDisable(GL_CULL_FACE); //Wy³¹czenie ukrywania tylnych œcian

	//Rysowanie figur (jeœli nie jest zbita/bita)
	for(i = 0; i < 32; i++)
		if(!figury[i]->isBeated())
			figury[i]->draw(shaderProgram, false);

	//Rysowanie figury aktualnie bitej z efektem stopniowego zanikania (stopniowa dekrementacja wartoœci kana³u alfa)
	if(beating != NULL) {
		glEnable(GL_BLEND); //Aktywacja mieszania kolorów
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Ustawienie funkcji mieszania kolorów
		glDepthMask(GL_FALSE); //Chwilowe wy³aczenie pisania do Z-bufora
		beating->draw(shaderProgram, false); //Narysuj zbijan¹ figurê
		glDisable(GL_BLEND); //Dezaktywuj mieszanie kolorów
		glDepthMask(GL_TRUE); //Ponownie w³acz pisanie do Z-bufora
	}
	
	//Posprz¹tanie po sobie
	glBindVertexArray(0);
}

//Procedura rysuj¹ca napisy
void drawText(const char* napis) {
	//W³¹czenie programu cieniuj¹cego (kolorowanie cznionek)
	sfont->use();

	//Wgranie macierzy modelu, widoku i perspektywy
	glUniformMatrix4fv(sfont->getUniformLocation("M"), 1, false, glm::value_ptr(fontM));
	glUniformMatrix4fv(sfont->getUniformLocation("P"), 1, false, glm::value_ptr(matP));
	glUniformMatrix4fv(sfont->getUniformLocation("V"), 1, false, glm::value_ptr(matV));

	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);

	glEnable(GL_BLEND); //Aktywacja mieszania kolorów
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Ustawienie funkcji mieszania kolorów
	glDepthMask(GL_FALSE); //Chwilowe wy³aczenie pisania do Z-bufora

	czcionka->draw(napis, -1 + 8*sx, 1-50*sy, sx, sy, sfont); //Narysuj napis

	glDisable(GL_BLEND); //Dezaktywuj mieszanie kolorów
	glDepthMask(GL_TRUE); //Ponownie w³acz pisanie do Z-bufora
}

//Procedura rysuj¹ca
void displayFrame() {
	//Wyczyœæ bufor kolorów i bufor g³êbokoœci
	glClearColor(0.7,0.7,0.7,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Wylicz macierz rzutowania
	matP = perspective(cameraAngle*zoom, (float)windowWidth/(float)windowHeight, 1.0f, 100.0f);
	
	//Wylicz macierz widoku
	matV = lookAt(camera, vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f));
	//Obrót kamery
	matV = rotate(matV, angle_x, vec3(1,0,0)); 
	matV = rotate(matV, angle_y, vec3(0,1,0));

	//Wyliczenie macierzy modelu dla napisów
	fontM = scale(mat4(1.0f), vec3(-1,1,1));
	//Obrót napisu
	fontM = rotate(fontM, -angle_x, vec3(1,0,0));
	fontM = rotate(fontM, angle_y, vec3(0,1,0));
	
	//Narysuj scenê
	drawReflection(); //Narysuj odbicia
	drawObject(); //Narysuj w³aœciwe obiekty sceny

	//Rysowanie napisów
	if ((poz > -1) && (poz <= (moves.size() - 1))) {
		if(moves.at(poz).mat == 1)
			drawText("Checkmate!");
		else if(moves.at(poz).szach == 1)
			drawText("Check!");
		if (moves.at(poz).fromx == -1)
			drawText("White wins!");
		else if (moves.at(poz).fromx == -2)
			drawText("Black wins!");
		else if (moves.at(poz).fromx == -3)
			drawText("A draw!");
	}
	
	//Tylny bufor na przedni
	glutSwapBuffers();
}

//Animacja
void nextFrame(void) {
	//Oblicz czas, jaki up³yn¹³
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	int interval = actTime-lastTime;
	lastTime = actTime;
	//Oblicz k¹t kamery
	angle_x += speed_x*interval/1000.0;
	angle_y += speed_y*interval/1000.0;
	if (angle_x > 360) angle_x -= 360;
	if (angle_y > 360) angle_y -= 360;
	if (angle_x < 0) angle_x += 360;
	if (angle_y < 0) angle_y += 360;
	//Wykonaj ruchy figur
	for(int i = 0; i < 2; i++)
		if(moving[i] != NULL) {
			if(moving[i]->move()) {
				moving[i] = NULL;
				beating = NULL;
			}
			if(beating != NULL)
				beating->decAlpha();
		}

	glutPostRedisplay();
}

//Reakcja na wciœniêcie klawisza
void keyDown(int c, int x, int y) {
	switch (c) {
	case GLUT_KEY_LEFT:
		speed_y = -120;
		break;
	case GLUT_KEY_RIGHT:
		speed_y = 120;
		break;
	case GLUT_KEY_UP:
		speed_x = 120;
		break;
	case GLUT_KEY_DOWN:
		speed_x = -120;
		break;
	case GLUT_KEY_PAGE_UP:
		zoom -= 0.05;
		break;
	case GLUT_KEY_PAGE_DOWN:
		zoom += 0.05;
		break;
	case GLUT_KEY_HOME:
		if(moving[0] == NULL && moving[1] == NULL) {
			poz++;
			if(poz < (moves.size() - 1)) {
				if(moves.at(poz).roszada == 0) {
					normalMove(figury, moving, &beating, pola, modele, moves.at(poz)); //Normalny ruch
				} else
					castling(figury, moving, pola, moves.at(poz)); //Roszada
			} else if(poz == (moves.size() - 1)) std::cout << "Koniec pliku" << endl;
		}
		break;
	}
}

//Reakcja na puszczenie klawisza
void keyUp(int c, int x, int y) {
	switch (c) {
	case GLUT_KEY_LEFT:
		speed_y = 0;
		break;
	case GLUT_KEY_RIGHT:
		speed_y = 0;
		break;
	case GLUT_KEY_UP:
		speed_x = 0;
		break;
	case GLUT_KEY_DOWN:
		speed_x = 0;
		break;
	}
}

//Procedura wywo³ywana przy zmianie rozmiaru okna
void changeSize(int w, int h) {
	//Ustawienie wymiarow przestrzeni okna
	glViewport(0,0,w,h);
	//Zapamiêtanie nowych wymiarów okna dla poprawnego wyliczania macierzy rzutowania
	windowWidth = w;
	windowHeight = h;
}

//Procedura inicjuj¹ca biblotekê glut
void initGLUT(int *argc, char** argv) {
	glutInit(argc, argv); //Zainicjuj bibliotekê GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_STENCIL); //Alokuj bufory kolorów (podwójne buforowanie), bufor kolorów i MSAA
	
	glutInitWindowPosition(windowPositionX, windowPositionY); //Wska¿ pocz¹tkow¹ pozycjê okna
	glutInitWindowSize(windowWidth, windowHeight); //Wska¿ pocz¹tkowy rozmiar okna
	glutCreateWindow("Szach MAT - odtwarzacz partii szachowej w 3D"); //Utwórz okno i nadaj mu tytu³
	
	glutReshapeFunc(changeSize); //Zarejestruj procedurê changeSize jako procedurê obs³uguj¹ca zmianê rozmiaru okna
	glutDisplayFunc(displayFrame); //Zarejestruj procedurê displayFrame jako procedurê obs³uguj¹ca odœwierzanie okna
	glutIdleFunc(nextFrame); //Zarejestruj procedurê nextFrame jako procedurê wywo³ywan¹ najczêœciêj jak siê da (animacja)

	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
}

//Procedura inicjuj¹ca bibliotekê glew
void initGLEW() {
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr,"%s\n", glewGetErrorString(err));
		exit(1);
	}
}

//Procedura inicjuj¹ca ró¿ne sprawy zwi¹zane z rysowaniem w OpenGL
void initOpenGL() {
	cout << "Shadery" << endl << "--------" << endl;
	setupShaders(&shaderProgram, &simple, &sfont);
	cout << endl << "Modele" << endl << "--------" << endl;
	setupChessSet(modele, tex, szachownica, pola, figury, moving, &beating, shaderProgram);
	cout << "FreeType" << endl << "--------" << endl;
	czcionka = new font("fonts\\ASansrounded.ttf");

	glEnable(GL_DEPTH_TEST); //W³¹cz Z-bufor
	glEnable(GL_MULTISAMPLE_ARB); //W³¹cz MSAA;
}

//Procedura zwalniaj¹ca zasoby programu
void clean() {
	cleanModels(modele);
	cleanBoard(szachownica);
	cleanChesseMen(figury);
	cleanTextures(tex);
	moves.clear();
	delete czcionka;
	delete shaderProgram;
	delete simple;
	delete sfont;
}

int main(int argc, char** argv) {
	//Init
	initGLUT(&argc, argv);
	initGLEW();
	initOpenGL();
	loadFile(moves);
	poz = -1;
	
	//Main loop
	glutMainLoop();
	
	//Free
	clean();	
	return 0;
}

#include "texture.h"

GLuint readTexture(char* filename) {
	GLuint tex;
	TGAImg img;
	glActiveTexture(GL_TEXTURE0);
	if (img.Load(filename)==IMG_OK) {
		glGenTextures(1,&tex); //Zainicjuj uchwyt tex
		glBindTexture(GL_TEXTURE_2D,tex); //Przetwarzaj uchwyt tex
		if (img.GetBPP()==24) //Obrazek 24bit
			glTexImage2D(GL_TEXTURE_2D,0,3,img.GetWidth(),img.GetHeight(),0,
			GL_RGB,GL_UNSIGNED_BYTE,img.GetImg());
		else if (img.GetBPP()==32) //Obrazek 32bit
			glTexImage2D(GL_TEXTURE_2D,0,4,img.GetWidth(),img.GetHeight(),0,
			GL_RGBA,GL_UNSIGNED_BYTE,img.GetImg());      
		else {
			printf("Nieobs³ugiwany format obrazka w pliku: %s \n",filename);
		}
	} else {
		printf("B³¹d przy wczytywaniu pliku: %s\n",filename);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	return tex;
}
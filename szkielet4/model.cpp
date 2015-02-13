#include "model.h"
#include <iostream>

using namespace std;

//Wczytanie pliku .obj z modelem (gotowa funkcja)
void load(char* fileName, vector<glm::vec3> &vertices, vector<glm::vec2> &texCoords, vector<glm::vec3> &normals) {
	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices; 
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;


	FILE * file = fopen(fileName, "r");
	if( file == NULL ){
		cout << "Nie mozna otworzyc pliku!" << endl;
		getchar();
	}

	while( 1 ){

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		
		if (strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		} else if (strcmp( lineHeader, "vt" ) == 0 ) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		} else if (strcmp( lineHeader, "vn" ) == 0 ) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		} else if ( strcmp( lineHeader, "f" ) == 0 ) {
			string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				cout << "Nie mozna odczytac pliku!" << endl;
		}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		} else {
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	for (unsigned int i=0; i<vertexIndices.size(); i++) {

		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];
		
		vertices.push_back(vertex);
		texCoords.push_back(uv);
		normals.push_back(normal);
	
	}
}

//Tworzenie obiektu typu VAO
void assignVBOtoAttribute(ShaderProgram *shaderProgram, char* attributeName, GLuint bufVBO, int variableSize) {
	GLuint location = shaderProgram -> getAttribLocation(attributeName); //Pobierz numery slotów dla atrybutu
	glBindBuffer(GL_ARRAY_BUFFER, bufVBO); //Uaktywnij uchwyt VBO 
	glEnableVertexAttribArray(location); //W³¹cz u¿ywanie atrybutu o numerze slotu zapisanym w zmiennej location
	glVertexAttribPointer(location, variableSize, GL_FLOAT, GL_FALSE, 0, NULL); //Dane do slotu location maj¹ byæ brane z aktywnego VBO
}

//Tworzenie buforu VBO dla wierzcho³ków i normalnych
GLuint makeBuffer(vector <glm::vec3> data, int vertexSize) {
	GLuint handle;
	
	glGenBuffers(1, &handle); //Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle);  //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, data.size()*vertexSize, &data[0], GL_STATIC_DRAW); //Wgraj tablicê do VBO
	
	return handle;
}

//Tworzenie buforu VBO dla koordynatów teksturowania
GLuint makeBuffer(vector <glm::vec2> data, int vertexSize) {
	GLuint handle;
	
	glGenBuffers(1, &handle); //Wygeneruj uchwyt na Vertex Buffer Object (VBO), który bêdzie zawiera³ tablicê danych
	glBindBuffer(GL_ARRAY_BUFFER, handle); //Uaktywnij wygenerowany uchwyt VBO 
	glBufferData(GL_ARRAY_BUFFER, data.size()*vertexSize, &data[0], GL_STATIC_DRAW); //Wgraj tablicê do VBO
	
	return handle;
}

//Procedura tworz¹ca bufory VBO
void setupVBO(model &m, vector <glm::vec3> vertices, vector <glm::vec2> texCoords, vector <glm::vec3> normals) {
	m.bufVertices = makeBuffer(vertices, sizeof(glm::vec3)); //Wspó³rzêdne wierzcho³ków
	m.bufNormals = makeBuffer(normals, sizeof(glm::vec3)); //Normalne wierzcho³ków
	m.bufTex = makeBuffer(texCoords, sizeof(glm::vec2)); //Koordynaty teksturowania
}

//Konstruktor
model::model(char* fileName, ShaderProgram *shaderProgram) {
	vector<glm::vec3> vertices;
	vector<glm::vec2> texCoords;
	vector<glm::vec3> normals;
	
	//£adowanie .obj
	cout << "Ladowanie pliku " << fileName << "..." << endl;
	load(fileName, vertices, texCoords, normals);
	cout << "Wczytanie modelu zakonczone sukcesem!" << endl;

	//Tworzenie 3xVBO
	cout << "Tworzenie VBO..." << endl;
	setupVBO(*this, vertices, texCoords, normals);
	cout << "Tworzenie buforow VBO zakonczone sukcesem!" << endl;

	//Tworzenie 1xVAO
	cout << "Tworzenie VAO..." << endl;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	assignVBOtoAttribute(shaderProgram, "vertex", bufVertices, 3);
	assignVBOtoAttribute(shaderProgram, "normal", bufNormals, 3);
	assignVBOtoAttribute(shaderProgram, "texCoord", bufTex, 2);
	cout << "Tworzenie VAO zakonczone sukcesem!" << endl;

	//Zapamiêtanie liczby wierzcho³ków
	indicesSize = vertices.size();
	
	cout << "Model wyczytany!" << endl << endl;
}

//Destruktor
model::~model() {
	//Free VBO
	glDeleteBuffers(1, &bufVertices);
	glDeleteBuffers(1, &bufNormals);
	glDeleteBuffers(1, &bufTex);

	//Free VAO
	glDeleteVertexArrays(1, &vao);
}
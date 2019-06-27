/* mesh.h
This class is taken directly from https://learnopengl.com/Model-Loading/Mesh
stores data for a mesh from an imported model.

*/

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader.h"

using namespace std;

//Struct defenitions
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
	void Draw(Shader shader);
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};
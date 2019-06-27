/* model.h
This class is taken directly from https://learnopengl.com/Model-Loading/Model
this class can import a 3d model using assimp and stores as mesh objects

*/

#pragma once

#include "shader.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL.h>

class Model
{
public:
	/*  Functions   */
	void loadModel(string path);
	void Draw(Shader shader);

private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	/*  Functions   */
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory);
};
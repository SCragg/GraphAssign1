/* box.h

based off of the cube object given to us in class, however with more vertices,
all vertices specified manually.
*/

#pragma once

#include "shader.h"
#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>
#include <iostream>

#include "SOIL.h"


class Box
{
public:
	Box();
	~Box();

	void makeBox(Shader shader);
	void drawBox(Shader shader);

	int numvertices;
	int drawmode;

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint vao;

	GLuint positionBufferObject;
	GLuint colourObject;
	GLuint normalsBufferObject;
	GLuint textureBufferObject;
	GLuint textures[3];

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;
	GLuint attribute_v_textures;
};


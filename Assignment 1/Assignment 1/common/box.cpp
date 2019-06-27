/* box.cpp
This class is based off of the cube class given to us in the examples however I added more vertices
I could have made this shape easily by using two cubes and scaling, however I wanted to add
textures and did not want to scale the textures by doing this.
*/

#include "box.h"

using namespace std;

/* Define the vertex attributes for vertex positions and normals.
These will send attributes to the shader
You might also want to add colours and texture coordinates */
Box::Box()
{
	attribute_v_coord = 0;
	attribute_v_normal = 1;
	attribute_v_textures = 2;
	attribute_v_colours = 3;
	numvertices = 28;
}


Box::~Box()
{
}


/* Make a box from hard-coded vertex positions and normals  */
void Box::makeBox(Shader shader)
{
	/* Define vertices for the box in 12 triangles */
	GLfloat vertexPositions[] =
	{
		//top segment
		-0.25f, 0.1f, -0.25f,
		-0.25f, -0.15f, -0.25f,
		0.25f, -0.15f, -0.25f,

		0.25f, -0.15f, -0.25f,
		0.25f, 0.1f, -0.25f,
		-0.25f, 0.1f, -0.25f,

		0.25f, -0.15f, -0.25f,
		0.25f, -0.15f, 0.25f,
		0.25f, 0.1f, -0.25f,

		0.25f, -0.15f, 0.25f,
		0.25f, 0.1f, 0.25f,
		0.25f, 0.1f, -0.25f,

		0.25f, -0.15f, 0.25f,
		-0.25f, -0.15f, 0.25f,
		0.25f, 0.1f, 0.25f,

		-0.25f, -0.15f, 0.25f,
		-0.25f, 0.1f, 0.25f,
		0.25f, 0.1f, 0.25f,

		-0.25f, -0.15f, 0.25f,
		-0.25f, -0.15f, -0.25f,
		-0.25f, 0.1f, 0.25f,

		-0.25f, -0.15f, -0.25f,
		-0.25f, 0.1f, -0.25f,
		-0.25f, 0.1f, 0.25f,

		-0.25f, 0.1f, -0.25f,
		0.25f, 0.1f, -0.25f,
		0.25f, 0.1f, 0.25f,

		0.25f, 0.1f, 0.25f,
		-0.25f, 0.1f, 0.25f,
		-0.25f, 0.1f, -0.25f,

		//Bottom segment
		-0.30f, -0.25f, 0.30f,
		0.30f, -0.25f, 0.30f,
		0.30f, -0.25f, -0.30f,

		0.30f, -0.25f, -0.30f,
		-0.30f, -0.25f, -0.30f,
		-0.30f, -0.25f, 0.30f,

		0.30f, -0.25f, -0.30f,
		0.30f, -0.25f, 0.30f,
		0.30f, -0.15f, -0.30f,

		0.30f, -0.25f, 0.30f,
		0.30f, -0.15f, 0.30f,
		0.30f, -0.15f, -0.30f,

		-0.30f, -0.25f, 0.30f,
		-0.30f, -0.25f, -0.30f,
		-0.30f, -0.15f, 0.30f,

		-0.30f, -0.25f, -0.30f,
		-0.30f, -0.15f, -0.30f,
		-0.30f, -0.15f, 0.30f,

		-0.30f, -0.15f, -0.30f,
		-0.30f, -0.25f, -0.30f,
		0.30f, -0.25f, -0.30f,

		0.30f, -0.25f, -0.30f,
		0.30f, -0.15f, -0.30f,
		-0.30f,-0.15f, -0.30f,

		0.30f, -0.25f, 0.30f,
		-0.30f, -0.25f, 0.30f,
		0.30f, -0.15f, 0.30f,

		-0.30f, -0.25f, 0.30f,
		-0.30f, -0.15f, 0.30f,
		0.30f, -0.15f, 0.30f,

		//top faces of second segment
		-0.25f, -0.15f, 0.25f,
		0.25f, -0.15f, 0.25f,
		-0.30f, -0.15f, 0.30f,

		0.30f, -0.15f, 0.30f,
		-0.30f, -0.15f, 0.30f,
		0.25f, -0.15f, 0.25f,

		0.25f, -0.15f, 0.25f,
		0.25f, -0.15f, -0.25f,
		0.30f, -0.15f, 0.30f,

		0.30f, -0.15f, -0.30f,
		0.30f, -0.15f, 0.30f,
		0.25f, -0.15f, -0.25f,

		0.25f, -0.15f, -0.25f,
		-0.25f, -0.15f, -0.25f,
		0.30f, -0.15f, -0.30f,

		-0.30f, -0.15f, -0.30f,
		0.30f, -0.15f, -0.30f,
		-0.25f, -0.15f, -0.25f,

		-0.25f, -0.15f, -0.25f,
		-0.25f, -0.15f, 0.25f,
		-0.30f, -0.15f, -0.30f,

		-0.30f, -0.15f, 0.30f,
		-0.30f, -0.15f, -0.30f,
		-0.25f, -0.15f, 0.25f,
	};

	/* Manually specified colours for our box, brown all over, however we will be using a texture */
	GLfloat vertexColours[] = {
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,

		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
		0.36f, 0.25f, 0.20f, 1.0f,
	};

	/* Manually specified normals*/
	
	GLfloat normals[] =
	{
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,

		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,

		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,

		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,

		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		0, -1.f, 0,
		0, -1.f, 0,
		0, -1.f, 0,
		0, -1.f, 0,
		0, -1.f, 0,
		0, -1.f, 0,

		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,
		1.f, 0, 0,

		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,
		-1.f, 0, 0,

		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,
		0, 0, -1.f,

		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,
		0, 0, 1.f,

		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
	};
	

	/*Manually specify texture coordinates*/
	
	GLfloat texturecoords[] =
	{
		0.5f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.5f, 1.0f,
		0.5f, 0.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.5f, 0.0f,
		0.5f, 1.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.5f, 0.0f,
		0.5f, 1.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.5f, 0.0f,
		0.5f, 1.0f,

		0.5f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.5f, 1.0f,
		0.5f, 0.0f,

		0.5f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.5f, 1.0f,
		0.5f, 0.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.7f, 1.0f,
		1.0f, 0.0f,
		0.7f, 0.0f,
		0.7f, 1.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.7f, 1.0f,
		1.0f, 0.0f,
		0.7f, 0.0f,
		0.7f, 1.0f,

		0.7f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.7f, 1.0f,
		0.7f, 0.0f,

		1.0f, 1.0f,
		1.0f, 0.0f,
		0.7f, 1.0f,
		1.0f, 0.0f,
		0.7f, 0.0f,
		0.7f, 1.0f,

		0.0f, 0.0833f,
		0.0f, 0.9166f,
		0.0166f, 0.0f,
		0.0166f, 1.0f,
		0.0166f, 0.0f,
		0.0f, 0.9166f,

		0.0f, 0.0833f,
		0.0f, 0.9166f,
		0.0166f, 0.0f,
		0.0166f, 1.0f,
		0.0166f, 0.0f,
		0.0f, 0.9166f,

		0.0f, 0.0833f,
		0.0f, 0.9166f,
		0.0166f, 0.0f,
		0.0166f, 1.0f,
		0.0166f, 0.0f,
		0.0f, 0.9166f,

		0.0f, 0.0833f,
		0.0f, 0.9166f,
		0.0166f, 0.0f,
		0.0166f, 1.0f,
		0.0166f, 0.0f,
		0.0f, 0.9166f,
	};
	

	//Create vertex array
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	/* Create the vertex buffer for the cube */
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the colours buffer for the cube */
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the cube */
	glGenBuffers(1, &normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	/* Create the texture buffer for the cube */
	glGenBuffers(1, &textureBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, textureBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturecoords), texturecoords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*Textures
	The code for the textures I reasearched from https://learnopengl.com/Getting-started/Textures
	and https://open.gl/textures I implemented bits of both pages to get my textures to work.
	I added two textures to this object one used as a diffuse texture and one as a specular map
	The idea for using a specular map came from https://learnopengl.com/Lighting/Lighting-maps
	and was very simple to implement in the frgament shader.
	*/
	shader.use();
	glGenTextures(2, textures);
	//load and generate the textures
	int width, height, nrChannels;
	unsigned char* data;

	//texture 1
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//Using the error if not loaded comes from https://learnopengl.com/Getting-started/Textures
	data = SOIL_load_image("Textures/wood_diff.jpg", &width, &height, &nrChannels, SOIL_LOAD_AUTO);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	SOIL_free_image_data(data);

	//Texture wrapping and filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Texture 2
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	data = SOIL_load_image("Textures/wood_specu.jpg", &width, &height, &nrChannels, SOIL_LOAD_AUTO);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	SOIL_free_image_data(data);

	//Texture wrapping and filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	/* Texture 3 - This is actually the specular map for the record, I put it here as a quick fix to a problem of getting the 
	correct texture to apply to the record, but it is a horrible way to fix the problem */
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	data = SOIL_load_image("Textures/record_2_spec.png", &width, &height, &nrChannels, SOIL_LOAD_AUTO);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	SOIL_free_image_data(data);

	//Texture wrapping and filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}


/* Draw the cube by bining the VBOs and drawing triangles */
void Box::drawBox(Shader shader)
{
	glBindVertexArray(this->vao);

	/* Bind box vertices. */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(attribute_v_coord);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind colours */
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(attribute_v_colours);
	glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind normals */
	glEnableVertexAttribArray(attribute_v_normal);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//Bind texture coords
	glEnableVertexAttribArray(attribute_v_textures);
	glBindBuffer(GL_ARRAY_BUFFER, textureBufferObject);
	glVertexAttribPointer(attribute_v_textures, 2, GL_FLOAT, GL_FALSE, 0, 0);
	
	//Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glUniform1i(glGetUniformLocation(shader.ID, "material.texture_diffuse1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glUniform1i(glGetUniformLocation(shader.ID, "material.texture_specular1"), 1);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numvertices * 3);

	//Unbind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// ** Horrible solution to fix the record texture problem **
	/* Binding the specular record texture here as it will not bind in the draw function in the mesh class,
	I did not have time to fix the problem properly, however I still wanted the texture to be applied as thought it looks neat.*/
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glUniform1i(glGetUniformLocation(shader.ID, "material.texture_specular1"), 1);

	glBindVertexArray(0);
}
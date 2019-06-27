/*
Assignment 1.

I used the lab 3 start file as a starting point for this project however bits were added as we progressed such as using
the positional light and lighting the fragment shader.
*/

//Linking to libraries - from lab examples
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "fmodL_vc.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#pragma comment(lib, "fmod_vc.lib")	

#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include <stack>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

// Include headers for our objects
#include "shader.h"
#include "SimpleAudioManager.h"
#include "box.h"
#include "cube.h"
#include "model.h"


//Variable Declarations
/* Position and view globals */
GLfloat angle_x, angle_inc_x, model_scale; //from lab example
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z; //from lab example
GLfloat move_x, move_y, move_z;

//View Globals
GLfloat aspect_ratio;		// Aspect ratio of the window defined in the reshape callback - from lab example
GLfloat lightx, lighty, lightz; //Light position values from positional light lab

//Model globals
GLfloat needle_rotation, arm_rotation, handle_rotation, platter_rotation;
GLfloat handle_inc, platter_inc, platter_speed, handle_speed;

//Global flags
GLboolean is_handle_spinning;
GLboolean is_platter_spinning;
GLboolean is_needle_down;
bool is_audio_paused = false;
bool is_audio_playing = false;

//Animation Constants
GLfloat const ARM_ROTATION_MAX = 1.0;
GLfloat const ARM_ROTATION_MIN = -19.0;
GLfloat const ARM_STANDARD_ROTATION = 0.001;
GLfloat const PLATTER_STANDARD_SPEED = -2;
GLfloat const HANDLE_STANDARD_SPEED = 1.5;

/* Uniforms*/
GLuint modelID, viewID, projectionID, normalmatrixID, lightposID, diffuseID, shininessID, is_model_texturedID;

/* Global instances of our objects */
Shader aShader;
SimpleAudioManager AudioPlayer;
Box aBox;
Cube aCube;
Model Cone;
Model ToneArm;
Model Needle;
Model Handle;
Model Platter;

using namespace std;
using namespace glm;

/*
This function is called before entering the main rendering loop. Initialisations.
*/
void init(GLWrapper *glw)
{
	/* Set the view transformation controls to their initial values -from lab exaplmes */
	angle_x = -5;
	angle_y = 130;
	angle_z = 0;
	angle_inc_x = angle_inc_y = angle_inc_z = 0;
	move_x = move_y = move_z = 0;

	model_scale = 1.f;
	aspect_ratio = 1024.f / 768.f;	// Initial aspect ratio from window size - from lab examples

	/*set model controls values to initial position */
	//rotations:
	needle_rotation = -180;
	arm_rotation = ARM_ROTATION_MAX;
	handle_rotation = 0;
	platter_rotation = 0;
	//rotation increments:
	handle_inc = 0;
	platter_inc = 0;
	//light position values
	lightx = 0.5;
	lighty = 0.5;
	lightz = 0.5;
	//speeds
	platter_speed = 1;
	handle_speed = 1;

	//Set flags to initial values	
	is_handle_spinning = FALSE;
	is_platter_spinning = FALSE;
	is_needle_down = FALSE;

	/* Load shaders in to shader object */
	
	try
	{
		aShader.LoadShader("..\\..\\shaders\\Assign1.vert", "..\\..\\shaders\\Assign1.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	//Load our audio
	AudioPlayer.Stream("dont_fence_me_in.wav");
	//Play and straight away pause it
	AudioPlayer.Play("dont_fence_me_in.wav");
	AudioPlayer.Pause();

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(aShader.ID, "model");
	viewID = glGetUniformLocation(aShader.ID, "view");
	projectionID = glGetUniformLocation(aShader.ID, "projection");
	lightposID = glGetUniformLocation(aShader.ID, "lightpos");
	normalmatrixID = glGetUniformLocation(aShader.ID, "normalmatrix");
	diffuseID = glGetUniformLocation(aShader.ID, "material.diffuse_col");
	shininessID = glGetUniformLocation(aShader.ID, "material.shininess");
	is_model_texturedID = glGetUniformLocation(aShader.ID, "is_model_textured");

	/* create cube object */
	aCube.makeCube();
	aBox.makeBox(aShader);
	//Load our imported models
	Cone.loadModel("Models/Cone.obj");
	ToneArm.loadModel("Models/ToneArm.obj");
	Needle.loadModel("Models/Needle.obj");
	Handle.loadModel("Models/Handle.obj");
	Platter.loadModel("Models/Platter.obj");

	//Display Instructions
	cout << "CONTROLS:" << endl << endl << "Viewing:" << endl <<
		"Rotate model - x axis: Q, W" << endl << "Rotate model - y axis: E, R" << endl <<
		"Rotate model - z axis: T, Y" << endl << "Scale model: A, S" << endl << "Move camera: Arrow Keys, Right Shift, Right Control" <<
		endl << endl << "Gramophone controls:" << endl << "Turn Handle (on/off): B" << endl << "Spin Platter (on/off): G" << endl <<
		"Raise/lower needle: Space bar" << endl << "Change handle speed: C, V" << endl << "Change platter speed: F, G" << endl <<
		"Move tone-arm: Z, X" << endl << endl << "Lighting" << endl << "Move light - x axis: O, P" << endl << "Move light - y axis: K, L" << endl << "Move light - z axis: N, M" <<
		endl << endl << "I recommend that you have your sound turned on as there is audio.";

}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	//Use our shader
	aShader.use();

	// Projection matrix : 30° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units - from lab example
	mat4 projection = perspective(radians(30.0f), aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	mat4 view = lookAt(
		vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		vec3(0, 0, 0), // looks at the origin
		vec3(0, 1, 0)  // up is positive y.
	);

	view = translate(view, vec3(move_x, move_y, move_z));

	// Define the light position and transform by the view matrix - from lab example
	vec4 lightpos = view * vec4(lightx, lighty, lightz, 1.0);

	// Send our projection and view uniforms and light position to the shader
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
	glUniform4fv(lightposID, 1, value_ptr(lightpos));

	//Declare material variables to send to shaders as a uniform
	//Used to modify shininess and colour for each object as obj does not store vertex colour data
	GLint shininess;
	vec4 diffuse;
	GLuint is_model_textured; //tells shader if model is textured or not

	// Define our model transformation in a stack and 
	// push the identity matrix onto the stack
	stack<mat4> model;
	model.push(mat4(1.0f));

	// Define the normal matrix
	mat3 normalmatrix;

	//controls the light cube
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(lightx, lighty, lightz));
		model.top() = scale(model.top(), vec3(0.05f, 0.05f, 0.05f));

		//Set material values
		diffuse = vec4(1, 1, 1, 1);
		is_model_textured = 0;
		//Send matrices and draw cube
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
		glUniform4fv(diffuseID, 1, value_ptr(diffuse));
		glUniform1ui(is_model_texturedID, is_model_textured);
		aCube.drawCube(aShader);
	}
	model.pop();

	// Define our transformations that apply to all our objects expecpt light cube
	model.top() = translate(model.top(), vec3(0, -0.3, 0));
	model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));//scale equally in all axis
	model.top() = rotate(model.top(), -radians(angle_x), vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model.top() = rotate(model.top(), -radians(angle_y), vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model.top() = rotate(model.top(), -radians(angle_z), vec3(0, 0, 1)); //rotating in clockwise direction around z-axis
	
	//Push current model on to the top of the stack
	//This block will control the scale and translation of the imported models as they are
	//makes it easier to position them all together as one block
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(0, 0, 0));
		model.top() = scale(model.top(), vec3(0.2, 0.2, 0.2));

		//This block controls rotation for the tone arm and needle
		model.push(model.top());
		{
			model.top() = translate(model.top(), vec3(-1.55, 0.75, -0.05)); // Values just worked out after trial and error to make them fit
			model.top() = rotate(model.top(), -radians(arm_rotation), vec3(0, 1, 0)); //rotating around y-axis

			//Set material values
			shininess = 5;
			diffuse = vec4(0.52, 0.45, 0.1, 1);
			is_model_textured = 0;

			//Send uniforms and Draw ToneArm
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
			normalmatrix = transpose(inverse(mat3(view * model.top())));
			glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
			glUniform1i(shininessID, shininess);
			glUniform4fv(diffuseID, 1, value_ptr(diffuse));
			glUniform1ui(is_model_texturedID, is_model_textured);
			ToneArm.Draw(aShader);

			//This Block works out rotation for the needle. (Up/down)
			model.push(model.top());
			{
				model.top() = translate(model.top(), vec3(1.175, -0.295, 0.58)); // Values just worked out after trial and error to make them fit
				model.top() = rotate(model.top(), -radians(needle_rotation), vec3(0, 0, 1)); //rotating around z-axis

				//Set material values
				shininess = 5;
				diffuse = vec4(0.52, 0.52, 0.52, 1);
				is_model_textured = 0;

				//Send uniforms and draw Needle
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
				normalmatrix = transpose(inverse(mat3(view * model.top())));
				glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
				glUniform1i(shininessID, shininess);
				glUniform4fv(diffuseID, 1, value_ptr(diffuse));
				glUniform1ui(is_model_texturedID, is_model_textured);
				Needle.Draw(aShader);
			}
			model.pop();
		}
		model.pop();

		//This controls movement for the handle
		model.push(model.top());
		{
			model.top() = translate(model.top(), vec3(0, -0.61f, -1.50f)); //Hard coded translation values
			model.top() = rotate(model.top(), -radians(handle_rotation), vec3(0, 0, 1)); //rotating around z-axis

			//Set material values
			shininess = 8;
			diffuse = vec4(0.62, 0.62, 0.62, 1);
			is_model_textured = 0;


			//Send uniforms and draw Handle
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
			normalmatrix = transpose(inverse(mat3(view * model.top())));
			glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
			glUniform1i(shininessID, shininess);
			glUniform4fv(diffuseID, 1, value_ptr(diffuse));
			glUniform1ui(is_model_texturedID, is_model_textured);
			Handle.Draw(aShader);
		}
		model.pop();

		//This controls movement for the platter
		model.push(model.top());
		{
			model.top() = translate(model.top(), vec3(0.0, -0.04, 0.0));
			model.top() = rotate(model.top(), -radians(platter_rotation), vec3(0, 1, 0)); //rotating around y-axis

			//Set material values
			shininess = 30;
			diffuse = vec4(0.1, 0.1, 0.1, 1);
			is_model_textured = 1;

			//Draw Platter
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
			// Recalculate the normal matrix and send to the vertex shader
			normalmatrix = transpose(inverse(mat3(view * model.top())));
			glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
			glUniform1i(shininessID, shininess);
			glUniform4fv(diffuseID, 1, value_ptr(diffuse));
			glUniform1ui(is_model_texturedID, is_model_textured);
			Platter.Draw(aShader);
		}
		model.pop();


		//Set materials for cone
		shininess = 5;
		diffuse = vec4(0.52, 0.45, 0.1, 1);
		is_model_textured = 0;

		//Send uniforms and draw cone
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
		glUniform1i(shininessID, shininess);
		glUniform4fv(diffuseID, 1, value_ptr(diffuse));
		glUniform1ui(is_model_texturedID, is_model_textured);
		Cone.Draw(aShader);
	}
	model.pop();

	//This block controls the sound box
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(0, -0.09, 0));
		model.top() = scale(model.top(), vec3(1.08, 1.08, 1.08));

		//Set materials for box
		shininess = 20;
		diffuse = vec4(0.4, 0.0, 0.04, 1);
		is_model_textured = 1;

		//Send uniforms and draw box
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top()[0][0]));
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
		glUniform1i(shininessID, shininess);
		glUniform4fv(diffuseID, 1, value_ptr(diffuse));
		glUniform1ui(is_model_texturedID, is_model_textured);
		aBox.drawBox(aShader);
	}
	model.pop();	

	////////////////Modify our animation variables///////////////////

	//taken from class example, spins around the whole model.
	angle_x += angle_inc_x;
	angle_y += angle_inc_y;
	angle_z += angle_inc_z;

	//Handle spinning animation
	if (is_handle_spinning)
	{
		//Used for smoother animation
		if (handle_inc < HANDLE_STANDARD_SPEED * handle_speed)
			handle_inc += 0.1;
		else
			handle_inc = HANDLE_STANDARD_SPEED * handle_speed;
	}
	else
	{	
		//This statment used for smoother stopping of handle rotation
		if (handle_inc > 0)
			handle_inc -= 0.1;
		else handle_inc = 0;
	}
	handle_rotation -= handle_inc;

	//Platter animation
	if (is_platter_spinning)
	{
		//Used for smoother animation
		if (platter_inc > PLATTER_STANDARD_SPEED * platter_speed)
			platter_inc -= 0.1;
		else
			platter_inc = PLATTER_STANDARD_SPEED * platter_speed;
	}
	else
	{
		//This statment used for smoother stopping of platter rotation
		if (platter_inc < 0)
			platter_inc += 0.1;
		else platter_inc = 0;
	}
	platter_rotation += platter_inc;

	//Needle drop/lift
	if (is_needle_down)
	{
		if (needle_rotation < 0)
			needle_rotation += 1;
		else needle_rotation = 0;
	}
	else
	{
		if (needle_rotation > -180)
			needle_rotation -= 1;
		else needle_rotation = -180;
	}

	//Tone arm movement caused by playing record.
	if (!is_audio_paused)
	{
		if (arm_rotation > ARM_ROTATION_MIN)
			arm_rotation -= ARM_STANDARD_ROTATION * platter_speed;
		else arm_rotation = ARM_ROTATION_MIN;
	}

	//////////// Audio Controls ////////////////
	float audioposition;

	//Check if audio is playing and update flag
	AudioPlayer.IsAudioPlaying(&is_audio_playing);
	if (!is_audio_playing)
	{
		AudioPlayer.Play("dont_fence_me_in.wav");
		AudioPlayer.Pause();
		//audioposition = (arm_rotation - ARM_ROTATION_MAX) / (ARM_ROTATION_MIN - ARM_ROTATION_MAX);
		//AudioPlayer.SetPosition(audioposition);
		AudioPlayer.SetPitch(platter_speed);
	}

	//Check if audio is paused and update flag
	AudioPlayer.IsAudioPaused(&is_audio_paused);

	//Play audio when needle is touching spinning record and no sound is playing
	if (is_audio_paused && needle_rotation == 0 && is_platter_spinning && arm_rotation > ARM_ROTATION_MIN)
	{
		//Works out where the audio should be based on position of the arm
		audioposition = (arm_rotation - ARM_ROTATION_MAX) / (ARM_ROTATION_MIN - ARM_ROTATION_MAX);
		AudioPlayer.SetPosition(audioposition);
		AudioPlayer.Unpause();
	}
	
	//Stop Audio when needle is lifted from platter or platter stops
	if (!is_audio_paused && (needle_rotation != 0 || !is_platter_spinning))
		AudioPlayer.Pause();

	//Update Audio every frame
	AudioPlayer.Update(1);
}

/* Called whenever the window is resized. The new window size is given, in pixels. taken from lab examples */ 
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspect_ratio = float(w) / float(h);
}

/* change view angle, exit upon ESC taken from lab lab examples*/
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	//Closes if escape is pressed, from lab example
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Controls model rotation and scale, adapted from class example
	if (key == 'Q') angle_inc_x -= 0.1f;
	if (key == 'Q' && action == GLFW_RELEASE) angle_inc_x = 0;
	if (key == 'W') angle_inc_x += 0.1f;
	if (key == 'W' && action == GLFW_RELEASE) angle_inc_x = 0;
	if (key == 'E') angle_inc_y -= 0.1f;
	if (key == 'E' && action == GLFW_RELEASE) angle_inc_y = 0;
	if (key == 'R') angle_inc_y += 0.1f;
	if (key == 'R' && action == GLFW_RELEASE) angle_inc_y = 0;
	if (key == 'T') angle_inc_z -= 0.1f;
	if (key == 'T' && action == GLFW_RELEASE) angle_inc_z = 0;
	if (key == 'Y') angle_inc_z += 0.1f;
	if (key == 'Y' && action == GLFW_RELEASE) angle_inc_z = 0;
	if (key == 'A')
	{
		if (model_scale > 0.04) model_scale -= 0.04f; //Prevent negative scale
		else model_scale = 0;
	}
	if (key == 'S') model_scale += 0.04f;

	//Moves camera along x, y, z axes
	if (key == GLFW_KEY_UP) move_y -= 0.02;
	if (key == GLFW_KEY_DOWN) move_y += 0.02;
	if (key == GLFW_KEY_LEFT) move_x += 0.02;
	if (key == GLFW_KEY_RIGHT) move_x -= 0.02;
	if (key == GLFW_KEY_RIGHT_SHIFT) move_z -= 0.02;
	if (key == GLFW_KEY_RIGHT_CONTROL) move_z += 0.02;

	/*Controls Rotation of the tone arm, tone arm can only move a range between beggining and the end of the record.
	The tone arm cannot be moved while the needle is down.
	*/
	if (key == 'Z') 
	{
		if (needle_rotation < 0)
		{
			if (arm_rotation < ARM_ROTATION_MAX)
			{
				arm_rotation += 0.4f;
			}
			else arm_rotation = ARM_ROTATION_MAX;
		}
		else cout << "You cannot move the tonearm when the needle is down, you will damage the record!" << endl;
	}
	if (key == 'X')
	{
		if (needle_rotation < 0)
		{
			if (arm_rotation > ARM_ROTATION_MIN)
			{
				arm_rotation -= 0.4f;
			}
			else arm_rotation = ARM_ROTATION_MIN;
		}
		else cout << "You cannot move the tonearm when the needle is down, you will damage the record!" << endl;
	}

	//Controls whether handle spins by pressing Z
	if (key == 'B' && action == GLFW_PRESS)
	{
		if (is_handle_spinning == FALSE)
			is_handle_spinning = TRUE;
		else
			is_handle_spinning = FALSE;
	}

	//Controls handle speed
	if (key == 'C')
	{
		if (handle_speed > 0.2)
			handle_speed -= 0.05;
		else handle_speed = 0.2;
	}
	if (key == 'V')
	{
		if (handle_speed < 3)
			handle_speed += 0.05;
		else handle_speed = 3;
	}

	//Controls Whether platter is spinning or not
	if (key == 'G' && action == GLFW_PRESS)
	{
		if (is_platter_spinning == FALSE)
			is_platter_spinning = TRUE;
		else
			is_platter_spinning = FALSE;
	}

	//Controls platter speed
	if (key == 'D')
	{
		if (platter_speed > 0.5)
			platter_speed -= 0.009;
		else platter_speed = 0.5;
		//Sets Pitch of audio based on platter speed/
		AudioPlayer.SetPitch(platter_speed);
	}
	if (key == 'F')
	{
		if (platter_speed < 2)
			platter_speed += 0.009;
		else platter_speed = 2;
		//Sets Pitch of audio based on platter speed/
		AudioPlayer.SetPitch(platter_speed);
	}

	//Controls whether needle is up or down
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		if (is_needle_down == FALSE)
			is_needle_down = TRUE;
		else
			is_needle_down = FALSE;
	}

	//Move light position
	//if (action != GLFW_PRESS) return;
	if (key == 'O') lightx += 0.02f;
	if (key == 'P') lightx -= 0.02f;
	if (key == 'K') lighty += 0.02f;
	if (key == 'L') lighty -= 0.02f;
	if (key == 'N')	lightz += 0.02f;
	if (key == 'M') lightz -= 0.02f;
}


/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Gramophone");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	// Register the callback functions
	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);
	
	/* Output the OpenGL vendor and version */
	glw->DisplayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

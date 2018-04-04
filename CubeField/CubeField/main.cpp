#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
// to scale
#include <glm/gtc/matrix_transform.hpp>

#include "common\shader.hpp"
#include "common\controls.hpp"
#include "common/objloader.hpp"
//for loading the image
#include <soil.h>

int width, height;
unsigned char* image;

void CreateObject();
void DrawObject();

int main() {

	Window gameWindow("Cube Field", 1024, 768);

	CreateObject();

	do {
		DrawObject();
		// Swap buffers
		glfwSwapBuffers(gameWindow.window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(gameWindow.window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(gameWindow.window) == 0);

	system("PAUSE");
	return 0;
}

GLuint loadtextures(const char* filename)
{
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	if (image == NULL)
		printf("Could not load the texture image from path %s!\n", filename);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureId;
}

GLuint programID;
GLuint MatrixID;
GLuint Texture;
GLuint TextureID;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;
GLuint vertexbuffer;
GLuint uvbuffer;

void CreateObject()
{

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	// Load the texture
	//GLuint Texture = loadtextures("L200-OBJ/truck_color.jpg");
	Texture = loadtextures("L200-OBJ-triangles/truck_color.jpg");


	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	bool res = loadOBJ("L200-OBJ-triangles/truck.obj", vertices, uvs, normals);

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void DrawObject()
{


	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our shader
	glUseProgram(programID);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);


	glm::mat4 trans;
	trans = glm::scale(trans, glm::vec3(0.4f, 0.4f, 0.4f));
	GLint uniTrans = glGetUniformLocation(programID, "trans");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the Model !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}
#include "Model.h"
#include "common/shader.hpp"
#include "common/objloader.hpp"
#include "common/controls.hpp"
#include "common/texloader.hpp"

GLuint Model::ShaderID;

void Model::InitShaders()
{
	Model::ShaderID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
}

Model::Model(GLFWwindow* window, const char* modelPath, const char* texturePath)
{
	this->window = window;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	//Model::ShaderID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(Model::ShaderID, "MVP");

	// Load the texture
	//GLuint Texture = loadtextures("L200-OBJ/truck_color.jpg");
	Texture = loadtextures(texturePath);


	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(Model::ShaderID, "myTextureSampler");

	// Read our .obj file
	bool res = loadOBJ(modelPath, vertices, uvs, normals);

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void Model::Draw()
{
	// Use our shader
	glUseProgram(Model::ShaderID);

	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs(window);
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);


	glm::mat4 trans;

	// TODO: Rearange these if needed
	if (_translation != glm::vec3(0.0f, 0.0f, 0.0f)) {
		trans = glm::translate(trans, _translation);
	}
	if (_scale != glm::vec3(1.0f, 1.0f, 1.0f)) {
		trans = glm::scale(trans, _scale);
	}
	if (_rotation != glm::vec3(0.0f, 0.0f, 0.0f)) {
		trans = glm::rotate(trans, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::rotate(trans, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		trans = glm::rotate(trans, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	GLint uniTrans = glGetUniformLocation(Model::ShaderID, "trans");
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

void Model::SetTranslation(glm::vec3 translation)
{
	_translation = translation;
}
void Model::SetRotation(glm::vec3 rotation)
{
	_rotation = rotation;
}
void Model::SetScale(glm::vec3 scale)
{
	_scale = scale;
}

glm::vec3 Model::GetTranslation()
{
	return _translation;
}
glm::vec3 Model::GetRotation()
{
	return _rotation;
}
glm::vec3 Model::GetScale()
{
	return _scale;
}

Model::~Model()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(Model::ShaderID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
}

#include <iostream>

#include "Model.h"
#include "common/shader.hpp"
#include "common/objloader.hpp"
#include "common/controls.hpp"
#include "common/texloader.hpp"

void calculate_bounds(std::vector<glm::vec3>& verts, glm::vec3& minBounds, glm::vec3& maxBounds);

glm::vec3 Model::ambientColor;
glm::vec3 Model::diffusePosition;
glm::vec3 Model::cameraLightPos;
glm::vec3 Model::explosionLightPos;
glm::vec3 Model::diffuseColor;
glm::vec3 Model::cameraLightColor;
glm::vec3 Model::explosionLightColor;


Model::Model(GLFWwindow* window, const char* modelPath, const char* texturePath)
{
	this->window = window;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	// Model::ShaderID = LoadShaders("TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader");
	ShaderID = LoadShaders("vert.glsl", "frag.glsl");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(ShaderID, "MVP");

	// Load the texture
	//GLuint Texture = loadtextures("L200-OBJ/truck_color.jpg");
	Texture = loadtextures(texturePath);


	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(ShaderID, "myTextureSampler");

	// Read our .obj file
	bool res = loadOBJ(modelPath, vertices, uvs, normals);

	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	//Added to load normals from the file
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	calculate_bounds(vertices, _minBounds, _maxBounds);
}

void Model::UpdateMatrices()
{
	// Use our shader
	glUseProgram(ShaderID);

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

	GLint uniTrans = glGetUniformLocation(ShaderID, "trans");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
}
void Model::CalculateLighting()
{
	// Set the ambient light color
	GLint Ambient_Light_color = glGetUniformLocation(ShaderID, "ambientColor");
	glUniform3fv(Ambient_Light_color, 1, glm::value_ptr(Model::ambientColor));

	//Diffuse Staff
	// Set the Diffuse light Position
	GLint Diffuse_Light_position = glGetUniformLocation(ShaderID, "lightPos");
	glUniform3fv(Diffuse_Light_position, 1, glm::value_ptr(Model::diffusePosition));
	GLint Camera_Light_position = glGetUniformLocation(ShaderID, "cameraLightPos");
	glUniform3fv(Camera_Light_position, 1, glm::value_ptr(Model::cameraLightPos));
	GLint Explosion_Light_position = glGetUniformLocation(ShaderID, "explosionLightPos");
	glUniform3fv(Explosion_Light_position, 1, glm::value_ptr(Model::explosionLightPos));
	// Set the Diffuse light Position
	GLint Diffuse_Light_color = glGetUniformLocation(ShaderID, "lightColor");
	glUniform3fv(Diffuse_Light_color, 1, glm::value_ptr(Model::diffuseColor));
	GLint Camera_Light_color = glGetUniformLocation(ShaderID, "cameraLightColor");
	glUniform3fv(Camera_Light_color, 1, glm::value_ptr(Model::cameraLightColor));
	GLint Explosion_Light_color = glGetUniformLocation(ShaderID, "explosionLightColor");
	glUniform3fv(Explosion_Light_color, 1, glm::value_ptr(Model::explosionLightColor));

	//Specular Staff
	//Set the Cameraposition (eye location)
	GLint eye_location = glGetUniformLocation(ShaderID, "vertexPosition_cameraspace");
	glm::vec3 CameraPosition = getCameraPosition();
	glUniform3fv(eye_location, 1, glm::value_ptr(CameraPosition));
	//Set the specular color
	GLint Spec_Light_color = glGetUniformLocation(ShaderID, "specColor");
	glUniform3fv(Spec_Light_color, 1, glm::value_ptr(Model::diffuseColor));
	// Set the Specular light Position
	GLint Spec_Light_position = glGetUniformLocation(ShaderID, "speclightPos");
	glUniform3fv(Spec_Light_position, 1, glm::value_ptr(Model::diffusePosition));
}
void Model::Render()
{
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

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the Model !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Model::Draw()
{
	UpdateMatrices();
	CalculateLighting();
	Render();
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

Model::~Model()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(ShaderID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
}


void calculate_bounds(std::vector<glm::vec3>& verts, glm::vec3& minBounds, glm::vec3& maxBounds)
{
	float minX, maxX, minY, maxY, minZ, maxZ;
	minX = verts[0].x;
	maxX = verts[0].x;
	minY = verts[0].y;
	maxY = verts[0].y;
	minZ = verts[0].z;
	maxZ = verts[0].z;

	for (unsigned int i = 0; i < verts.size(); i++)
	{
		if (verts[i].x < minX) minX = verts[i].x;
		if (verts[i].x > maxX) maxX = verts[i].x;

		if (verts[i].y < minY) minY = verts[i].y;
		if (verts[i].y > maxY) maxY = verts[i].y;

		if (verts[i].z < minZ) minZ = verts[i].z;
		if (verts[i].z > maxZ) maxZ = verts[i].z;
	}

	minBounds.x = minX;
	minBounds.y = minY;
	minBounds.z = minZ;
	maxBounds.x = maxX;
	maxBounds.y = maxY;
	maxBounds.z = maxZ;

	std::cout << "min: (" << minX << "," << minY << "," << minZ << ") | max(" << maxX << "," << maxY << "," << maxZ << ")" << std::endl;
}
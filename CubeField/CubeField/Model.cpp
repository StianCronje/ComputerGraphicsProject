#include <iostream>

#include "Model.h"
#include "common/shader.hpp"
#include "common/objloader.hpp"
#include "common/controls.hpp"
#include "common/texloader.hpp"

void calculate_bounds(std::vector<glm::vec3>& verts, glm::vec3& minBounds, glm::vec3& maxBounds);


glm::vec3 Model::directionalLightDir;
glm::vec3 Model::directionalLightColor;
glm::vec3 Model::ambientColor;
glm::vec3 Model::cameraLightPos;
glm::vec3 Model::explosionLightPos;
glm::vec3 Model::cameraLightColor;
glm::vec3 Model::explosionLightColor;


Model::Model(GLFWwindow* window, const char* modelPath, const char* texturePath)
{
	this->window = window;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	ShaderID = LoadShaders("vert.glsl", "lighting_fragment.glsl");

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(ShaderID, "MVP");

	// Load the texture
	Texture = loadtextures(texturePath);


	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(ShaderID, "material.diffuse");

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
	glm::vec3 CameraPosition = getCameraPosition();
	glUniform3fv( glGetUniformLocation(ShaderID, "viewPos"), 1, glm::value_ptr(CameraPosition));
	glUniform1f( glGetUniformLocation(ShaderID, "material.shininess"), 32.0f);

	//directional light
	glUniform3fv( glGetUniformLocation(ShaderID, "dirLight.direction"), 1, glm::value_ptr(Model::directionalLightDir));
	glUniform3fv( glGetUniformLocation(ShaderID, "dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2f)));
	glUniform3fv( glGetUniformLocation(ShaderID, "dirLight.diffuse"), 1, glm::value_ptr(Model::directionalLightColor));
	glUniform3fv( glGetUniformLocation(ShaderID, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));


	//point light 1
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[0].position"), 1, glm::value_ptr(Model::cameraLightPos));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[0].ambient"), 1, glm::value_ptr(glm::vec3(0.05f)));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[0].diffuse"), 1, glm::value_ptr(Model::cameraLightColor));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[0].specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[0].constant"), 1.0f);
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[0].linear"), 0.09f);
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[0].quadratic"), 0.032f);

	//point light 2
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[1].position"), 1, glm::value_ptr(Model::explosionLightPos));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[1].ambient"), 1, glm::value_ptr(glm::vec3(0.05f)));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[1].diffuse"), 1, glm::value_ptr(Model::explosionLightColor));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[1].specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[1].constant"), 1.0f);
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[1].linear"), 0.09f);
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[1].quadratic"), 0.032f);

	//point light 3
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[2].position"), 1, glm::value_ptr(glm::vec3(0.0f)));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[2].ambient"), 1, glm::value_ptr(glm::vec3(0.05f)));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[2].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f)));
	glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[2].specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[2].constant"), 1.0f);
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[2].linear"), 0.09f);
	glUniform1f( glGetUniformLocation(ShaderID, "pointLights[2].quadratic"), 0.032f);

	//point light 4
	// glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[3].position"), 1, glm::value_ptr(pointLightPositions[3]));
	// glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[3].ambient"), 1, glm::value_ptr(glm::vec3(0.05f)));
	// glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[3].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f)));
	// glUniform3fv( glGetUniformLocation(ShaderID, "pointLights[3].specular"), 1, glm::value_ptr(glm::vec3(1.0f)));
	// glUniform1f( glGetUniformLocation(ShaderID, "pointLights[3].constant"), 1.0f);
	// glUniform1f( glGetUniformLocation(ShaderID, "pointLights[3].linear"), 0.09f);
	// glUniform1f( glGetUniformLocation(ShaderID, "pointLights[3].quadratic"), 0.032f);
}
void Model::Render()
{
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(ShaderID, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(ShaderID, "material.specular"), 0);

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
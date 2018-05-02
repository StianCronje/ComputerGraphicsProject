#pragma once
#include <vector>
#include <GL/glew.h>
// #include <GL\glut.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// to scale
#include <glm/gtc/matrix_transform.hpp>

class Model
{
private:
	GLuint ShaderID;
	GLuint VertexArrayID;
	GLuint MatrixID;
	GLuint Texture;
	GLuint TextureID;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLFWwindow* window;

	glm::vec3 _translation = glm::vec3(0.0f, 0.0f, 0.0f);
	float _rotationDeg = 0;
	glm::vec3 _rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 _minBounds = glm::vec3(0,0,0);
	glm::vec3 _maxBounds = glm::vec3(0,0,0);
public:
	Model(GLFWwindow* window, const char* modelPath, const char* texturePath);
	~Model();
public:
	void Draw();
	void SetTranslation(glm::vec3 translation);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	inline glm::vec3 GetTranslation() { return _translation; }
	inline glm::vec3 GetRotation(){ return _rotation; }
	inline glm::vec3 GetScale() { return _scale; }
	inline glm::vec3 GetMinBounds() { return _minBounds; }
	inline glm::vec3 GetMaxBounds(){ return _maxBounds; }
private:
	void UpdateMatrices();
	void CalculateLighting();
	void Render();
public:
	static glm::vec3 ambientColor;
	static glm::vec3 diffusePosition;
	static glm::vec3 cameraLightPos;
	static glm::vec3 explosionLightPos;
	static glm::vec3 diffuseColor;
	static glm::vec3 cameraLightColor;
	static glm::vec3 explosionLightColor;
};


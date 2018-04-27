#ifndef CONTROLS_HPP
#define CONTROLS_HPP

glm::vec3 getCameraPosition();
void setCameraPosition(glm::vec3 pos);
void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

void setCameraSpeed(float _speed);
float getCameraSpeed();

#endif
#ifndef CONTROLS_HPP
#define CONTROLS_HPP

glm::vec3 getCameraPosition();
void computeMatricesFromInputs(GLFWwindow* window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif
#pragma once
#include <glm/glm.hpp>

glm::vec3 universeUp = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
private:
	glm::vec3 cameraPos;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

public:
	Camera();
	~Camera();
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 lookingAt = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f)); //initialise with start position and looking towards lookingAt
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 forwardVector = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f)); //initialise with position and direction vectors

	//TODO: test
};
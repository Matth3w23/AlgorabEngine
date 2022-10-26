#include "camera.h"

Camera::Camera() {
	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
}

Camera::~Camera() {
}

Camera::Camera(glm::vec3 position, glm::vec3 lookingAt) {
}

Camera::Camera(glm::vec3 position, glm::vec3 forwardVector, glm::vec3 upVector) {
}

#include "camera.h"

Camera::Camera() {
}

Camera::~Camera() {
}

Camera::Camera(glm::vec3 pos, glm::vec3 directionIndicator, bool lookAt) : position(pos) {
	if (lookAt) {
		forward = glm::normalize(directionIndicator - position);
	} else {
		forward = glm::normalize(directionIndicator);
	}

	right = glm::cross(universeUp, forward); //may have to check if forward is straight up?
	up = glm::cross(forward, right); //add roll later?
}

void Camera::updateViewMatrix() {
	viewMatrix = glm::lookAt(position, position + forward, up);
}

void Camera::updateProjectionMatrix() {
	projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
}

void Camera::lookAt(glm::vec3 lookPos) {
	forward = glm::normalize(lookPos - position);
}

float Camera::getFov() {
	return fieldOfView;
}

float Camera::getAspectRatio() {
	return aspectRatio;
}

float Camera::getNearPlane() {
	return nearPlane;
}

float Camera::getFarPlane() {
	return farPlane;
}

void Camera::setNearPlane(float near) {
	nearPlane = near;
}

void Camera::setAspectRatio(float aspect) {
	aspectRatio = aspect;
}

void Camera::setFieldOfView(float fov) {
	fieldOfView = fov;
}

void Camera::setFarPlane(float far) {
	farPlane = far;
}

void Camera::setProjectionFrustum(float fov, float aspect, float near, float far, bool updateProjection) {
	setFieldOfView(fov);
	setAspectRatio(aspect);
	setNearPlane(near);
	setFarPlane(far);

	if (updateProjection) {
		updateProjectionMatrix();
	}
}


#include "camera.h"

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

	setProjectionFrustum(60, 800.0f / 600.0f, 0.1, 100, true); //TODO: Add this to constructor

	updateViewMatrix();
	updateRelativeViewMatrix();
	updateProjectionMatrix();
}

void Camera::updateViewMatrix() {
	viewMatrix = glm::lookAtLH(position, position + forward, up);
}

void Camera::updateRelativeViewMatrix() {
	relativeViewMatrix = glm::lookAtLH(glm::vec3(0.0f), forward, up);
}

void Camera::updateProjectionMatrix() {
	projectionMatrix = glm::perspectiveLH(fieldOfView, aspectRatio, nearPlane, farPlane);
}

void Camera::lookAt(glm::vec3 lookPos) {
	forward = glm::normalize(lookPos - position);
	updateAnglesFromVectors();
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

glm::vec3& Camera::getPosition() {
	return position;
}

glm::mat4& Camera::getViewMatrix() {
	return viewMatrix;
}
glm::mat4& Camera::getRelativeViewMatrix() {
	return relativeViewMatrix;
}
glm::mat4& Camera::getProjectionMatrix() {
	return projectionMatrix;
}

void Camera::updateVectorsFromAngles() {
	forward.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(universeUp, forward));
	up = glm::normalize(glm::cross(forward, right));

}

void Camera::updateAnglesFromVectors() { //0 degrees yaw points towards [0,0,1]
	yaw = glm::degrees(atan2(forward.x, forward.z));
	pitch = glm::degrees(asin(forward.y));
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

void Camera::setPosition(glm::vec3 pos) {
	position = pos;
}

void Camera::move(glm::vec3 moveVec) {
	position += moveVec;
}

void Camera::moveRelative(glm::vec3 moveVec) {
	position += moveVec.x * right + moveVec.y * up + moveVec.z * forward;
}

void Camera::turn(float yw, float ptch, bool constrain) {
	yaw += yw;
	pitch += ptch;
	yaw = fmod(yaw, 360.0f);

	if (constrain) {
		if (pitch > 89.99f) {
			pitch = 89.99f;
		}
		if (pitch < -89.99f) {
			pitch = -89.99f;
		}
	}

	updateVectorsFromAngles();
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


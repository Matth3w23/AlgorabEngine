#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	//all in world space, which is left handed (y up)
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	glm::vec3 universeUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float fieldOfView = 60.0f;
	float aspectRatio = 800.0f/600.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

public:
	~Camera();
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 directionIndicator = glm::vec3(0.0f, 0.0f, 1.0f), bool lookAt = false);

	void updateViewMatrix();
	void updateProjectionMatrix();

	void lookAt(glm::vec3 lookPos);

	float getFov();
	float getAspectRatio();
	float getNearPlane();
	float getFarPlane();

	glm::mat4* getViewMatrix();
	glm::mat4* getProjectionMatrix();

	void setFieldOfView(float fov);
	void setAspectRatio(float aspect);
	void setNearPlane(float near);
	void setFarPlane(float far);

	void setPosition(glm::vec3 pos);

	void setProjectionFrustum(float fov, float aspect, float near, float far, bool updateProjection = true);

};
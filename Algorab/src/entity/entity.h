#pragma once
#include <glm/glm.hpp>
#include "../model/model.h"

class Entity {
private:
	glm::vec3 position;
public:
	Entity(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
	glm::vec3& getPosition();
	void setPosition(glm::vec3 pos);

};

class ModelEntity : public Entity {
private:
	Model* model;
	float scale;
public:
	ModelEntity(Model* mod, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float scl = 1.0f);

	Model* getModel();
	float getScale();
	float getFurVertDist();

	void setModel(Model* mod);
	void setScale(float scl);
};

class PointEntity : public Entity {
private:
	glm::vec4 colour;
	float radius;
	bool constantScale; //whether the size of the point changes by distance
public:
	PointEntity(glm::vec4 col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float rad = 1.0f, bool conScale = false);

	glm::vec4 getColour();
	float getRadius();
	bool getConstantScale();

	void setColour(glm::vec4 col);
	void setRadius(float rad);
	void setConstantScale(bool conScl);


};
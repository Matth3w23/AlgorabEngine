#pragma once
#include <glm/glm.hpp>
#include "../model/model.h"
#include "../utility/uFVec.h"

class Entity {
private:
	UFVec3 defaultAnchor = UFVec3(0.0f, 0.0f, 0.0f);

	UFVec3 position;
	glm::vec3 relPosition;
	UFVec3* anchorPosition = &defaultAnchor; //TODO: Have default anchor in anchor class
	
public:
	Entity(glm::vec3 pos);
	Entity(UFVec3 pos = UFVec3());

	UFVec3& getPosition();
	void setPosition(UFVec3 pos);
	void setPosition(glm::vec3 pos);
};

class ModelEntity : public Entity {
private:
	Model* model;
	float scale;
public:
	ModelEntity(Model* mod, glm::vec3 pos, float scl = 1.0f);
	ModelEntity(Model* mod, UFVec3 pos = UFVec3(), float scl = 1.0f);

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
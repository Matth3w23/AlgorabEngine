#pragma once
#include <glm/glm.hpp>
#include "../model/model.h"
#include "../utility/uFVec.h"



class Entity {
protected:
	UFVec3 defaultAnchor = UFVec3(0.0f, 0.0f, 0.0f);

	UFVec3 position = UFVec3(0.0f, 0.0f, 0.0f);
	glm::vec3 relPosition;
	UFVec3* anchorPosition = &defaultAnchor; //TODO: Have default anchor in anchor class
	float scale = 1.0f;

	Entity* parent = nullptr; //assume default (position 0, scale 1, etc.)
	
public:
	glm::vec3 posDif; //need to rename
	glm::vec4 relCamPos;

	Entity(glm::vec3 pos);
	Entity(UFVec3 pos = UFVec3());

	UFVec3& getPosition();
	void setPosition(UFVec3 pos);
	void setPosition(glm::vec3 pos);

	float getScale();
	void setScale(float scl);

	Entity* getParent();
	void setParent(Entity* ent);
};

class ModelEntity : public Entity {
private:
	Model* model;
public:
	ModelEntity(Model* mod, glm::vec3 pos, float scl = 1.0f);
	ModelEntity(Model* mod, UFVec3 pos = UFVec3(), float scl = 1.0f);

	Model* getModel();
	float getFurVertDist();

	void setModel(Model* mod);
	
};

class PointEntity : public Entity {
private:
	glm::vec4 colour;
	bool constantScale; //whether the size of the point changes by distance
public:
	PointEntity(glm::vec4 col = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), float rad = 1.0f, bool conScale = false);

	glm::vec4 getColour();
	bool getConstantScale();

	void setColour(glm::vec4 col);
	void setConstantScale(bool conScl);


};

class EntityGrouper : public Entity {
private:
	std::vector<EntityGrouper*> childGroups;
	std::vector<ModelEntity*> childModels;
	float furthestDistance; //just a float, doesn't have to be too precise?
	bool childrenToBeRendered = false;
	
public:
	void addChild(EntityGrouper* ent); //check if entity not already a child
	void addChild(ModelEntity* ent);

	void removeChild(EntityGrouper* ent);
	void removeChild(ModelEntity* ent);

	void clearChildren();

	void updateFurthestDistance(UFVec3 childPos, float childFurDist);
	float getFurthestDistance();

	void setChildRendered(bool renderBool);
	bool getChildrenToBeRendered();

};
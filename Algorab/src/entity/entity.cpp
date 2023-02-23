#include "entity.h"

////////
//ENTITY
////////

Entity::Entity(glm::vec3 pos) :
	position(UFVec3(pos)) {
	;
}
Entity::Entity(UFVec3 pos) :
	position(pos) {
}
UFVec3& Entity::getPosition() {
	return position;
}

void Entity::setPosition(UFVec3 pos) {
	position = pos;
}

void Entity::setPosition(glm::vec3 pos) {
	position = UFVec3(pos);
}

float Entity::getScale() {
	return scale;
}

void Entity::setScale(float scl) {
	scale = scl;
}

Entity* Entity::getParent() {
	return parent;
}

void Entity::setParent(Entity* ent) {
	parent = ent;
}

//////////////
//MODEL ENTITY
//////////////

ModelEntity::ModelEntity(Model* mod, glm::vec3 pos, float scl) :
	model(mod), Entity::Entity(pos){
	setScale(scl);
}

ModelEntity::ModelEntity(Model* mod, UFVec3 pos, float scl) :
	model(mod), Entity::Entity(pos) {
	setScale(scl);
}

Model* ModelEntity::getModel() {
	return model;
}

float ModelEntity::getFurVertDist() {
	return model->getfurVertDist();
}

void ModelEntity::setModel(Model* mod) {
	model = mod;
}

void ModelEntity::setHidden(bool val) {
	hidden = val;
}

bool ModelEntity::getHidden() {
	return hidden;
}

///////////////////////
//POINT ENTITY (UNUSED)
///////////////////////

PointEntity::PointEntity(glm::vec4 col, float rad, bool conScale) :
	colour(col), constantScale(conScale) {
	setScale(rad);
}

glm::vec4 PointEntity::getColour() {
	return colour;
}

bool PointEntity::getConstantScale() {
	return constantScale;
}

void PointEntity::setColour(glm::vec4 col) {
	colour = col;
}

void PointEntity::setConstantScale(bool conScl) {
	constantScale = conScl;
}


////////////////
//ENTITY GROUPER
////////////////

//need to update furthest distance
void EntityGrouper::addChild(EntityGrouper* ent) {
	if (std::find(childGroups.begin(), childGroups.end(), ent) != childGroups.end()) {
		childGroups.push_back(ent);

		glm::vec3 posDiff = uFVecToVec(uFVecSub(position, ent->getPosition()));
		float length = glm::length(posDiff);
		furthestDistance = std::max(length + ent->getFurthestDistance(), furthestDistance);
	}

	//note this doesn't check for cyclical children, which would cause issues
}
void EntityGrouper::addChild(ModelEntity* ent) {
	if (std::find(childModels.begin(), childModels.end(), ent) != childModels.end()) {
		childModels.push_back(ent);
	}

	//note this doesn't check for cyclical children, which would cause issues
}

void EntityGrouper::removeChild(EntityGrouper* ent) {
	auto find = std::find(childGroups.begin(), childGroups.end(), ent);
	if (find != childGroups.end()) {
		childGroups.erase(find);
	}
}
void EntityGrouper::removeChild(ModelEntity* ent) {
	auto find = std::find(childModels.begin(), childModels.end(), ent);
	if (find != childModels.end()) {
		childModels.erase(find);
	}
}

void EntityGrouper::clearChildren() {
	for (EntityGrouper* e: childGroups) {
		e->setParent(nullptr);
	}
	for (ModelEntity* e : childModels) {
		e->setParent(nullptr);
	}

	childGroups.clear();
	childModels.clear();

}

void EntityGrouper::updateFurthestDistance(UFVec3 childPos, float childFurDist, float childScale) {
	glm::vec3 posDiff = uFVecToVec(uFVecSub(position, childPos));
	float length = glm::length(posDiff);
	furthestDistance = std::max((length + childFurDist) * childScale, furthestDistance);
}

void EntityGrouper::updateFurthestDistance() {
	for (EntityGrouper* eg : childGroups) {
		updateFurthestDistance(eg->getPosition(), eg->getFurthestDistance(), eg->getScale());
	}
	for (ModelEntity* me : childModels) {
		updateFurthestDistance(me->getPosition(), me->getFurVertDist(), me->getScale());
	}
}


float EntityGrouper::getFurthestDistance() {
	return furthestDistance;
}

std::vector<EntityGrouper*>* EntityGrouper::getChildGroups() {
	return &childGroups;
}

std::vector<ModelEntity*>* EntityGrouper::getChildModels() {
	return &childModels;
}

void EntityGrouper::setHidden(bool val) {
	hidden = val;
}

bool EntityGrouper::getHidden() {
	return hidden;
}

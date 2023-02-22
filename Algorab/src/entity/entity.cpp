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

////////////////////////
//POINT GROUPER (UNUSED)
////////////////////////

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

void EntityGrouper::addChild(Entity* ent) {
	if (std::find(children.begin(), children.end(), children) != children.end()) {
		children.push_back(ent);
	}

	//note this doesn't check for cyclical children, which would cause issues
}

void EntityGrouper::removeChild(Entity*) {
	auto find = std::find(children.begin(), children.end(), children);
	if (find != children.end()) {
		children.erase(find);
	}
}

void EntityGrouper::clearChildren() {
	for (Entity* e: children) {
		e->setParent(nullptr);
	}

	children.clear();

}

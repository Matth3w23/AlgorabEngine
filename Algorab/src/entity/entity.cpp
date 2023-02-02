#include "entity.h"

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




ModelEntity::ModelEntity(Model* mod, glm::vec3 pos, float scl) :
	model(mod), scale(scl), Entity::Entity(pos){
	;
}

ModelEntity::ModelEntity(Model* mod, UFVec3 pos, float scl) :
	model(mod), scale(scl), Entity::Entity(pos) {
}

Model* ModelEntity::getModel() {
	return model;
}

float ModelEntity::getScale() {
	return scale;
}

float ModelEntity::getFurVertDist() {
	return model->getfurVertDist();
}

void ModelEntity::setModel(Model* mod) {
	model = mod;
}

void ModelEntity::setScale(float scl) {
	scale = scl;
}





PointEntity::PointEntity(glm::vec4 col, float rad, bool conScale) :
	colour(col), radius(rad), constantScale(conScale) {
	;
}

glm::vec4 PointEntity::getColour() {
	return colour;
}

float PointEntity::getRadius() {
	return radius;
}

bool PointEntity::getConstantScale() {
	return constantScale;
}

void PointEntity::setColour(glm::vec4 col) {
	colour = col;
}

void PointEntity::setRadius(float rad) {
	radius = rad;
}

void PointEntity::setConstantScale(bool conScl) {
	constantScale = conScl;
}

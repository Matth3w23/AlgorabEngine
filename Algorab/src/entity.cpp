#include "entity.h"

Entity::Entity(glm::vec3 pos) :
	position(pos) {
	;
}

glm::vec3 Entity::getPosition() {
	return position;
}

void Entity::setPosition(glm::vec3 pos) {
	position = pos;
}

ModelEntity::ModelEntity(Model mod, glm::vec3 pos, float scl) :
	model(mod), scale(scl), Entity::Entity(pos){
	;
}

Model ModelEntity::getModel() {
	return model;
}

float ModelEntity::getScale() {
	return scale;
}

void ModelEntity::setModel(Model mod) {
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

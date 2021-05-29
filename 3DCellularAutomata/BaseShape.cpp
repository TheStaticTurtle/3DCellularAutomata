#include "BaseShape.h"

BaseShape::BaseShape() {
	this->calculateMesh();
}

void BaseShape::calculateMesh() {
	this->mesh = new Mesh(this->vertices, this->indices);
}

void BaseShape::draw(Shader& shader, Camera& camera) {
	this->mesh->draw(shader, camera, glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}
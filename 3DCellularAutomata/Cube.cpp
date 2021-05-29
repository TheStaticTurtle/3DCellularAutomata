#include "Cube.h"

Cube::Cube(glm::vec3 center, glm::vec3 size) {

	float x = center.x + size.x / 2;
	float py = center.y + size.y / 2;
	float ny = center.y - size.y / 2;
	float z = center.z + size.z / 2;

	Vertex vertices[] = {
		Vertex{glm::vec3(-x,  py, -z), glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(x,  py, -z), glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(-x,  py,  z), glm::vec3(0.0f, 1.0f, 1.0f)},
		Vertex{glm::vec3(x,  py,  z), glm::vec3(0.0f, 1.0f, 1.0f)},

		Vertex{glm::vec3(-x, ny, -z), glm::vec3(1.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(x, ny, -z), glm::vec3(1.0f, 0.0f, 1.0f)},
		Vertex{glm::vec3(-x, ny,  z), glm::vec3(0.5f, 0.5f, 0.5f)},
		Vertex{glm::vec3(x, ny,  z), glm::vec3(1.0f, 1.0f, 1.0f)},
	};

	// Indices for vertices order
	GLuint indices[] = {
		0,1,3,  0,2,3, //Top     face
		4,6,7,  4,5,7, //Bottom  face
		0,1,5,  0,4,5, //Rear    face
		2,3,7,  2,6,7, //Front   face
		1,5,7,  1,3,7, //Left    face
		0,2,6,  0,4,6, //Right   face
	};

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	this->vertices = verts;
	this->indices = ind;

	this->calculateMesh();
}

void Cube::calculateMesh() {
	BaseShape::calculateMesh();
}

void Cube::draw(Shader& shader, Camera& camera) {
	BaseShape::draw(shader, camera);
}
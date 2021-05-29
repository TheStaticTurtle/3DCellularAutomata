#include "Pyramid.h"

Pyramid::Pyramid(glm::vec3 center, glm::vec3 size) {
	this->vertices = new std::vector <Vertex>();
	this->indices = new std::vector <GLuint>();

	float px = center.x + size.x / 2;
	float nx = center.x - size.x / 2;
	float py = center.y + size.y / 2;
	float ny = center.y - size.y / 2;
	float pz = center.z + size.z / 2;
	float nz = center.z - size.z / 2;


	Vertex vertices[] = { //               COORDINATES           /            COLORS       
		Vertex{glm::vec3(center.x, py,  center.z),  glm::vec3(1.0f, 1.0f, 1.0f)},

		Vertex{glm::vec3(nx, ny, pz), glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(nx, ny, nz), glm::vec3(1.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(px, ny, nz), glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(px, ny, pz), glm::vec3(0.0f, 0.0f, 1.0f)},
	};

	// Indices for vertices order
	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,

		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	this->vertices = &verts;
	this->indices = &ind;

	this->calculateMesh();
}

void Pyramid::calculateMesh() {
	BaseShape::calculateMesh();
}

void Pyramid::draw(Shader& shader, Camera& camera) {
	BaseShape::draw(shader, camera);
}
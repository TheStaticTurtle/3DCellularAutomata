#include "Cube.h"

Cube::Cube(glm::vec3 center, glm::vec3 size) {
	this->vertices = new std::vector <Vertex>();
	this->indices = new std::vector <GLuint>();

	float px = center.x + size.x / 2;
	float nx = center.x - size.x / 2;
	float py = center.y + size.y / 2;
	float ny = center.y - size.y / 2;
	float pz = center.z + size.z / 2;
	float nz = center.z - size.z / 2;

	Vertex vertices[] = {
		Vertex{glm::vec3(nx,  py, nz), glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(px,  py, nz), glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(nx,  py, pz), glm::vec3(0.0f, 1.0f, 1.0f)},
		Vertex{glm::vec3(px,  py, pz), glm::vec3(0.0f, 1.0f, 1.0f)},

		Vertex{glm::vec3(nx, ny, nz), glm::vec3(1.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(px, ny, nz), glm::vec3(1.0f, 0.0f, 1.0f)},
		Vertex{glm::vec3(nx, ny, pz), glm::vec3(0.5f, 0.5f, 0.5f)},
		Vertex{glm::vec3(px, ny, pz), glm::vec3(1.0f, 1.0f, 1.0f)},

		/*
		Vertex{glm::vec3(nx + 2,  py, nz), glm::vec3(1.0f, 0.0f, 0.0f)},
		Vertex{glm::vec3(px + 2,  py, nz), glm::vec3(0.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(nx + 2,  py, pz), glm::vec3(0.0f, 1.0f, 1.0f)},
		Vertex{glm::vec3(px + 2,  py, pz), glm::vec3(0.0f, 1.0f, 1.0f)},

		Vertex{glm::vec3(nx + 2, ny, nz), glm::vec3(1.0f, 1.0f, 0.0f)},
		Vertex{glm::vec3(px + 2, ny, nz), glm::vec3(1.0f, 0.0f, 1.0f)},
		Vertex{glm::vec3(nx + 2, ny, pz), glm::vec3(0.5f, 0.5f, 0.5f)},
		Vertex{glm::vec3(px + 2, ny, pz), glm::vec3(1.0f, 1.0f, 1.0f)},
		*/
	};

	// Indices for vertices order
	GLuint indices[] = {
		0,1,3,  0,2,3, //Top     face
		4,6,7,  4,5,7, //Bottom  face
		0,1,5,  0,4,5, //Rear    face
		2,3,7,  2,6,7, //Front   face
		1,5,7,  1,3,7, //Left    face
		0,2,6,  0,4,6, //Right   face

		/*
		0 + 8,1 + 8,3 + 8,  0 + 8,2 + 8,3 + 8, //Top     face
		4 + 8,6 + 8,7 + 8,  4 + 8,5 + 8,7 + 8, //Bottom  face
		0 + 8,1 + 8,5 + 8,  0 + 8,4 + 8,5 + 8, //Rear    face
		2 + 8,3 + 8,7 + 8,  2 + 8,6 + 8,7 + 8, //Front   face
		1 + 8,5 + 8,7 + 8,  1 + 8,3 + 8,7 + 8, //Left    face
		0 + 8,2 + 8,6 + 8,  0 + 8,4 + 8,6 + 8, //Right   face
		*/
	};

	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	this->vertices = &verts;
	this->indices = &ind;

	this->calculateMesh();
}

void Cube::calculateMesh() {
	BaseShape::calculateMesh();
}

void Cube::draw(Shader& shader, Camera& camera) {
	BaseShape::draw(shader, camera);
}
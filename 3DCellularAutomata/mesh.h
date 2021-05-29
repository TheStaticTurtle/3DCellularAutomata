#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include "VAO.h"
#include "EBO.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh
{
public:
	std::vector <Vertex> *vertices;
	std::vector <GLuint> *indices;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;

	// Initializes the mesh
	Mesh(std::vector <Vertex>* vertices, std::vector <GLuint>* indices);

	// Draws the mesh
	void draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);

	void update();

private:
	VBO* vbo;
	EBO* ebo;
};
#endif
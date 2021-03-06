#include "mesh.h"

Mesh::Mesh(std::vector <Vertex>* vertices, std::vector <Index>* indices)
{
	this->vertices = vertices;
	this->indices = indices;

	VAO.bind();
	// Generates Vertex Buffer Object and links it to vertices
	this->vbo = new VBO(*this->vertices);
	// Generates Element Buffer Object and links it to indices
	this->ebo = new EBO(*this->indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.linkAttrib(*this->vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.linkAttrib(*this->vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	// unbind all to prevent accidentally modifying them
	VAO.unbind();
	this->vbo->unbind();
	this->ebo->unbind();
}


void Mesh::draw(Shader& shader, Camera& camera, glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale) {
	// bind shader to be able to access uniforms
	shader.activate();
	VAO.bind();

	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.matrix(shader, "camMatrix");

	// Initialize matrices
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
	glm::mat4 sca = glm::mat4(1.0f);

	// Transform the matrices to their correct form
	trans = glm::translate(trans, translation);
	rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	// Push the matrices to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	unsigned int size = this->indices->size() * (sizeof(Index) / sizeof(int));
	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, size , GL_UNSIGNED_INT, 0);
}

void Mesh::update() {
	this->vbo->updateData(this->vertices);
	this->ebo->updateData(this->indices);
}

void Mesh::updateIndexes() {
	this->ebo->updateData(this->indices);
}

#include"VBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
}

// Binds the VBO
void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::updateData(std::vector<Vertex>* vertices) {
	//glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), vertices->data(), GL_DYNAMIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Unbinds the VBO
void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::_delete()
{
	glDeleteBuffers(1, &ID);
}
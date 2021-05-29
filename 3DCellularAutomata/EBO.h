#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(std::vector<GLuint>& indices);

	// Binds the EBO
	void bind();
	// Unbinds the EBO
	void unbind();
	// Deletes the EBO
	void _delete();
};

#endif
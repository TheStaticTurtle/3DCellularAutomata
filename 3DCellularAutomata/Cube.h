#ifndef SHAPE_CUBE_CLASS_H
#define SHAPE_CUBE_CLASS_H

#include "mesh.h"
#include "shader.h"
#include "camera.h"

#include "BaseShape.h"

class Cube : BaseShape {
public:
	Cube(glm::vec3 center, glm::vec3 size);
	void calculateMesh();
	void draw(Shader& shader, Camera& camera);
};

#endif
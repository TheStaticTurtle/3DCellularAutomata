#ifndef SHAPE_BASE_CLASS_H
#define SHAPE_BASE_CLASS_H

#include "mesh.h"
#include "shader.h"
#include "camera.h"

class BaseShape
{
public:
	BaseShape();
	void calculateMesh();
	void draw(Shader &shader, Camera &camera);

protected:
	std::vector <Vertex>* vertices;
	std::vector <Index>* indices;
	Mesh* mesh;
};

#endif
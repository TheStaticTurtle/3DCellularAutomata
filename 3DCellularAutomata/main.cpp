#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"
#include "mesh.h"

const unsigned int width = 800;
const unsigned int height = 800;
/*
// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      //
	-0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, // Lower left corner
	 0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f, // Upper left corner
	-0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 1.0f, // Upper left corner

	-0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f, // Lower left corner
	 0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f, // Upper left corner
	-0.5f, -0.5f,  0.5f,    0.5f, 0.5f, 0.5f, // Upper left corner
	 0.5f, -0.5f,  0.5f,    1.0f, 1.0f, 1.0f, // Upper left corner
};

// Indices for vertices order
GLuint indices[] =
{
	//Top face
	0,1,3,  1,3,2,

	//bottomface
	4,5,7,  4,6,7,

	//front face
	2,3,7,  2,6,7,

	//rear face
	0,1,5,  0,4,5,

	//left face
	0,5,6,  0,2,6,

	//right face
	1,5,7,  1,3,7
};*/


// Vertices coordinates
Vertex vertices[] =
{ //               COORDINATES           /            COLORS          
	Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(1.0f, 0.0f, 0.0f)},
	Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f)},
	Vertex{glm::vec3(0.5f, 0.0f, -0.5f),  glm::vec3(0.0f, 1.0f, 0.0f)},
	Vertex{glm::vec3(0.5f, 0.0f,  0.5f),  glm::vec3(0.0f, 0.0f, 1.0f)},
	Vertex{glm::vec3(0.0f, 0.8f,  0.0f),  glm::vec3(1.0f, 1.0f, 1.0f)},
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



/*
// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS       //
	-0.5f, 0.0f,  0.5f,     1.00f, 0.00f, 0.00f,
	-0.5f, 0.0f, -0.5f,     0.00f, 1.00f, 0.00f,
	 0.5f, 0.0f, -0.5f,     0.00f, 0.00f, 1.00f,
	 0.5f, 0.0f,  0.5f,     1.00f, 1.00f, 0.00f,
	 0.0f, 0.8f,  0.0f,     1.00f, 1.00f, 1.0f,
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
};*/


int main() {
	// Initialize GLFW
	glfwInit();

	//Select the proper OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a window
	GLFWwindow* window = glfwCreateWindow(width, height, "3D Cellular automata", NULL, NULL);
	//Exit if an error occurs
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Set the windows to the current context (use the window)
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, height, width);


	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	// Create floor mesh
	Mesh triangle1(verts, ind);

	std::vector <Vertex> verts2(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind2(indices, indices + sizeof(indices) / sizeof(GLuint));
	// Create floor mesh
	Mesh triangle2(verts2, ind2);

	std::vector <Vertex> verts3(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind3(indices, indices + sizeof(indices) / sizeof(GLuint));
	// Create floor mesh
	Mesh triangle3(verts3, ind3);

	std::vector <Vertex> verts4(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind4(indices, indices + sizeof(indices) / sizeof(GLuint));
	// Create floor mesh
	Mesh triangle4(verts4, ind4);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	shaderProgram.activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	double prevTime = glfwGetTime();
	float rotation = 0.0f;
	Camera camera(width, height, glm::vec3(0.0f, 2.0f, 8.0f));

	while (!glfwWindowShouldClose(window))	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			prevTime = crntTime;
			rotation += 0.01f;
		};

		// Handles camera inputs
		camera.inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f, rotation);

		triangle1.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		triangle2.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		triangle3.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		triangle4.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));


		glfwSwapBuffers(window);
		glfwPollEvents(); // Check for events (Like close window)

	}

	// Delete all the objects we've created
	shaderProgram._delete();
	//Destroy everything and quit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
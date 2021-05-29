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

#include "Cube.h"
#include "Pyramid.h"
#include "CA3D.h"

unsigned int width = 720;
unsigned int height = 480;

Camera *camera = NULL;

void framebuffer_size_callback(GLFWwindow* window, int _width, int _height) {
	width = _width;
	height = _height;
	glViewport(0, 0, _width, _height);
	if (camera != NULL) {
		camera->updateSize(_width, _height);
	}
}

int main() {
	std::cout << "Hello world" << std::endl;
	CA3D::CA3D playground(50, 50);
	playground.initRule("4/4/5/M");

	while (true) {}

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "3D Cellular automata", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGL();
	glViewport(0, 0, width, height);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);

	Shader shaderProgram("default.vert", "default.frag");
	shaderProgram.activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));


	double prevTime = glfwGetTime();
	float rotation = 0.0f;
	camera = new Camera(width, height, glm::vec3(0.0f, 2.0f, 8.0f));

	Cube c(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Pyramid p(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	while (!glfwWindowShouldClose(window))	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60) {
			prevTime = crntTime;
			rotation += 0.01f;
		};

		camera->inputs(window);
		camera->updateMatrix(45.0f, 0.1f, 100.0f, rotation);

		c.draw(shaderProgram, *camera);
		p.draw(shaderProgram, *camera);

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
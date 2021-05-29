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

const unsigned int width = 800;
const unsigned int height = 800;


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

		// Handles camera inputs
		camera.inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f, rotation);

		c.draw(shaderProgram, camera);
		p.draw(shaderProgram, camera);
		//triangle1.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, -0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//triangle2.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//triangle3.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//triangle4.draw(shaderProgram, camera, glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.5f), glm::quat(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));


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
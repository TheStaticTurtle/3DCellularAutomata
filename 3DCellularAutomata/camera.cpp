#include "camera.h"



Camera::Camera(int width, int height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	this->position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane, float rotation)
{
	// Initializes matrices since otherwise they will be the null matrix
	this->rotation = rotation;
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right this->position
	view = glm::lookAt(this->position, this->position + this->orientation, this->up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;

}


void Camera::matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glm::mat4 cameraMatrixRotated = glm::rotate(this->cameraMatrix, glm::radians(this->rotation), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrixRotated));
}



void Camera::inputs(GLFWwindow* window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)            this->position += this->speed * this->orientation;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)            this->position += this->speed * -glm::normalize(glm::cross(this->orientation, this->up));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)            this->position += this->speed * -this->orientation;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)            this->position += this->speed * glm::normalize(glm::cross(this->orientation, this->up));
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)        this->position += this->speed * this->up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) this->position += this->speed * -this->up;
	
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)         this->speed = 0.04f;
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)  this->speed = 0.01f;
	
	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick) {
			glfwSetCursorPos(window, (this->width / 2), (this->height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;

		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = this->sensitivity * (float)(mouseY - (this->height / 2)) / this->height;
		float rotY = this->sensitivity * (float)(mouseX - (this->width / 2)) / this->width;

		// Calculates this->upcoming vertical change in the this->orientation
		glm::vec3 newOrientation = glm::rotate(this->orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->orientation, this->up)));

		// Decides whether or not the next vertical this->orientation is legal or not
		if (abs(glm::angle(newOrientation, this->up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			this->orientation = newOrientation;
		}

		// Rotates the this->orientation left and right
		this->orientation = glm::rotate(this->orientation, glm::radians(-rotY), this->up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end this->up roaming around
		glfwSetCursorPos(window, (this->width / 2), (this->height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}

void Camera::updateSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

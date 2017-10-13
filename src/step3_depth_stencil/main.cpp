#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Filesystem.h"
#include "Camera.h"
#include "utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>


void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const int windowWidth = 1280;
const int windowHeight = 720;

//camera
Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool  firstMouse = true;

//time
float deltatime = 0.0f;
float lastframe = 0.0f;

int main(int argc, char* argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LearnOpenGL", NULL, NULL);
	if (NULL == window)
	{
		std::cout << "glfw create window failed!" << std::endl;
		glfwTerminate();

		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//set cursor 
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//lamda
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){

		glViewport(0, 0, width, height);
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Shader shader(FileSystem::getPath("resources/shader/step3_depth_stencil/stencil_testing.vs").c_str(),
		FileSystem::getPath("resources/shader/step3_depth_stencil/stencil_testing.fs").c_str());

	Shader shaderSigleColor(FileSystem::getPath("resources/shader/step3_depth_stencil/stencil_testing.vs").c_str(),
		FileSystem::getPath("resources/shader/step3_depth_stencil/stencil_single_color.fs").c_str());

	unsigned int cubeVAO, cubeVBO;
	cubeVAO = Utility::genCubeVAO(cubeVBO);

	unsigned int planeVAO, planeVBO;
	planeVAO = Utility::genPlaneVAO(planeVBO);

	unsigned int cubeTexture = Utility::loadTexture(FileSystem::getPath("resources/marble.jpg").c_str());
	unsigned int floorTexture = Utility::loadTexture(FileSystem::getPath("resources/metal.png").c_str());

	shader.use();
	shader.setInt("texture1", 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltatime = currentTime - lastframe;
		lastframe = currentTime;

		processInput(window);

		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//set uniforms
		shaderSigleColor.use();
		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.mZoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		shaderSigleColor.setMatrix4("view", view);
		shaderSigleColor.setMatrix4("projection", projection);

		shader.use();
		shader.setMatrix4("projection", projection);
		shader.setMatrix4("view", view);

		// draw floor as normal, but don't write the floor to the stencil buffer, 
		// we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
		glStencilMask(0x00);

		//floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMatrix4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// 1st. render pass, draw objects as normal, writing to the stencil buffer
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.1f, -1.0f));
		shader.setMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.1f, 0.0f));
		shader.setMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
		// Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing 
		// the objects' size differences, making it look like borders.
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		shaderSigleColor.use();
		float scale = 1.1;
		// cubes
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.1f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSigleColor.setMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSigleColor.setMatrix4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltatime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltatime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltatime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		lastX = xpos;
		lastY = ypos;
	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

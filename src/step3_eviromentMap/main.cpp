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
#include <vector>
#include <map>

void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const int windowWidth = 1280;
const int windowHeight = 720;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
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

	// draw as wire frame
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	Shader shader(FileSystem::getPath("resources/shader/step3_eviromentMap/cubemap.vs").c_str(),
		FileSystem::getPath("resources/shader/step3_eviromentMap/cubemap.fs").c_str());

	Shader skyShader(FileSystem::getPath("resources/shader/step3_eviromentMap/skybox.vs").c_str(),
		FileSystem::getPath("resources/shader/step3_eviromentMap/skybox.fs").c_str());


	unsigned int cubeVAO, cubeVBO;
	cubeVAO = Utility::genCubeWithNormalVAO(cubeVBO);

	unsigned int skyVAO, skyVBO;
	skyVAO = Utility::genSkyVAO(skyVBO);

	std::vector<std::string> faces
	{
		FileSystem::getPath("resources/textures/skybox/right.jpg"),
		FileSystem::getPath("resources/textures/skybox/left.jpg"),
		FileSystem::getPath("resources/textures/skybox/top.jpg"),
		FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
		FileSystem::getPath("resources/textures/skybox/back.jpg"),
		FileSystem::getPath("resources/textures/skybox/front.jpg")
	};
	unsigned int cubemapTexture = Utility::loadCubeTexture(faces);

	shader.use();
	shader.setInt("skybox", 0);

	skyShader.use();
	skyShader.setInt("skybox", 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltatime = currentTime - lastframe;
		lastframe = currentTime;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//draw scene as normal
		shader.use();
		glm::mat4 model;
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0, 0.5, 0.0));
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.mZoom, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		shader.setMatrix4("model", model);
		shader.setMatrix4("view", view);
		shader.setMatrix4("projection", projection);
		shader.setVec3("cameraPos", camera.mPosition);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyShader.setMatrix4("view", view);
		skyShader.setMatrix4("projection", projection);
		// skybox cube
		glBindVertexArray(skyVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyVBO);

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

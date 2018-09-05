#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Filesystem.h"
#include "Camera.h"
#include "model.h"
#include "utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <iostream>
#include <string>


void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void initData();
unsigned int loadTexture(const char* path);

const int windowWidth = 800;
const int windowHeight = 600;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool  firstMouse = true;

//time
float deltatime = 0.0f;
float lastframe = 0.0f;

unsigned int boundingVAO;

glm::vec3 gMin;
glm::vec3 gMax;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;

Model* ourModel;

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


	Shader shader(FileSystem::getPath("resources/shader/load_mesh/loadmesh.vs").c_str(),
		FileSystem::getPath("resources/shader/load_mesh/loadmesh.fs").c_str());

	Shader boundingBoxShader(FileSystem::getPath("resources/shader/load_mesh/bounding.vs").c_str(),
		FileSystem::getPath("resources/shader/load_mesh/bounding.fs").c_str());

	ourModel = new Model(FileSystem::getPath("resources/nanosuit/nanosuit.obj"),false);

	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		deltatime = currentTime - lastframe;
		lastframe = currentTime;

		processInput(window);

		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		// view/projection transformations
		projection = glm::perspective(glm::radians(camera.mZoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		shader.setMatrix4("projection", projection);
		shader.setMatrix4("view", view);

		// render the loaded model
		glm::mat4 modelTmp;
		modelTmp = glm::translate(modelTmp, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		modelTmp = glm::scale(modelTmp, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(modelTmp, (float)glfwGetTime(), glm::vec3(1.0, 0.5, 0.0));
		shader.setMatrix4("model", model);
		ourModel->Draw(shader);

		ourModel->mAABB.drawBoundingBox(model, view, projection,boundingBoxShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

unsigned int loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed, texture path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
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

		glm::vec3 ray_origin;
		glm::vec3 ray_direction;
		ScreenPosToWorldRay(
			xpos, ypos,
			windowWidth, windowHeight,
			view,
			projection,
			ray_origin,
			ray_direction
			);

		Ray ray(ray_origin, ray_direction);

		if (ourModel->mAABB.hit(ray, model))
		{
			std::cout << "picked" << std::endl;
			//::MessageBox(NULL, "模型已选中", "模型已选中", NULL);
		}

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

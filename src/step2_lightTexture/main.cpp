#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Filesystem.h"
#include "Camera.h"

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

unsigned int cubeVAO,lightVAO, VBO;

unsigned int texture1,texture2;

//camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool  firstMouse = true;

//time
float deltatime = 0.0f;
float lastframe = 0.0f;

//lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

	Shader lightShader(FileSystem::getPath("resources/shader/step2_lightTexture/lightMap.vs").c_str(),
		FileSystem::getPath("resources/shader/step2_lightTexture/lightMap.fs").c_str());

	Shader lampShader(FileSystem::getPath("resources/shader/step2_lightTexture/lamp.vs").c_str(),
		FileSystem::getPath("resources/shader/step2_lightTexture/lamp.fs").c_str());
	
	initData();
	texture1 = loadTexture(FileSystem::getPath("resources/container2.png").c_str());
	texture2 = loadTexture(FileSystem::getPath("resources/container2_specular.png").c_str());
	
	
	while (!glfwWindowShouldClose(window))
	{	
		float currentTime = glfwGetTime();
		deltatime = currentTime - lastframe;
		lastframe = currentTime;

		processInput(window);

		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.use();
		lightShader.setInt("material.diffuse", 0);
		lightShader.setInt("material.specular", 1);
		//lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightShader.setVec3("light.position", lightPos);
		lightShader.setVec3("viewPos", camera.mPosition);

		//light properties
		lightShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//material properties
		lightShader.setFloat("material.shininess", 64.0f);

		//Model View Projection
		glm::mat4 projections;
		projections = glm::perspective(glm::radians(camera.mZoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		lightShader.setMatrix4("projection", projections);

		glm::mat4 mods;
		//mods = glm::rotate(mods, -5.0f, glm::vec3(0.0, 0.0, 1.0));
		//mods = glm::rotate(mods, 15.0f, glm::vec3(0.0, 1.0, 0.0));
		mods = glm::rotate(mods, (float)glfwGetTime(), glm::vec3(1.0, 0.5, 0.0));
		lightShader.setMatrix4("model", mods);

		glm::mat4 views = camera.GetViewMatrix();
		lightShader.setMatrix4("view", views);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//draw lamb object
		lampShader.use();
		lampShader.setMatrix4("projection", projections);
		lampShader.setMatrix4("view", views);
		mods = glm::mat4();
		mods = glm::translate(mods, lightPos);
		mods = glm::scale(mods, glm::vec3(0.2f));
		lampShader.setMatrix4("model", mods);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

void initData()
{
	//vertex buffer
	float vertexData[] =
	{  
		//position             //normal            //UV
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, 0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, 0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, 1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, 1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, 1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, 0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, 0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, 0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f, 1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f, 1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, 0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, 0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f, 0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f, 1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, 0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, 1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, 1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, 1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, 0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, 0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, 0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, 1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, 1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, 1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, 0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, 0.0f,  1.0f
	};

	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//second scene
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//only need to bind to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,format, GL_UNSIGNED_BYTE, data);
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
	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
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

	if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		lastX = xpos;
		lastY = ypos;
	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
		
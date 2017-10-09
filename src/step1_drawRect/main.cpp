#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Filesystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>

void processInput(GLFWwindow *window);
void initData();

const int windowWidth = 800;
const int windowHeight = 600;

unsigned int VAO, VBO, EBO;
unsigned int texture1,texture2;

float blendFactor = 0.2;

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
	
	//lamda
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){

		glViewport(0, 0, width, height);
	});

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shader(FileSystem::getPath("resources/shader/step1_drawRect/vertexShader").c_str(),FileSystem::getPath("resources/shader/step1_drawRect/fragmentShader").c_str());
	initData();
	shader.use();
	
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{	
		processInput(window);

		//Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		shader.setFloat("factor", blendFactor);

		//first transform
		//scale -> rotation -> translation
		glm::mat4 scals, rotats, trans;
		scals = glm::scale(scals, glm::vec3(0.5, 0.5, 0.5));
		rotats = glm::rotate(rotats, glm::radians((float)glfwGetTime() * 50), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::translate(trans, glm::vec3(0.75f, 0.75f, 0.0f));

		unsigned int transformLoc = glGetUniformLocation(shader.program, "transform");
		if (transformLoc)
		{
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans*rotats*scals));
		}

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//second transform
		scals = glm::mat4();
		rotats = glm::mat4();
		trans = glm::mat4();
		scals = glm::scale(scals, glm::vec3(0.5, 0.5, 0.5));
		rotats = glm::rotate(rotats, glm::radians(-(float)glfwGetTime() * 50), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::translate(trans, glm::vec3(-0.75f, -0.75f, 0.0f));

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans*rotats*scals));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void initData()
{
	//vertex buffer
	float vertexData[] =
	{    
		//position        //color       //uv
		-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,
		 0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,
		 0.5f, 0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
		-0.5f, 0.5f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f
	};

	//index buffer
	unsigned int indexData[] =
	{
		0,1,3,
		3,1,2
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//texture1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	//wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE/*GL_REPEAT*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE/*GL_REPEAT*/);

	//filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load
	int imgWidth, imgHeight, imgChannel;
	unsigned char* pData = stbi_load(FileSystem::getPath("resources/wall.jpg").c_str(), &imgWidth, &imgHeight, &imgChannel, 0);
	if (pData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image1 failed." << std::endl;
	}
	stbi_image_free(pData);

	//texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load
	pData = stbi_load(FileSystem::getPath("resources/awesomeface.png").c_str(), &imgWidth, &imgHeight, &imgChannel, 0);
	if (pData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "load image2 failed." << std::endl;
	}
	stbi_image_free(pData);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		blendFactor += 0.01f;

		if (blendFactor >= 1.0f)
		{
			blendFactor = 1.0f;
		}
	}

	if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		blendFactor -= 0.01f;

		if (blendFactor <= 0.0f)
		{
			blendFactor = 0.0f;
		}
	}
}
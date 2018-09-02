#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


#include "Shader.h"
#include "Filesystem.h"


#include <iostream>
#include <string>

void processInput(GLFWwindow *window);
void initData();

const int windowWidth = 800;
const int windowHeight = 600;

unsigned int VAO, VBO;




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

	//glEnable(GL_CULL_FACE);


	Shader shader(FileSystem::getPath("resources/shader/test/test.vs").c_str(),FileSystem::getPath("resources/shader/test/test.fs").c_str());
	initData();
	shader.use();
	

	while (!glfwWindowShouldClose(window))
	{	

		processInput(window);

		//Render
		glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//glBindVertexArray(VAO);
		GLfloat vVertices[] = { 
				0.0f,  0.5f, 0.0f,
			   -0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f };

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

void initData()
{
	//vertex buffer
	float vertexData[] =
	{    
		 1.0f, 1.0f, -0.5f,
		 1.0f, -1.0f, -0.5f,
		-1.0f, -1.0f, -0.5f,
		
		

		/* -0.5f, -0.5f, -0.5f,
		  2.0f, -0.5f, -0.5f,
		  0.0f,  0.5f, -0.5f,*/
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
		
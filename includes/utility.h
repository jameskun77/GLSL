#pragma once

#include "glad/glad.h"

#include "stb_image.h"

#include <iostream>
#include <string>
#include <vector>

class Utility
{
public:
	static unsigned int loadTexture(const char* path)
	{
		unsigned int textureID;
		glGenTextures(1,&textureID);
		
		int width,height,nComponent;
		
		unsigned char* data = stbi_load(path,&width,&height,&nComponent,0);
		if(data)
		{
			GLenum format;
			if(nComponent == 1)
				format = GL_RED;
			else if(nComponent == 3)
				format = GL_RGB;
			else if(nComponent == 4)
				format = GL_RGBA;
			
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout<<"Texture load fail, path: "<<path<<std::endl;
			stbi_image_free(data);
		}
		
		return textureID;
	}

	static unsigned int loadCubeTexture(std::vector<std::string> faces)
	{
		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed, path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return textureID;
	}
	
	static unsigned int genCubeVAO(unsigned int& VBO)
	{
		unsigned int cubeVAO;
		
		float cubeVertices[] = {
        // positions          //UV
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		
		glGenVertexArrays(1,&cubeVAO);
		glGenBuffers(1, &VBO);
		
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(cubeVertices),cubeVertices,GL_STATIC_DRAW);
		
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		
		return cubeVAO;
	}

	static unsigned int genCubeWithNormalVAO(unsigned int& VBO)
	{
		unsigned int cubeNormalVAO;

		float cubeVertices[] = {
			// positions          //UV
			// positions          // normals
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
			};

		glGenVertexArrays(1, &cubeNormalVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(cubeNormalVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);

		return cubeNormalVAO;
	}
	
	static unsigned int genPlaneVAO(unsigned int& VBO)
	{
		unsigned int planeVAO;
		
		 float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode)
							  // this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
		};
		
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &VBO);
		
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
		
		return planeVAO;
	}

	static unsigned int genPlaneWithNormalVAO(unsigned int& VBO)
	{
		unsigned int planeVAO;

		float planeVertices[] = {
			
			// positions           // normals         // texcoords
			 10.0f, -0.5f,  10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
			-10.0f, -0.5f,  10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
			-10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f, 0.0f, 1.0f, 0.0f, 10.0f, 10.0f
		};

		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);

		return planeVAO;
	}

	static unsigned int genPlaneWithNormalUVVAO(unsigned int& VBO)
	{
		unsigned int planeVAO;

		float planeVertices[] = {

			 25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
			-25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

			 25.0f, -0.5f,  25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
			-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
			 25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 10.0f
		};

		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);

		return planeVAO;
	}

	static unsigned int genQuadVAO(unsigned int& VBO)
	{
		unsigned int quadVAO;

		float quadVertices[] = {
			
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

			0.0f,  0.5f, 0.0f, 0.0f, 0.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
			1.0f,  0.5f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

		return quadVAO;
	}

	static unsigned int genRectVAO(unsigned int& VBO)
	{
		unsigned int rectVAO;

		float quadVertices[] = { //vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
			};

		glGenVertexArrays(1, &rectVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(rectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		return rectVAO;
	}

	static unsigned int genSkyVAO(unsigned int& VBO)
	{
		unsigned int skyVAO;

		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			 1.0f, 1.0f, -1.0f,
			 1.0f, 1.0f,  1.0f,
			 1.0f, 1.0f,  1.0f,
			-1.0f, 1.0f,  1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
			};

		glGenVertexArrays(1, &skyVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(skyVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		return skyVAO;
	}

	static unsigned int genAABBVAO(glm::vec3 minPos,glm::vec3 maxPos)
	{
		static unsigned int AABBVAO;
		static unsigned int AABBVB0;

		float AABBVertices[] = {
			// positions          
			
			minPos.x,maxPos.y,minPos.z,
			minPos.x,minPos.y,minPos.z,
			maxPos.x,minPos.y,minPos.z,
			maxPos.x,minPos.y,minPos.z,
			maxPos.x,maxPos.y,minPos.z,
			minPos.x,maxPos.y,minPos.z,

			minPos.x, minPos.y, maxPos.z,
			minPos.x, minPos.y, minPos.z,
			minPos.x, maxPos.y, minPos.z,
			minPos.x, maxPos.y, minPos.z,
			minPos.x, maxPos.y, maxPos.z,
			minPos.x, minPos.y, maxPos.z,

			
			maxPos.x, minPos.y, minPos.z,
			maxPos.x, minPos.y, maxPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			maxPos.x, maxPos.y, minPos.z,
			maxPos.x, minPos.y, minPos.z,

			minPos.x, minPos.y, maxPos.z,
			minPos.x, maxPos.y, maxPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			maxPos.x, minPos.y, maxPos.z,
			minPos.x, minPos.y, maxPos.z,

			minPos.x, maxPos.y, minPos.z,
			maxPos.x, maxPos.y, minPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			maxPos.x, maxPos.y, maxPos.z,
			minPos.x, maxPos.y, maxPos.z,
			minPos.x, maxPos.y, minPos.z,

			minPos.x, minPos.y, minPos.z,
			minPos.x, minPos.y, maxPos.z,
			maxPos.x, minPos.y, minPos.z,
			maxPos.x, minPos.y, minPos.z,
			minPos.x, minPos.y, maxPos.z,
			maxPos.x, minPos.y, maxPos.z
		};

		glGenVertexArrays(1, &AABBVAO);
		glGenBuffers(1, &AABBVB0);

		glBindVertexArray(AABBVAO);
		glBindBuffer(GL_ARRAY_BUFFER, AABBVB0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(AABBVertices), &AABBVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		return AABBVAO;
	}
};
#pragma once

#include "glad/glad.h"

#include "assimp/Importer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct Vertex
{
	//position
	glm::vec3 mPosition; 
	//normal
	glm::vec3 mNormal;
	//UV
	glm::vec2 mTexCoords;
	//tangent ÇÐÏß
	glm::vec3 mTangent;
	//bitangent
	glm::vec3 mBitangent;

};

struct Texture
{
	unsigned int id;
	string type;
	aiString path;
};

class Mesh
{
public:
	vector<Vertex> mVertices;
	vector<unsigned int> mIndices;
	vector<Texture> mTextures;
	unsigned int VAO;

	Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, vector<Texture>& textures)
	{
		this->mVertices = vertices;
		this->mIndices = indices;
		this->mTextures = textures;

		_setupMesh();
	}

	void Draw(Shader shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < mTextures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			stringstream ss;
			string number;
			string name = mTextures[i].type;

			if (name == "texture_diffuse")
				ss << diffuseNr++;
			else if (name == "texture_specular")
				ss << specularNr++;
			else if (name == "texture_normal")
				ss << normalNr++;
			else if (name == "texture_height")
				ss << heightNr++;

			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
		}

		//draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int VBO, EBO;

	void _setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTexCoords));
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTangent));
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mBitangent));
		glEnableVertexAttribArray(4);

		glBindVertexArray(0);
	}
};
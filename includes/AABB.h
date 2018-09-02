
#ifndef _AABB_H__
#define _AABB_H__

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"

class AABB
{
public:
	AABB() :
		mVAO(0),
		minPos(FLT_MAX, FLT_MAX, FLT_MAX),
		maxPos(FLT_MIN, FLT_MIN, FLT_MIN)
	{

	}

	~AABB(){}

	void add(const glm::vec3& pos)
	{
		if (pos.x < minPos.x)minPos.x = pos.x;
		if (pos.y < minPos.y)minPos.y = pos.y;
		if (pos.z < minPos.z)minPos.z = pos.z;
		if (pos.x > maxPos.x)maxPos.x = pos.x;
		if (pos.y > maxPos.y)maxPos.y = pos.y;
		if (pos.z > maxPos.z)maxPos.z = pos.z;
	}

	void drawBoundingBox(glm::mat4& model,glm::mat4& view,glm::mat4& projection,Shader& shader)
	{
		if (mVAO == 0)
		{
			float AABBVertices[] = {
				// positions          
				minPos.x, maxPos.y, minPos.z,
				minPos.x, minPos.y, minPos.z,
				maxPos.x, minPos.y, minPos.z,
				maxPos.x, minPos.y, minPos.z,
				maxPos.x, maxPos.y, minPos.z,
				minPos.x, maxPos.y, minPos.z,

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

			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mVBO);

			glBindVertexArray(mVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(AABBVertices), &AABBVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		shader.use();
		shader.setMatrix4("model", model);
		shader.setMatrix4("view", view);
		shader.setMatrix4("projection", projection);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

private:
	glm::vec3 minPos;
	glm::vec3 maxPos;
	unsigned int mVAO;
	unsigned int mVBO;
};

#endif // !_AABB_H__


#ifndef _AABB_H__
#define _AABB_H__

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"

class Ray
{
public:
	Ray(glm::vec3 pos, glm::vec3 dir)
	{
		position = pos;
		direction = dir;
	}

public:
	glm::vec3 position;
	glm::vec3 direction;
};

void ScreenPosToWorldRay(
	int mouseX, int mouseY,             // Mouse position, in pixels, from bottom-left corner of the window
	int screenWidth, int screenHeight,  // Window size, in pixels
	glm::mat4 ViewMatrix,               // Camera position and orientation
	glm::mat4 ProjectionMatrix,         // Camera parameters (ratio, field of view, near and far planes)
	glm::vec3& out_origin,              // Ouput : Origin of the ray. /!\ Starts at the near plane, so if you want the ray to start at the camera's position instead, ignore this.
	glm::vec3& out_direction            // Ouput : Direction, in world space, of the ray that goes "through" the mouse.
	){

	// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
	glm::vec4 lRayStart_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
		-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
		1.0f
		);
	glm::vec4 lRayEnd_NDC(
		((float)mouseX / (float)screenWidth - 0.5f) * 2.0f,
		((float)mouseY / (float)screenHeight - 0.5f) * 2.0f,
		0.0,
		1.0f
		);


	// The Projection matrix goes from Camera Space to NDC.
	// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
	glm::mat4 InverseProjectionMatrix = glm::inverse(ProjectionMatrix);

	// The View Matrix goes from World Space to Camera Space.
	// So inverse(ViewMatrix) goes from Camera Space to World Space.
	glm::mat4 InverseViewMatrix = glm::inverse(ViewMatrix);

	glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
	glm::vec4 lRayStart_world = InverseViewMatrix       * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
	glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
	glm::vec4 lRayEnd_world = InverseViewMatrix       * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


	// Faster way (just one inverse)
	//glm::mat4 M = glm::inverse(ProjectionMatrix * ViewMatrix);
	//glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
	//glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


	glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
	lRayDir_world = glm::normalize(lRayDir_world);


	out_origin = glm::vec3(lRayStart_world);
	out_direction = glm::normalize(lRayDir_world);
}

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
	
	bool hit(const Ray& ray,glm::mat4 modelMatrix) 
	{
		glm::vec4 tmpMinPos = modelMatrix * glm::vec4(minPos.x, minPos.y, minPos.z, 1.0);
		glm::vec4 tmpMaxPos = modelMatrix * glm::vec4(maxPos.x, maxPos.y, maxPos.z, 1.0);


		double ox = ray.position.x; double oy = ray.position.y; double oz = ray.position.z;
		double dx = ray.direction.x; double dy = ray.direction.y; double dz = ray.direction.z;
		double tx_min, ty_min, tz_min;
		double tx_max, ty_max, tz_max;

		//x0,y0,z0为包围体的最小顶点
		//x1,y1,z1为包围体的最大顶点
		if (abs(dx) < 0.000001f)
		{
			//若射线方向矢量的x轴分量为0且原点不在盒体内
			if (ox < tmpMinPos.x || ox > tmpMaxPos.x)
				return false;
		}
		else
		{
			if (dx >= 0)
			{
				tx_min = (tmpMinPos.x - ox) / dx;
				tx_max = (tmpMaxPos.x - ox) / dx;
			}
			else
			{
				tx_min = (tmpMaxPos.x - ox) / dx;
				tx_max = (tmpMinPos.x - ox) / dx;
			}

		}


		if (abs(dy) < 0.000001f)
		{
			//若射线方向矢量的x轴分量为0且原点不在盒体内
			if (oy < tmpMinPos.y || oy > tmpMaxPos.y)
				return false;
		}
		else
		{
			if (dy >= 0)
			{
				ty_min = (tmpMinPos.y - oy) / dy;
				ty_max = (tmpMaxPos.y - oy) / dy;
			}
			else
			{
				ty_min = (tmpMaxPos.y - oy) / dy;
				ty_max = (tmpMinPos.y - oy) / dy;
			}

		}


		if (abs(dz) < 0.000001f)
		{
			//若射线方向矢量的x轴分量为0且原点不在盒体内
			if (oz < tmpMinPos.z || oz > tmpMaxPos.z)
				return false;
		}
		else
		{
			if (dz >= 0)
			{
				tz_min = (tmpMinPos.z - oz) / dz;
				tz_max = (tmpMaxPos.z - oz) / dz;
			}
			else
			{
				tz_min = (tmpMaxPos.z - oz) / dz;
				tz_max = (tmpMinPos.z - oz) / dz;
			}

		}

		double t0, t1;

		//光线进入平面处（最靠近的平面）的最大t值 
		t0 = max(tz_min, max(tx_min, ty_min));

		//光线离开平面处（最远离的平面）的最小t值
		t1 = min(tz_max, min(tx_max, ty_max));

		return t0 < t1;
	}


public:
	glm::vec3 minPos;
	glm::vec3 maxPos;
	unsigned int mVAO;
	unsigned int mVBO;
};

#endif // !_AABB_H__

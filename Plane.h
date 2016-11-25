// Plane.h
//
//////////////////////////////////////////////////////////////////////
#ifndef _PLANE_
#define _PLANE_

#define _USE_MATH_DEFINES
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace glm;

class Plane  
{

public:

	vec3 normal,point;
	float d;


	Plane(vec3 v1,  vec3 v2,  vec3 v3);
	Plane(void);
	~Plane();

	void set3Points( vec3 v1,  vec3 v2,  vec3 v3);
	float distance(vec3 p);
    void setNormalAndPoint(vec3 normal, vec3 point);

	void print();

};


#endif
// Plane.cpp
//
//////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include "Plane.h"
using namespace glm;

Plane::Plane( vec3 v1,  vec3 v2,  vec3 v3) {

	set3Points(v1,v2,v3);
}


Plane::Plane() {}

Plane::~Plane() {}


void Plane::set3Points( vec3 v1,  vec3 v2,  vec3 v3) {


	vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

	normal = cross(aux2, aux1);

	normal = normalize(normal);
    point = v2;
	d = -dot(normal,point);
}


void Plane::setNormalAndPoint(vec3 normal, vec3 point) {
    
    this->normal = normal;
    this->normal = normalize(this->normal);
    d = -(dot(this->normal,point));
}
	

float Plane::distance(vec3 p) {

	return (d + dot(this->normal,p));
}

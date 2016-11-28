/* ------------------------------------------------------

 View Frustum - Lighthouse3D

  -----------------------------------------------------*/


#ifndef _FRUSTUMG_
#define _FRUSTUMG_
#include "Plane.h"
#include <iostream>





class FrustumG 
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};


public:

    enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];
    int lastWall = 0;
    
	vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	FrustumG();
	~FrustumG();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(vec3 p, vec3 l, vec3 u);
    void setCubePlanes(float length);
    bool sphereInFrustum(vec3 p, float r);
    int ballHitWall(vec3 p, float r);
    vec3 reflection(vec3 curDir, int plane);


};


#endif

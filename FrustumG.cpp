/* ------------------------------------------------------

 View Frustum - Lighthouse3D

  -----------------------------------------------------*/

#include "FrustumG.h"
#include <math.h>
using namespace std;


#define ANG2RAD 3.14159265358979323846/180.0



FrustumG::FrustumG() {}

FrustumG::~FrustumG() {}

void FrustumG::setCamInternals(float angle, float ratio, float nearD, float farD) {

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle* ANG2RAD * 0.5) ;
	nh = nearD * tang;
	nw = nh * ratio; 
	fh = farD  * tang;
	fw = fh * ratio;


}


void FrustumG::setCamDef(vec3 p, vec3 l, vec3 u) {

	vec3 dir,nc,fc,X,Y,Z;

	Z = p - l;
	Z = normalize(Z);

	X = cross(u, Z);
	X = normalize(X);

	Y = cross(Z,X);
    //Y = normalize(Y);
    
	nc = p - Z * nearD;
	fc = p - Z * farD;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

    pl[TOP].set3Points(ntr,ntl,ftl);
    pl[BOTTOM].set3Points(nbl,nbr,fbr);
    pl[LEFT].set3Points(ntl,nbl,fbl);
    pl[RIGHT].set3Points(nbr,ntr,fbr);
    pl[NEARP].set3Points(ntl,ntr,nbr);
    pl[FARP].set3Points(ftr,ftl,fbl);
}

void FrustumG::setCubePlanes(float len) {
    vec3 top1(2,len/2, 1); vec3 top2(1, len/2, 1); vec3 top3(1,len/2,0);
    vec3 bot1(2,-len/2, 1); vec3 bot2(1, -len/2, 0); vec3 bot3(1,-len/2,1);
    vec3 left1(-len/2, 2, 1); vec3 left2(-len/2, 1, 1); vec3 left3(-len/2, 1,0);
    vec3 right1(len/2, 2, 1); vec3 right2(len/2, 1, 0); vec3 right3(len/2, 1,1);
    vec3 near1(2, 1, len/2); vec3 near2(1, 1, len/2); vec3 near3(1,2,len/2);
    vec3 far1(1, 2, -len/2); vec3 far2(1, 1, -len/2); vec3 far3(2,1,-len/2);

    
    
    pl[TOP].set3Points(top1, top2, top3);
//    Plane pa= pl[TOP];
//    cout<< "plane"<< pa.normal.x <<","<<pa.normal.y<< ","<<pa.normal.z<<endl;

    pl[BOTTOM].set3Points(bot1,bot2,bot3);
    pl[LEFT].set3Points(left1,left2,left3);
    pl[RIGHT].set3Points(right1,right2,right3);
    pl[NEARP].set3Points(near1, near2, near3);
    pl[FARP].set3Points(far1, far2, far3);
    
    
}





/*return index of walls that ball hit
 if no collision, return -1
 */
int FrustumG::ballHitWall(vec3 p, float r) {
    
    float distance;
    int i = 0;
    int curWall = lastWall;
    //cout<< p.x <<","<<p.y<< ","<<p.z<<endl;
    while(i<6) {
        curWall = curWall%6;
        Plane pa = pl[curWall];
        //cout<< "plane"<< pa.normal.x <<","<<pa.normal.y<< ","<<pa.normal.z<<endl;
        distance = pl[curWall].distance(p);
        //cout<< distance << endl;
        if(distance < r) {
            lastWall = (curWall+1)%6;
            return curWall;
        }
        curWall++;
        i++;
    }
    lastWall = curWall%6;
    return -1;
}

/*given hit wall and current direction, calculate reflect direction*/
vec3 FrustumG::reflection(vec3 curDir, int plane) {
    vec3 I = normalize(curDir);
    vec3 R = reflect(I, normalize(pl[plane].normal));
    return R;
}

bool FrustumG::sphereInFrustum(vec3 p, float r) {
    
    int isCollide = false;
    float distance;
    
    for(int i=0; i < 6; i++) {
        distance = pl[i].distance(p);
        
        if (distance < -6.7f) {
            return false;
        }
    }
    return true;
    
}



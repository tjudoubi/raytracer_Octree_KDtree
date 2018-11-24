#include "stdafx.h"
#include "Primitive.h"

//////////////////////////////////////////////////////////////
// 计算球体是否与包围盒相交
// box	- 包围盒
//////////////////////////////////////////////////////////////
bool Sphere::IntersectAABB(const AABB &box)
{
	double d = 0;
	Vector3D v1 = box.p1;
	Vector3D v2 = box.p2;
	for( int i=0; i<3; i++ )
	{
		if( Center[i] < v1[i] )
			d += ( Center[i] - v1[i] ) * ( Center[i] - v1[i] );
		else if( Center[i] > v2[i] )
			d += ( Center[i] - v2[i] ) * ( Center[i] - v2[i] );
	}
	return ( d <= Radius * Radius );
}

//////////////////////////////////////////////////////////////
// 计算球体是否与某条光线相交
// a_Ray	- 光线
// a_Dist	- 当前的最近距离
//////////////////////////////////////////////////////////////
bool Sphere::Intersect(const Ray& a_Ray, double& a_Dist)
{
	//----------------------------- your code here --------------------------------
	Vector3D v = a_Ray.o - Center;
	double b = -Dot(v, a_Ray.d);
	double det = (b * b) - Dot(v, v) + SqRadius;
	bool retval = false;
	if (det > 0)
	{
		det = sqrt(det);
		double i1 = b - det;
		double i2 = b + det;
		if (i2 > 0)
		{
			if (i1 < 0)
			{
				if (i2 < a_Dist)
				{
					a_Dist = i2;
					retval = false;
				}
			}
			else
			{
				if (i1 < a_Dist)
				{
					a_Dist = i1;
					retval = true;
				}
			}
		}
	}
	return retval;
	return false;

	//----------------------------- end of your code ------------------------------
}

//////////////////////////////////////////////////////////////
// 计算光线是否与包围盒相交
//////////////////////////////////////////////////////////////
bool RayIntersectAABB( const Ray &r, const AABB &box, double &tMin, double &tMax )
{
	double dist[6];
	Vector3D ip[6], v1 = box.p1, v2 = box.p2;
	bool ret = false;
	dist[0] = dist[1] = dist[2] = dist[3] = dist[4] = dist[5] = -1;
	if( r.d.x != 0 )
	{
		double rc = 1 / r.d.x;
		dist[0] = ( v1.x - r.o.x ) * rc;
		dist[3] = ( v2.x - r.o.x ) * rc;
	}
	if( r.d.y != 0 )
	{
		double rc = 1 /  r.d.y;
		dist[1] = ( v1.y - r.o.y ) * rc;
		dist[4] = ( v2.y - r.o.y ) * rc;
	}
	if( r.d.z != 0 )
	{
		double rc = 1 /  r.d.z;
		dist[2] = ( v1.z - r.o.z ) * rc;
		dist[5] = ( v2.z - r.o.z ) * rc;
	}

	double tMinTmp = DBL_MAX, tMaxTmp = -1e20;
	for( int i=0; i<6; i++ )
	{
		//if( dist[i] >= 0 )
		//{
		ip[i] = r.o + r.d * dist[i];
		if( ip[i].x > (v1.x - EPSILON) && ip[i].x < (v2.x + EPSILON) &&
			ip[i].y > (v1.y - EPSILON) && ip[i].y < (v2.y + EPSILON) &&
			ip[i].z > (v1.z - EPSILON) && ip[i].z < (v2.z + EPSILON) )
	
		{
			if( dist[i] < tMinTmp )
			{
				ret = true;
				tMinTmp = dist[i];
			}
			if( dist[i] > tMaxTmp )
			{
				tMaxTmp = dist[i];
			}
		}
	//	}
	}
	if( ret ) { tMin = tMinTmp; tMax = tMaxTmp; }
	return ret;

}

//***** Triangle-AABB test *****//
//
// from web

#define X 0
#define Y 1
#define Z 2

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0]; 

#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

#define FINDMINMAX(x0,x1,x2,min,max) \
  min = max = x0;   \
  if(x1<min) min=x1;\
  if(x1>max) max=x1;\
  if(x2<min) min=x2;\
  if(x2>max) max=x2;

bool planeBoxOverlap(Vector3D normal,double d, Vector3D maxbox)
{
  int q;
  double vmin[3],vmax[3];
  for(q=X;q<=Z;q++)
  {
    if(normal[q]>0.0f)
    {
      vmin[q]=-maxbox[q];
      vmax[q]=maxbox[q];
    }
    else
    {
      vmin[q]=maxbox[q];
      vmax[q]=-maxbox[q];
    }
  }
  if(DOT(normal,vmin)+d>0.0f) return false;
  if(DOT(normal,vmax)+d>=0.0f) return true;
  
  return false;
}


/*======================== X-tests ========================*/
#define AXISTEST_X01(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			       	   \
	p2 = a*v2[Y] - b*v2[Z];			       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_X2(a, b, fa, fb)			   \
	p0 = a*v0[Y] - b*v0[Z];			           \
	p1 = a*v1[Y] - b*v1[Z];			       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[Y] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

/*======================== Y-tests ========================*/
#define AXISTEST_Y02(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p2 = -a*v2[X] + b*v2[Z];	       	       	   \
        if(p0<p2) {min=p0; max=p2;} else {min=p2; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_Y1(a, b, fa, fb)			   \
	p0 = -a*v0[X] + b*v0[Z];		      	   \
	p1 = -a*v1[X] + b*v1[Z];	     	       	   \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Z];   \
	if(min>rad || max<-rad) return false;

/*======================== Z-tests ========================*/

#define AXISTEST_Z12(a, b, fa, fb)			   \
	p1 = a*v1[X] - b*v1[Y];			           \
	p2 = a*v2[X] - b*v2[Y];			       	   \
        if(p2<p1) {min=p2; max=p1;} else {min=p1; max=p2;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return false;

#define AXISTEST_Z0(a, b, fa, fb)			   \
	p0 = a*v0[X] - b*v0[Y];				   \
	p1 = a*v1[X] - b*v1[Y];			           \
        if(p0<p1) {min=p0; max=p1;} else {min=p1; max=p0;} \
	rad = fa * boxhalfsize[X] + fb * boxhalfsize[Y];   \
	if(min>rad || max<-rad) return false;

bool TriIntersectAABB( const Point &point1,const Point &point2, const Point &point3, const AABB &box )
{

  /*    use separating axis theorem to test overlap between triangle and box */
  /*    need to test for overlap in these directions: */
  /*    1) the {x,y,z}-directions (actually, since we use the AABB of the triangle */
  /*       we do not even need to test these) */
  /*    2) normal of the triangle */
  /*    3) crossproduct(edge from tri, {x,y,z}-directin) */
  /*       this gives 3x3=9 more tests */
	Vector3D boxhalfsize( box.w() * 0.5, box.h() * 0.5, box.d() * 0.5 );
	Point boxcenter = box.p1 + boxhalfsize;
	Point v0,v1,v2;
	//   double axis[3];
	double min,max,d,p0,p1,p2,rad,fex,fey,fez;  
	Vector3D normal,e0,e1,e2;

	/* This is the fastest branch on Sun */
	/* move everything so that the boxcenter is in (0,0,0) */
	SUB(v0,point1,boxcenter);
	SUB(v1,point2,boxcenter);
	SUB(v2,point3,boxcenter);

	/* compute triangle edges */
	SUB(e0,v1,v0);      /* tri edge 0 */
	SUB(e1,v2,v1);      /* tri edge 1 */
	SUB(e2,v0,v2);      /* tri edge 2 */

	/* Bullet 3:  */
	/*  test the 9 tests first (this was faster) */
	fex = fabs(e0[X]);
	fey = fabs(e0[Y]);
	fez = fabs(e0[Z]);
	AXISTEST_X01(e0[Z], e0[Y], fez, fey);
	AXISTEST_Y02(e0[Z], e0[X], fez, fex);
	AXISTEST_Z12(e0[Y], e0[X], fey, fex);

	fex = fabs(e1[X]);
	fey = fabs(e1[Y]);
	fez = fabs(e1[Z]);
	AXISTEST_X01(e1[Z], e1[Y], fez, fey);
	AXISTEST_Y02(e1[Z], e1[X], fez, fex);
	AXISTEST_Z0(e1[Y], e1[X], fey, fex);

	fex = fabs(e2[X]);
	fey = fabs(e2[Y]);
	fez = fabs(e2[Z]);
	AXISTEST_X2(e2[Z], e2[Y], fez, fey);
	AXISTEST_Y1(e2[Z], e2[X], fez, fex);
	AXISTEST_Z12(e2[Y], e2[X], fey, fex);

	/* Bullet 1: */
	/*  first test overlap in the {x,y,z}-directions */
	/*  find min, max of the triangle each direction, and test for overlap in */
	/*  that direction -- this is equivalent to testing a minimal AABB around */
	/*  the triangle against the AABB */

	/* test in X-direction */
	FINDMINMAX(v0[X],v1[X],v2[X],min,max);
	if(min>boxhalfsize[X] || max<-boxhalfsize[X]) return false;

	/* test in Y-direction */
	FINDMINMAX(v0[Y],v1[Y],v2[Y],min,max);
	if(min>boxhalfsize[Y] || max<-boxhalfsize[Y]) return false;

	/* test in Z-direction */
	FINDMINMAX(v0[Z],v1[Z],v2[Z],min,max);
	if(min>boxhalfsize[Z] || max<-boxhalfsize[Z]) return false;

	/* Bullet 2: */
	/*  test if the box intersects the plane of the triangle */
	/*  compute plane equation of triangle: normal*x+d=0 */
	CROSS(normal,e0,e1);
	d=-DOT(normal,v0);  /* plane eq: normal.x+d=0 */
	if(!planeBoxOverlap(normal,d,boxhalfsize)) return false;

	return true;   /* box and triangle overlaps */
}

//////////////////////////////////////////////////////////////
// 计算三角形是否与包围盒相交
//////////////////////////////////////////////////////////////
bool Triangle::IntersectAABB(const AABB &box)
{
	return TriIntersectAABB(v0, v1, v2, box);
}

//////////////////////////////////////////////////////////////
// 计算三角形是否与某条光线相交
// a_Ray	- 光线
// a_Dist	- 当前的最近距离
//////////////////////////////////////////////////////////////
bool Triangle::Intersect(const Ray& a_Ray, double& a_Dist )
{
	//----------------------------- your code here --------------------------------
	double u, v, t;
	Vector3D e1 = v1 - v0;
	Vector3D e2 = v2 - v0;
	Vector3D p = Cross(a_Ray.d, e2);
	double a = Dot(e1, p);
	if (fabs(a) < EPSILON)
		return false;
	double f = 1.0 / a;
	Vector3D s = a_Ray.o - v0;
	u = f * Dot(s, p);
	if (u < 0.0 || 1.0 < u)
		return false;
	Vector3D q = Cross(s, e1);
	v = f * Dot(a_Ray.d, q);
	if (v < 0.0 || 1.0 < u + v)
		return false;

	t = f * Dot(e2, q);

	if (t < 0.0 || t > a_Dist)
		return false;

	a_Dist = t;
	return true;
	return false;

	//----------------------------- end of your code ------------------------------
}

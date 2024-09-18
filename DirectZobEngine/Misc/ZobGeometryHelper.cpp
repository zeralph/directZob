#include "ZobGeometryHelper.h"
#include "../Rendering/ZobMatrix2x2.h"
#include "../Rendering/ZobMatrix4x4.h"
#include "../Rendering/ZobVector2.h"
#include "../Rendering/Color.h"


bool ZobGeometryHelper::RayTriangleIntersect(const Ray* r, const Triangle* t, float& z, float& u, float& v)
{
	float kEpsilon = 0.000001;
	// compute the plane's normal
	ZobVector3 v0v1 = ZobVector3(t->vb->x - t->va->x, t->vb->y - t->va->y, t->vb->z - t->va->z);
	ZobVector3 v0v2 = ZobVector3(t->vc->x - t->va->x, t->vc->y - t->va->y, t->vc->z - t->va->z);
	// no need to normalize
	ZobVector3 N = ZobVector3::Cross(&v0v1, &v0v2); // N
	float denom = ZobVector3::Dot(&N, &N);
	//float area2 = N.length2();

	// Step 1: finding P

	if (ZobVector3::Dot(&r->n, &N) > 0) // back-facing surface
	{
		return false;
	}

	// check if the ray and plane are parallel.
	float NdotRayDirection = ZobVector3::Dot(&N, &r->n);
	if (fabs(NdotRayDirection) < kEpsilon) // almost 0
	{
		return false; // they are parallel, so they don't intersect! 
	}

	// compute d parameter using equation 2
	ZobVector3 minusN = -N;
	float d = ZobVector3::Dot(&minusN, t->va);

	// compute t (equation 3)
	z = -(ZobVector3::Dot(&N, &r->p) + d) / NdotRayDirection;

	// check if the triangle is behind the ray
	//if (t. < 0)
	{
		return false; // the triangle is behind
	}

	// compute the intersection point using equation 1
	ZobVector3 P = r->p + z * r->n;

	// Step 2: inside-outside test
	ZobVector3 C; // vector perpendicular to triangle's plane

	// edge 0
	ZobVector3 edge0 = ZobVector3(t->vb->x - t->va->x, t->vb->y - t->va->y, t->vb->z - t->va->z);
	ZobVector3 vp0 = P - t->va;
	C = ZobVector3::Cross(&edge0, &vp0);
	if (ZobVector3::Dot(&N, &C) < 0)
	{
		return false; // P is on the right side
	}

	// edge 1
	ZobVector3 edge1 = ZobVector3(t->vc->x - t->vb->x, t->vc->y - t->vb->y, t->vc->z - t->vb->z);
	ZobVector3 vp1 = P - t->vb;
	C = ZobVector3::Cross(&edge1, &vp1);
	u = ZobVector3::Dot(&N, &C);
	if (u < 0)
	{
		return false; // P is on the right side
	}

	// edge 2
	ZobVector3 edge2 = ZobVector3(t->va->x - t->vc->x, t->va->y - t->vc->y, t->va->z - t->vc->z);
	ZobVector3 vp2 = P - t->vc;
	C = ZobVector3::Cross(&edge2, &vp2);
	v = ZobVector3::Dot(&N, &C);
	if (v < 0)
	{
		return false; // P is on the right side;
	}

	u /= denom;
	v /= denom;
	return true; // this ray hits the triangle
}


bool ZobGeometryHelper::WorldLineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt)
{
	//int
	//	intersect3D_RayTriangle(Ray R, Triangle T, Point * I)
	//{
	ZobVector3    u, v, n;              // triangle vectors
	ZobVector3    dir, w0, w;           // ray vectors
	float     r, a, b;              // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u.x = t->vb->x - t->va->x;
	u.y = t->vb->y - t->va->y;
	u.z = t->vb->z - t->va->z;
	v.x = t->vc->x - t->va->x;
	v.y = t->vc->y - t->va->y;
	v.z = t->vc->z - t->va->z;

	n = u * v;              // cross product
	//if (n == (ZobVector3)0)             // triangle is degenerate
	//	return -1;                  // do not deal with this case

	dir = lv;              // ray direction vector
	w0.x = l0->x - t->va->x;
	w0.x = l0->y - t->va->y;
	w0.x = l0->z - t->va->z;
	a = -ZobVector3::Dot(&n, &w0);
	b = ZobVector3::Dot(&n, &dir);
	if (fabs(b) < 0.00000001) {     // ray is  parallel to triangle plane
		//if (a == 0)                 // ray lies in triangle plane
		//	return true;
		//else return false;              // ray disjoint from plane
		return false;
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return false;                   // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	intersectionPt = l0 + dir * r;            // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = ZobVector3::Dot(&u, &u);
	uv = ZobVector3::Dot(&u, &v);
	vv = ZobVector3::Dot(&v, &v);
	w = intersectionPt - t->va;
	wu = ZobVector3::Dot(&w, &u);
	wv = ZobVector3::Dot(&w, &v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float ss, tt;
	ss = (uv * wv - vv * wu) / D;
	if (ss < 0.0 || ss > 1.0)         // I is outside T
		return 0;
	tt = (uv * wu - uu * wv) / D;
	if (tt < 0.0 || (ss + tt) > 1.0)  // I is outside T
		return false;

	return true;                       // I is in T
}
bool ZobGeometryHelper::ProjectionLineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt)
{
	//int
	//	intersect3D_RayTriangle(Ray R, Triangle T, Point * I)
	//{
	ZobVector3    u, v, n;              // triangle vectors
	ZobVector3    dir, w0, w;           // ray vectors
	float     r, a, b;              // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u.x = t->pb->x - t->pa->x;
	u.y = t->pb->y - t->pa->y;
	u.z = t->pb->z - t->pa->z;
	v.x = t->pc->x - t->pa->x;
	v.y = t->pc->y - t->pa->y;
	v.z = t->pc->z - t->pa->z;

	n = u * v;              // cross product
	//if (n == (ZobVector3)0)             // triangle is degenerate
	//	return -1;                  // do not deal with this case

	dir = lv;              // ray direction vector
	w0.x = l0->x - t->pa->x;
	w0.x = l0->y - t->pa->y;
	w0.x = l0->z - t->pa->z;
	a = -ZobVector3::Dot(&n, &w0);
	b = ZobVector3::Dot(&n, &dir);
	if (fabs(b) < 0.00000001) {     // ray is  parallel to triangle plane
		if (a == 0)                 // ray lies in triangle plane
			return true;
		else return false;              // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b;
	if (r < 0.0)                    // ray goes away from triangle
		return false;                   // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	intersectionPt = l0 + dir * r;            // intersect point of ray and plane

	// is I inside T?
	float    uu, uv, vv, wu, wv, D;
	uu = ZobVector3::Dot(&u, &u);
	uv = ZobVector3::Dot(&u, &v);
	vv = ZobVector3::Dot(&v, &v);
	w = intersectionPt - t->va;
	wu = ZobVector3::Dot(&w, &u);
	wv = ZobVector3::Dot(&w, &v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float ss, tt;
	ss = (uv * wv - vv * wu) / D;
	if (ss < 0.0 || ss > 1.0)         // I is outside T
		return false;
	tt = (uv * wu - uu * wv) / D;
	if (tt < 0.0 || (ss + tt) > 1.0)  // I is outside T
		return false;

	return true;                       // I is in T
}
/*
ZobVector3 ZobGeometryHelper::LinePlaneIntersection(const ZobVector3* planeOrigin, const ZobVector3* planeNormal, const ZobVector3* lineOrigin, const ZobVector3* lineDirection)
{
	ZobVector3 ret;
	float d = -(planeNormal->x * planeOrigin->x + planeNormal->y * planeOrigin->y + planeNormal->z * planeOrigin->z);
	float t = -(planeNormal->x * lineOrigin->x + planeNormal->y * lineOrigin->y + planeNormal->z * lineOrigin->z + d) / (planeNormal->x * lineDirection->x + planeNormal->y * lineDirection->y + planeNormal->z * lineDirection->z);
	if (std::isfinite(t) && std::isfinite(d))
	{
		ret.x = lineOrigin->x + t * lineDirection->x;
		ret.y = lineOrigin->y + t * lineDirection->y;
		ret.z = lineOrigin->z + t * lineDirection->z;
	}
	return ret;
}
*/
ZobVector3 ZobGeometryHelper::LinePlaneIntersection(const ZobVector3* planeOrigin, const ZobVector3* planeNormal, const ZobVector3* lineOrigin, const ZobVector3* lineDirection) 
{
	ZobVector3 ret = ZobVector3(0, 0, 0);
	float a = ZobVector3::Dot(planeNormal, lineOrigin);
	if (a == 0) {
		return ret;
	}
	float b = ZobVector3::Dot(planeNormal, planeOrigin);
	float c = ZobVector3::Dot(planeNormal, lineOrigin);
	float d = ZobVector3::Dot(planeNormal, lineDirection);
	double t = (b - c) / d;
	ret.x = lineDirection->x * t + lineOrigin->x;
	ret.y = lineDirection->y * t + lineOrigin->y;
	ret.z = lineDirection->z * t + lineOrigin->z;
	return ret;
}

bool ZobGeometryHelper::LineLineIntersection(const ZobVector3* a0, const ZobVector3* a1, const ZobVector3* b0, const ZobVector3* b1, ZobVector3& out, float& outFactor)
{
	ZobVector3 da = ZobVector3(a1->x - a0->x, a1->y - a0->y, a1->z - a0->z);
	ZobVector3 db = ZobVector3(b1->x - b0->x, b1->y - b0->y, b1->z - b0->z);
	ZobVector3 dc = ZobVector3(b0->x - a0->x, b0->y - a0->y, b0->z - a0->z);
	ZobVector3 cross = ZobVector3::Cross(&da, &db);
	if (ZobVector3::Dot(&dc, &cross) != 0.0)
	{
		//return false;
	}
	ZobVector3 crossA = ZobVector3::Cross(&dc, &db);
	ZobVector3 crossB = ZobVector3::Cross(&da, &db);
	float s = ZobVector3::Dot(&crossA, &crossB) / crossB.length2();
	if (s >= 0.0 && s <= 1.0)
	{
		out = da;
		out.Mul(s);
		out.Add(a0);
		outFactor = s;
		return true;
	}
	return false;
}

void ZobGeometryHelper::ClipSegmentToPlane(ZobVector3& s0, ZobVector3& s1, ZobVector3& pp, ZobVector3& pn)
{
	ZobVector3 u = s1 - s0;
	ZobVector3 w = s0 - pp;
	float D = ZobVector3::Dot(&pn, &u);
	float N = -ZobVector3::Dot(&pn, &w);
	if (fabs(D) < 0.001f)
	{
		return;
	}
	float sI = N / D;
	if (sI < 0 || sI > 1)
	{
		return;
	}
	u.Mul(sI);
	s1 = s0 + u;
}

ZobVector3 ZobGeometryHelper::ProjectPointOnLine(const ZobVector3* linePoint, const ZobVector3* lineVector, const ZobVector3* point)
{
	ZobVector3 ret = ZobVector3(point);
	ZobVector3 p = ret - linePoint;
	float d = ZobVector3::Dot(&p, lineVector);
	ret = ZobVector3(lineVector);
	ret = ret * d;
	ret.x = linePoint->x + (ret.x);
	ret.y = linePoint->y + (ret.y);
	ret.z = linePoint->z + (ret.z);
	return ret;
}
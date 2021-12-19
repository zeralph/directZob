#include "ZobGeometryHelper.h"
#include "../Rendering/ZobMatrix2x2.h"
#include "../Rendering/ZobMatrix4x4.h"
#include "../Rendering/ZobVector2.h"
#include "../Rendering/Color.h"



bool ZobGeometryHelper::LineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt)
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
		return 0;
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
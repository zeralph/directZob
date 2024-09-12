#pragma once
#include "../Types.h"
#include <math.h>
#include <stdlib.h> 
#include <string>
#include <ctime>
#include <stdarg.h>
#include "../Rendering/ZobVector3.h"
#include "../Rendering/Triangle.h"


class ZobGeometryHelper
{
public:

	static bool			RayTriangleIntersect(const DirectZobType::Ray* r, const Triangle* t, float& z, float& u, float& v);
	static ZobVector3	LinePlaneIntersection(const ZobVector3* planeOrigin, const ZobVector3* planeNormal, const ZobVector3* lineOrigin, const ZobVector3* lineDirection);
	static bool			ProjectionLineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt);
	static bool			WorldLineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt);
	static bool			LineLineIntersection(const ZobVector3* s0, const ZobVector3* s1, const ZobVector3* e0, const ZobVector3* e1, ZobVector3& out, float& outFactor);
	static void			ClipSegmentToPlane(ZobVector3& s0, ZobVector3& s1, ZobVector3& pp, ZobVector3& pn);
	static ZobVector3	ProjectPointOnLine(const ZobVector3* linePoint, const ZobVector3* lineVector, const ZobVector3* point);
};
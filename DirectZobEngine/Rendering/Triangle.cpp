#include "Triangle.h"



Triangle::Triangle()
{
	material = NULL;
	va = NULL;
	vb = NULL;
	vc = NULL;

	pa = NULL;
	pb = NULL;
	pc = NULL;

	na = NULL;
	nb = NULL;
	nc = NULL;

	ua = NULL;
	ub = NULL;
	uc = NULL;

	ca = NULL;
	cb = NULL;
	cc = NULL;

	draw = false;
	options = NULL;
}


Triangle::~Triangle()
{
	material = NULL;
	draw = false;
	va = NULL;
	vb = NULL;
	vc = NULL;
	pa = NULL;
	pb = NULL;
	pc = NULL;
	na = NULL;
	nb = NULL;
	nc = NULL;
	ua = NULL;
	ub = NULL;
	uc = NULL;
	options = NULL;
}

bool Triangle::PointInTriangle2D(const ZobVector3* v) const
{
	bool b1, b2, b3;

	b1 = sign(v, pa, pb) < 0.0f;
	b2 = sign(v, pb, pc) < 0.0f;
	b3 = sign(v, pc, pa) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}
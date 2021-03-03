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

	ar = 1.0f;
	ab = 1.0f;
	ag = 1.0f;

	br = 1.0f;
	bb = 1.0f;
	bg = 1.0f;

	cr = 1.0f;
	cb = 1.0f;
	cg = 1.0f;

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

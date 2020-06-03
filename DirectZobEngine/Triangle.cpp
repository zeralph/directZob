#include "Triangle.h"

using namespace directZob;

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

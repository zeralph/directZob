#include "Triangle.h"



Triangle::Triangle()
{
	tex = NULL;
	va = new Vector3();
	vb = new Vector3();
	vc = new Vector3();

	pa = new Vector3();
	pb = new Vector3();
	pc = new Vector3();

	na = new Vector3();
	nb = new Vector3();
	nc = new Vector3();

	ua = new Vector2();
	ub = new Vector2();
	uc = new Vector2();

	draw = false;
}


Triangle::~Triangle()
{
}

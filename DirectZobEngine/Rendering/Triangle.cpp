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

void Triangle::CopyTriangle(Triangle* dst, const Triangle* src)
{
	//memcpy(dst, src, sizeof(Triangle));
	
	dst->va->Copy(src->va);
	dst->vb->Copy(src->vb);
	dst->vc->Copy(src->vc);
	dst->pa->Copy(src->pa);
	dst->pb->Copy(src->pb);
	dst->pc->Copy(src->pc);
	dst->na->Copy(src->na);
	dst->nb->Copy(src->nb);
	dst->nc->Copy(src->nc);
	dst->n->Copy(src->n);
	dst->ua->Copy(src->ua);
	dst->ub->Copy(src->ub);
	dst->uc->Copy(src->uc);
	
	dst->material = src->material;
	dst->options = src->options;
	dst->draw = src->draw;
	dst->clipMode = src->clipMode;
	dst->ca = src->ca;
	dst->cb = src->cb;
	dst->cc = src->cc;
	dst->zobObject = src->zobObject;
	dst->area = src->area;
	
}
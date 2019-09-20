#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"
#include "Types.h"

//FLAGS :
//					7			6			5			4			3			2			1			0
//		|no lighting|no zBuffer |	...
//
class Triangle
{
public:
	Triangle();
	~Triangle();

	Vector3* va;
	Vector3* vb;
	Vector3* vc;

	Vector3* na;
	Vector3* nb;
	Vector3* nc;

	Vector2* ua;
	Vector2* ub;
	Vector2* uc;

	const Material* tex;
	float area;
	float la;
	float lb;
	float lc;
	uint owner;
	bool draw;
	DirectZobType::RenderOptions options;

	inline void ComputeArea()
	{
		area = (vc->x - va->x) * (vb->y - va->y) - (vc->y - va->y) * (vb->x - va->x);
	}

	inline void ComputeLighting(Vector3* v)
	{
		la = fmax(0.0f, Vector3::Dot(na, v));
		lb = fmax(0.0f, Vector3::Dot(na, v));
		lc = fmax(0.0f, Vector3::Dot(na, v));
		//la = 0.5f;
		//lb = 0.5f;
		//lc = 0.5f;
	}

	inline void Copy(const Triangle* t)
	{
		va->Copy(t->va);
		vb->Copy(t->vb);
		vc->Copy(t->vc);  
		na->Copy(t->na);
		nb->Copy(t->nb);
		nc->Copy(t->nc);
		ua->Copy(t->ua);
		ub->Copy(t->ub);
		uc->Copy(t->uc);
		tex = t->tex;
		area = t->area;
		owner = t->owner;
		lb = t->lb;
		la = t->la;
		lc = t->lc;
		options = t->options;
		draw = t->draw;
	}
};


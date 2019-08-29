#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"

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

	const Texture* tex;

	float area;

	float la;
	float lb;
	float lc;

	bool draw;

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
};


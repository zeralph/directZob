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

	Vector3* pa;
	Vector3* pb;
	Vector3* pc;

	Vector3* na;
	Vector3* nb;
	Vector3* nc;

	Vector3* n;

	Vector2* ua;
	Vector2* ub;
	Vector2* uc;
	const Material* material;
	float area;
	uint owner;
	bool draw;
	const DirectZobType::RenderOptions* options;

	inline void ComputeArea()
	{
		area = (pc->x - pa->x) * (pb->y - pa->y) - (pc->y - pa->y) * (pb->x - pa->x);
		//area = (vc->x - va->x) * (vb->y - va->y) - (vc->y - va->y) * (vb->x - va->x);
	}

};


#pragma once
#include "ZobVector2.h"
#include "ZobVector3.h"
#include "ZobMaterial.h"
#include "Types.h"

class ZobObject;

//FLAGS :
//					7			6			5			4			3			2			1			0
//		|no lighting|no zBuffer |	...
//
class Triangle
{
public:
	Triangle();
	~Triangle();

	ZobVector3* va;
	ZobVector3* vb;
	ZobVector3* vc;

	ZobVector3* pa;
	ZobVector3* pb;
	ZobVector3* pc;

	ZobVector3* na;
	ZobVector3* nb;
	ZobVector3* nc;

	ZobVector3* n;

	ZobVector2* ua;
	ZobVector2* ub;
	ZobVector2* uc;
	const ZobMaterial* material;
	ZobObject* zobObject;
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


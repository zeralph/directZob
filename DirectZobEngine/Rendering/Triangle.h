#pragma once
#include "ZobVector2.h"
#include "ZobVector3.h"
#include "../ZobMaterial.h"
#include "../Types.h"

class ZobObject;

//FLAGS :
//					7			6			5			4			3			2			1			0
//		|no lighting|no zBuffer |	...
//
class Triangle
{
public:

	enum eClip
	{
		eClip_0_in=0,
		eClip_1_in,
		eClip_A_in_BC_out,
		eClip_B_in_AC_out,
		eClip_C_in_AB_out,
		eClip_2_in,
		eClip_AB_in_C_out,
		eClip_AC_in_B_out,
		eClip_BC_in_A_out,
		eClip_3_in,
	};

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

	int verticeAIndex;
	int verticeBIndex;
	int verticeCIndex;

	eClip clipMode;

	int ca;
	int cb;
	int cc;

	const ZobMaterial* material;
	ZobObject* zobObject;
	float area;
	bool draw;
	const DirectZobType::RenderOptions* options;

	inline void ComputeArea()
	{
		area = (pc->x - pa->x) * (pb->y - pa->y) - (pc->y - pa->y) * (pb->x - pa->x);
		//area = (vc->x - va->x) * (vb->y - va->y) - (vc->y - va->y) * (vb->x - va->x);
	}

};


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

	ZobVector3* ca;
	ZobVector3* cb;
	ZobVector3* cc;

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

	const ZobMaterial* material;
	ZobObject* zobObject;
	float area;
	bool draw;
	const DirectZobType::RenderOptions* options;

	inline void ComputeArea()
	{
		area = (pc->x - pa->x) * (pb->y - pa->y) - (pc->y - pa->y) * (pb->x - pa->x);
	}

	inline float sign(const ZobVector3* p1, const ZobVector3* p2, const ZobVector3* p3) const
	{
		return (p1->x - p3->x) * (p2->y - p3->y) - (p2->x - p3->x) * (p1->y - p3->y);
	}

	bool PointInTriangle2D(const ZobVector3* v) const;

	static inline void CopyTriangle(Triangle* dst, const Triangle* src)
	{
		//memcpy(dst, src, sizeof(Triangle));
		//return;
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

		dst->ca->Copy(src->ca);
		dst->cb->Copy(src->cb);
		dst->cc->Copy(src->cc);

		dst->material = src->material;
		dst->options = src->options;
		dst->draw = src->draw;
		dst->clipMode = src->clipMode;
		dst->zobObject = src->zobObject;
		dst->area = src->area;

	}

};


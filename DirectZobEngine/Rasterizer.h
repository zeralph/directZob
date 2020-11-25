#pragma once

#include "Types.h"
#include "ZobVector2.h"
#include "ZobVector3.h"
#include "ZobMaterial.h"
#include "ZobMatrix4x4.h"
#include "Triangle.h"
#include <thread> 
#include <algorithm>
#ifdef LINUX
	#include <unistd.h>
#elif MACOS
    #include <unistd.h>
#elif WINDOWS
	#include <windows.h>
#endif //LINUX

class Light;
class Rasterizer
{
public:

	Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData);
	~Rasterizer();

	void 					DrawTriangle(const Triangle* t) const;
	void 					plotLine(int x0, int y0, int x1, int y1) const;
	void 					DrawLine(const Line3D* l) const;
	void 					DrawLine2(const Line3D* l) const;
	void 					Start(const Triangle* triangles, const uint nbTriangles, const std::vector<Line3D>* lines, const bool wireFrame, const eRenderMode renderMode, const bool bEvenFrame, const eLightingPrecision lp);
	float 					WaitForEnd();
	void 					End();
	void 					Init();
	void 					Render();
	void 					Clear();
	inline const Triangle*	GetTriangle(int i) { return &m_triangles[i]; }
	inline int				GetNbTriangle() { return m_nbTriangles; }
private:

	void 					FillTopFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const ZobVector3* la, const ZobVector3* lb, const ZobVector3* lc) const;
	void 					FillBottomFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const ZobVector3* la, const ZobVector3* lb, const ZobVector3* lc) const;
	inline const void 		FillBufferPixel(const ZobVector3* p, const Triangle* t, const ZobVector3* la, const ZobVector3* lb, const ZobVector3* lc) const;
	inline ZobVector3 		ComputeLightingAtPoint(const ZobVector3* position, const ZobVector3* normal, RenderOptions::eLightMode lighting) const;
	void 					sortVerticesAscendingByY(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3) const ;
	void 					sortVerticesAscendingByY(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, ZobVector2* uv1, ZobVector2* uv2, ZobVector2* uv3) const;
	inline float 			edgeFunction(const ZobVector3* a, const ZobVector3* b, const ZobVector3* c) const { return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }
	inline float 			clamp2(float x, const float min, const float max) const { if (x < min) return min; if (x > max) return max; return x; }
	inline const bool 		RenderLine(int line) const 
	{ 
		return (m_renderMode == eRenderMode_fullframe) ||
			(m_renderMode == eRenderMode_interlaced && line % 2 == m_bEvenFrame) ||
			(m_renderMode == eRenderMode_scanline && line % 2 == 0);
	}
	inline const float 	computeAmbient(float ambientIntensity) const
	{
		return clamp2(ambientIntensity, 0.0f, 1.0f);
	};

	inline const float 	computeLighting(const ZobVector3* normal, const ZobVector3* light) const
	{
		//float f = 1.0f - clamp2(-fabsf(ZobVector3::Dot(normal, light)), 0.0f, 1.0f);
		float f = 1.0f - clamp2(ZobVector3::Dot(normal, light), 0.0f, 1.0f);
		return f;// clamp2(fabsf(ZobVector3::Dot(normal, light)), 0.0f, 1.0f);
	};

	inline const float 	computeSpecular(const ZobVector3* normal, const ZobVector3* light, const ZobVector3* c, const float lightIntensity, const float specularIntensity) const
	{
		ZobVector3 r = ZobVector3(normal->x - light->x, normal->y - light->y, normal->z - light->z);
		r.Mul(2.0f * lightIntensity);
		float sl = ZobVector3::Dot(&r, &m_camDir);
		sl = pow(lightIntensity, specularIntensity) * sl;
		sl = clamp2(sl, 0.0f, 1.0f);
		return sl;
	};

	const std::vector<Light*>* m_lights;
	const std::vector<Line3D>* m_lines;
	const Triangle* m_triangles;
	const ZobVector3* m_ambientColor;
	float m_ambientIntensity;
	const ZobVector3* m_fogColor;
	float m_fogDistance;
	float m_fogDensity;
	eFogType m_fogType;
	BufferData* m_bufferData;
	ZobVector3 m_camDir;
	uint m_nbTriangles;
	//std::thread m_thread;
	uint m_startHeight;
	uint m_width;
	uint m_endHeight;
	std::thread m_thread;
	bool m_wireFrame = false;
	eRenderMode m_renderMode;
	int m_bEvenFrame = 0;
	eLightingPrecision m_lightingPrecision;
	clock_t	m_tick;
	float m_time;
};

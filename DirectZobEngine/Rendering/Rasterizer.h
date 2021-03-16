#pragma once

#include "Types.h"
#include "ZobVector2.h"
#include "ZobVector3.h"
#include "ZobMaterial.h"
#include "ZobMatrix4x4.h"
#include "Triangle.h"
#include <thread> 
#include <mutex>
#include <algorithm>
#include <condition_variable>
#ifdef LINUX
	#include <unistd.h>
#elif MACOS
    #include <unistd.h>
#elif WINDOWS
	#include <windows.h>
#endif //LINUX
#include <mutex>

class Light;
class Rasterizer
{
public:

	struct LightingData
	{
		float LightingDiffuseR;
		float LightingDiffuseG;
		float LightingDiffuseB;
		float LightingSpecularR;
		float LightingSpecularG;
		float LightingSpecularB;
		inline void Init()
		{
			LightingDiffuseR = 0.0f;
			LightingDiffuseG = 0.0f;
			LightingDiffuseB = 0.0f;
			LightingSpecularR = 0.0f;
			LightingSpecularG = 0.0f;
			LightingSpecularB = 0.0f;
		}
	};

	Rasterizer(uint width, uint height, uint startHeight, uint endHeight, uint MaxQueueSize, uint rasterizerNumber, BufferData* bufferData);
	~Rasterizer();

	void 					DrawTriangle(const Triangle* t) const;
	void 					plotLine(int x0, int y0, int x1, int y1) const;
	void 					DrawLine(const Line3D* l) const;
	void 					Start();
	float 					WaitForEnd();
	void 					End();
	void 					Init(std::condition_variable* cv, std::mutex* m);
	void 					Render();
	void 					Clear();
	void					QueueTriangle(const Triangle* t);
	void					QueueLine(const Line3D* l);
	inline const Triangle*	GetTriangle(int i) { return m_triangles[i]; }
	inline int				GetNbTriangle() const { return m_nbTriangles; }// m_triangles.size(); }
	inline int				GetNbLine() const { return m_lines.size(); }
	inline float			GetRenderTimeMS() const { return m_time;  }

private:

	void					RenderInternal();
	void 					FillTopFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const;
	void 					FillBottomFlatTriangle2(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const;
	inline const void 		FillBufferPixel(const ZobVector3* p, const Triangle* t, const LightingData* lDataA, const LightingData* lDataB, const LightingData* lDataC) const;
	inline void		 		ComputeLightingAtPoint(const ZobVector3* position, const ZobVector3* normal, const Triangle* t, LightingData& outlData) const;
	void 					sortVerticesAscendingByY(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3) const ;
	void 					sortVerticesAscendingByY(ZobVector2* v1, ZobVector2* v2, ZobVector2* v3, ZobVector2* uv1, ZobVector2* uv2, ZobVector2* uv3) const;
	inline float 			edgeFunction(const ZobVector3* a, const ZobVector3* b, const ZobVector3* c) const { return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }
	inline float 			clamp2(float x, const float min, const float max) const { if (x < min) return min; if (x > max) return max; return x; }
	bool 					RenderLine(int line) const;
	inline const float 		computeAmbient(float ambientIntensity) const
	{
		return clamp2(ambientIntensity, 0.0f, 1.0f);
	};

	const float ComputeFog(float z) const;

	inline const float computeSpecular(const ZobVector3* normal, const ZobVector3* lightPos, const ZobVector3* position, const Triangle* t) const
	{
		float sl = 0.0f;
		float exp = 1.0f;
		if (t->material)
		{
			exp = t->material->GetSpecularExponent();
		}
		if (exp > 0.0f)
		{
			ZobVector3 o = m_camPos - position;
			ZobVector3 l = lightPos;
			l.Sub(position);
			o.Normalize();
			l.Normalize();
			ZobVector3 h = o + l;
			h.Normalize();
			float cos_theta_h = ZobVector3::Dot(normal, &h);
			float cos_theta = fmaxf(0, ZobVector3::Dot(normal, &l));
			static float tt = 100.0f;
			sl = (exp + 8.0f) / (8.0f * M_PI) * pow(cos_theta_h, tt) * cos_theta;
			if (!isfinite(sl))
			{
				int y = 0;
				y++;
			}
		}
		return sl;
	};

	std::vector<const Light*> m_lights;
	std::vector<const Line3D*> m_lines;
	const Triangle** m_triangles;
	uint m_maxTriangleQueueSize;
	uint m_nbTriangles;
	uint m_rasterizerNumber;
	const ZobVector3* m_ambientColor;
	float m_ambientIntensity;
	const ZobVector3* m_fogColor;
	float m_fogDistance;
	float m_fogDensity;
	eFogType m_fogType;
	BufferData* m_bufferData;
	ZobVector3 m_camPos;
	//std::thread m_thread;
	uint m_startHeight;
	uint m_width;
	uint m_height;
	uint m_endHeight;
	std::thread m_thread;
	bool m_wireFrame = false;
	eRenderMode m_renderMode;
	int m_bEvenFrame = 0;
	eLightingPrecision m_lightingPrecision;
	clock_t	m_tick;
	float m_time;
	int m_num;
	std::mutex* m_drawMutex;
	std::condition_variable* m_startConditionVariable;
	bool m_runThread;
	volatile bool m_bStartDraw;
};

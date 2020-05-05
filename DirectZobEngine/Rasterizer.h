#pragma once

#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Material.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include <thread> 

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

	void DrawTriangle(const Triangle* t) const;
	void plotLine(int x0, int y0, int x1, int y1) const;
	void DrawLine(const Line3D* l) const;
	Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData);
	~Rasterizer();

	void Start(const Triangle* triangles, const uint nbTriangles, const std::vector<Line3D>* lines, const bool wireFrame, const bool scanline, const bool bEvenFrame, RenderOptions::Lighting_precision lp);
	void WaitForEnd();
	void End();
	void Init();
	void Render();
	void Clear();

private:

	void FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Vector3* la, const Vector3* lb, const Vector3* lc) const;
	void FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, const Vector3* la, const Vector3* lb, const Vector3* lc) const;
	inline const void FillBufferPixel(const Vector3* p, const Triangle* t, const Vector3* la, const Vector3* lb, const Vector3* lc) const;
	inline Vector3 ComputeLightingAtPoint(const Vector3* position, const Vector3* normal, RenderOptions::eLightMode lighting) const;
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3) const ;
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3) const;
	inline float edgeFunction(const Vector3* a, const Vector3* b, const Vector3* c) const { return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }
	inline float clamp2(float x, const float min, const float max) const { if (x < min) x = min; if (x > max) x = max; return x; }
	inline const bool RenderLine(int line) const { return !m_scanline || line % 2 == m_bEvenFrame; }
	inline const float computeAmbient(float ambientIntensity) const
	{
		return clamp2(ambientIntensity, 0.0f, 1.0f);
	};

	inline const float computeLighting(const Vector3* normal, const Vector3* light) const
	{
		return clamp2(-Vector3::Dot(normal, light), 0.0f, 1.0f);
	};

	inline const float computeSpecular(const Vector3* normal, const Vector3* light, const Vector3* c, const float lightIntensity, const float specularIntensity) const
	{
		Vector3 r = Vector3(normal->x - light->x, normal->y - light->y, normal->z - light->z);
		r.Mul(2.0f * lightIntensity);
		float sl = Vector3::Dot(&r, &m_camDir);
		sl = pow(lightIntensity, specularIntensity) * sl;
		sl = clamp2(sl, 0.0f, 1.0f);
		return sl;
	};

	const std::vector<Light*>* m_lights;
	const std::vector<Line3D>* m_lines;
	const Triangle* m_triangles;
	const Vector3* m_ambientColor;
	const Vector3* m_fogColor;
	float m_fogDistance;
	float m_fogDensity;
	FogType m_fogType;
	BufferData* m_bufferData;
	Vector3 m_camDir;
	uint m_nbTriangles;
	//std::thread m_thread;
	uint m_startHeight;
	uint m_width;
	uint m_endHeight;
	std::thread m_thread;
	bool m_wireFrame = false;
	bool m_scanline = false;
	int m_bEvenFrame = 0;
	RenderOptions::Lighting_precision m_lightingPrecision;
};

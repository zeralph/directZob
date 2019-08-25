#pragma once
#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include <thread> 
#include <shared_mutex>
		

class Rasterizer
{
public:

	template <typename t>
	t clamp2(t x, t min, t max) const
	{
		if (x < min) x = min;
		if (x > max) x = max;
		return x;
	}

	void DrawTriangle(const Triangle* t, BufferData* bufferData) const;
	void DrawLine(const Line2D* l, BufferData* bufferData) const;
	Rasterizer(uint width, uint startHeight, uint endHeight, float zNear, float zFar);
	~Rasterizer();

	void Start(const std::vector<const Triangle*>* triangles, const std::vector<Line2D>* lines, BufferData* bufferData);
	void End() { m_run = false; }
	void Init();
	void Run();
	void Render() const;
	void Clear();
	void FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, BufferData* bufferData) const;
	void FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, BufferData* bufferData) const;
	void FillBufferPixel(const Vector3* p, const Triangle* t, BufferData* bufferData) const;
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3) const ;
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3) const;
	inline float edgeFunction(const Vector3* a, const Vector3* b, const Vector3* c) const { return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }

	const std::vector<Line2D>* m_lines;
	const std::vector<const Triangle*>* m_triangles;
	BufferData* m_bufferData;
	std::thread m_thread;
	uint m_startHeight;
	uint m_width;
	uint m_height;
	float m_zNear;
	float m_zFar;
	volatile bool m_run;
	volatile bool m_started;
};



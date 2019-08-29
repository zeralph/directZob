#pragma once
#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4x4.h"
#include "Triangle.h"
#include <thread> 
#include <atomic>

#ifdef LINUX
	#include <unistd.h>
#else
	#include <windows.h>
#endif //LINUX

#include <windows.h>
		

class Rasterizer
{
public:

	void DrawTriangle(const Triangle* t) const;
	void DrawLine(const Line2D* l) const;
	Rasterizer(uint width, uint startHeight, uint endHeight, BufferData* bufferData);
	~Rasterizer();

	void Start(const std::vector<const Triangle*>* triangles, const std::vector<Line2D>* lines);
	void End() { m_run = false; }
	void Init();
	void Run();
	void Render() const;
	void Clear();
	void FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t) const;
	void FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t) const;
	void FillBufferPixel(const Vector3* p, const Triangle* t) const;
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3) const ;
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3) const;
	inline float edgeFunction(const Vector3* a, const Vector3* b, const Vector3* c) const { return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }
	inline float clamp2(float x, float min, float max) const { if (x < min) x = min; if (x > max) x = max; return x; }

	const std::vector<Line2D>* m_lines;
	const std::vector<const Triangle*>* m_triangles;
	BufferData* m_bufferData;
	std::thread m_thread;
	uint m_startHeight;
	uint m_width;
	uint m_height;
	std::atomic_bool m_run;
	std::atomic_bool m_started;
};



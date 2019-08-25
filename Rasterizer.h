#pragma once
#include "Types.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Texture.h"
#include "Matrix4x4.h"
#include "Engine.h"

class Rasterizer
{
public:
	void DrawTriangle(const Triangle* t, BufferData* bufferData);
	Rasterizer(uint width, uint startHeight, uint endHeigth);
	~Rasterizer();

private:
	void FillTopFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, BufferData* bufferData);
	void FillBottomFlatTriangle2(Vector2* v1, Vector2* v2, Vector2* v3, const Triangle* t, BufferData* bufferData);
	void FillBufferPixel(const Vector3* p, const Triangle* t, BufferData* bufferData);
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3);
	void sortVerticesAscendingByY(Vector2* v1, Vector2* v2, Vector2* v3, Vector2* uv1, Vector2* uv2, Vector2* uv3);
	inline float edgeFunction(const Vector3* a, const Vector3* b, const Vector3* c) { return (c->x - a->x) * (b->y - a->y) - (c->y - a->y) * (b->x - a->x); }

	uint m_width;
	uint m_height;
	float m_zNear;
	float m_zFar;
};


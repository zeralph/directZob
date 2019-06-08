#pragma once
#include "Types.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Engine.h"
#include "Color.h"

class Cube
{
public:
	Cube();
	~Cube();
	void Init();
	inline void SetSCale(float x, float y, float z) { m_modelMatrix.SetScale(x, y, z); }
	inline void SetRotation(float x, float y, float z) { m_modelMatrix.SetRotation(x, y, z); }

	void Draw(Engine* engine);

private:


	inline void ReinitVertices() { memcpy(&m_vertices, &m_verticesData, sizeof(Vector3) * NB_VERTICES); };

	static const uint NB_VERTICES = 8;
	static const uint NB_VERTICES_INDEX = 36;

	Matrix4x4	m_modelMatrix;
	Vector3		m_vertices[NB_VERTICES];

	static const Vector3 m_verticesData[NB_VERTICES];
	static const uint m_verticesIndex[NB_VERTICES_INDEX];
};
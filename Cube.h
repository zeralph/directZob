#pragma once
#include "Types.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Engine.h"
#include "Color.h"

class Cube
{
public:
	Cube(const Texture* tex);
	~Cube();
	void Init();
	inline void SetSCale(float x, float y, float z) { m_modelMatrix.SetScale(x, y, z); }
	inline void SetRotation(float x, float y, float z) { m_modelMatrix.SetRotation(x, y, z); }
	inline void SetTranslation(float x, float y, float z) { m_modelMatrix.SetTranslation(x, y, z); }

	void Draw(Engine* engine);

private:


	inline void ReinitVertices();;

	static const uint NB_VERTICES = 8;
	static const uint NB_TRIANGLES = 12;
	static const uint NB_VERTICES_INDEX = 36;

	Matrix4x4	m_modelMatrix;
	Vector3		m_vertices[NB_VERTICES];
	Vector3		m_normals[NB_TRIANGLES];

	static const Vector3 m_verticesData[NB_VERTICES];
	static const Vector3 m_normalsData[NB_TRIANGLES];
	static const uint m_colorsData[NB_TRIANGLES];
	static const uint m_verticesIndex[NB_VERTICES_INDEX];

	const Texture* m_texture;
};
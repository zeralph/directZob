#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "Types.h"
#include "Engine.h"

class Mesh
{
public:


	Mesh(const std::string* path, const Texture* tex);
	~Mesh();
	void Draw(Engine* engine);
	void Init();

	inline void SetSCale(float x, float y, float z) { m_modelMatrix.SetScale(x, y, z); }
	inline void SetRotation(float x, float y, float z) { m_modelMatrix.SetRotation(x, y, z); }
	inline void SetTranslation(float x, float y, float z) { m_modelMatrix.SetTranslation(x, y, z); }

	Vector3* m_vertices;
	Vector3* m_normals;

	Vector3* m_verticesData;
	Vector3* m_normalsData;

	Vector2* m_uvs;
	Engine::Triangle* m_triangles;
private:
	void SplitEntry(std::string* s, std::vector<std::string>* v, char delim);
	inline void ReinitVertices();

	uint m_nbVertices;
	uint m_nbUvs;
	uint m_nbNormals;
	uint m_nbFaces;

	Matrix4x4	m_modelMatrix;

	const Texture* m_texture;
};


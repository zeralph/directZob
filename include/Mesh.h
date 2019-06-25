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

	Mesh(const char* path, const Texture* tex);
	~Mesh();
	void Draw(const Camera* camera, Engine* engine);
	void Init();

	inline void SetSCale(float x, float y, float z) { m_modelMatrix.SetScale(x, y, z); }
	inline void SetRotation(float x, float y, float z) { m_modelMatrix.SetRotation(x, y, z); m_modelMatrixRotationOnly.SetRotation(x, y, z); }
	inline void SetTranslation(float x, float y, float z) { m_modelMatrix.SetTranslation(x, y, z); }

	Vector3* m_vertices = NULL;
	Vector3* m_normals = NULL;

	Vector3* m_verticesData = NULL;
	Vector3* m_normalsData = NULL;

	Vector2* m_uvs = NULL;
	Engine::Triangle* m_triangles = NULL;

private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	void CreateTriangles(const std::vector<std::string>* line, Engine::Triangle* tArray, size_t &tArrayIdx);
	inline void ReinitVertices();

	uint m_nbVertices = 0;
	uint m_nbUvs = 0;
	uint m_nbNormals = 0;
	uint m_nbFaces = 0;

	Matrix4x4	m_modelMatrix;
	Matrix4x4	m_modelMatrixRotationOnly;
	const Texture* m_texture = NULL;
};


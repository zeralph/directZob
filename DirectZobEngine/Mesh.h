#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Vector3.h"
#include "Vector2.h"
#include "Types.h"
#include "Triangle.h"
#include "Camera.h"
#include "Events.h"
class Mesh
{
public:

	Mesh(std::string& name, std::string& path, const Texture* tex);
	~Mesh();
	void Update(const Camera* camera, const BufferData* bData);
	void Init();

	inline void SetSCale(float x, float y, float z) { m_modelMatrix.SetScale(x, y, z); }
	inline void SetRotation(float x, float y, float z) { m_modelMatrix.SetRotation(x, y, z); m_modelMatrixRotationOnly.SetRotation(x, y, z); }
	inline void SetTranslation(float x, float y, float z) { m_modelMatrix.SetTranslation(x, y, z); }

	const std::vector<Triangle>* GetTrianglesList() const { return &m_triangles; }
	const uint GetNbTriangles() const { return m_nbFaces; }

	Vector3* m_vertices = NULL;
	Vector3* m_normals = NULL;

	Vector3* m_verticesData = NULL;
	Vector3* m_normalsData = NULL;

	Vector2* m_uvs = NULL;
	std::vector<Triangle> m_triangles;

private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	void CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* t, size_t& tArrayIdx, const Texture* tex);
	inline void ReinitVertices();

	uint m_nbVertices = 0;
	uint m_nbUvs = 0;
	uint m_nbNormals = 0;
	uint m_nbFaces = 0;
	std::string m_name;
	bool m_hasNormals = false;
	Matrix4x4	m_modelMatrix;
	Matrix4x4	m_modelMatrixRotationOnly;
	const Texture* m_texture = NULL;

};


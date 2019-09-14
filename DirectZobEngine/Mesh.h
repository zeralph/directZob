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
#include "Engine.h"

class Mesh
{
public:

	Mesh(std::string& name, std::string& path, std::string& file);
	~Mesh();


	void Draw(const Matrix4x4& modelMatrix, const Camera* camera, Core::Engine* engine);

	const std::vector<Triangle>* GetTrianglesList() const { return &m_triangles; }
	const uint GetNbTriangles() const { return m_nbFaces; }

	Vector3* m_vertices = NULL;
	Vector3* m_normals = NULL;

	Vector3* m_verticesData = NULL;
	Vector3* m_normalsData = NULL;

	Vector2* m_uvs = NULL;
	std::vector<Triangle> m_triangles;

	const std::string& GetName() const { return m_name; }
	const std::string& GetFile() const { return m_file; }

private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	void CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* t, size_t& tArrayIdx, const Texture* tex);
	inline void ReinitVertices();
	inline bool RejectTriangle(const Triangle* t, const float znear, const float zfar, const float width, const float height);
	uint m_nbVertices = 0;
	uint m_nbUvs = 0;
	uint m_nbNormals = 0;
	uint m_nbFaces = 0;
	
	bool m_hasNormals = false;
	std::string m_name;
	std::string m_file;
	Matrix4x4	m_modelMatrixRotationOnly;
};


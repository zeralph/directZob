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
#include <fbxsdk.h>
class Mesh
{
public:

	Mesh(std::string& name, std::string& path, std::string& file);
	~Mesh();


	void Draw(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions options);
	void DrawBoundingBox(const Matrix4x4& modelMatrix, const Matrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions options);
	const std::vector<Triangle>* GetTrianglesList() const { return &m_triangles; }
	const uint GetNbTriangles() const { return m_nbFaces; }

	Vector3* m_vertices = NULL;
	Vector3* m_projectedVertices = NULL;
	Vector3* m_verticesNormals = NULL;
	Vector3* m_trianglesNormals = NULL;
	Vector3* m_verticesData = NULL;
	Vector3* m_verticesNormalsData = NULL;
	Vector3* m_trianglesNormalsData = NULL;
	Vector2* m_uvs = NULL;
	std::vector<Triangle> m_triangles;

	const std::string& GetName() const { return m_name; }
	const std::string& GetFile() const { return m_file; }

private:
	Mesh(std::string& parentName, std::string& path, fbxsdk::FbxMesh* mesh);
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	void CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* t, size_t& tArrayIdx, const Material* tex);
	void LoadOBJ(const std::string& fullPath);
	void LoadFbx(const std::string& fullPath);
	void FbxMultT(FbxNode* node, FbxVector4 &vector);
	inline void ReinitVertices()
	{
		memcpy(m_vertices, m_verticesData, sizeof(Vector3) * m_nbVertices);
		memcpy(m_verticesNormals, m_verticesNormalsData, sizeof(Vector3) * m_nbNormals);
		memcpy(m_trianglesNormals, m_trianglesNormalsData, sizeof(Vector3) * m_nbFaces);
	};
	inline bool RejectTriangle(const Triangle* t, const float znear, const float zfar, const float width, const float height);
	std::vector<Mesh*> m_subMeshes;
	uint m_nbVertices = 0;
	uint m_nbUvs = 0;
	uint m_nbNormals = 0;
	uint m_nbFaces = 0;
	
	bool m_hasNormals = false;
	std::string m_name;
	std::string m_file;
	std::string m_path;
	Vector3 m_minBouding;
	Vector3 m_maxBouding;
};


#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Rendering/ZobVector3.h"
#include "Rendering/ZobVector2.h"
#include "Types.h"
#include "Rendering/Triangle.h"
#include "ZobObjects/Camera.h"
#include "Events.h"
#include "Rendering/Engine.h"
#include <fbxsdk.h>

class ZobObject;

class Mesh
{
public:

	Mesh(std::string& name);
	Mesh(Mesh* m);
	Mesh(std::string& name, std::string& path, std::string& file);
	~Mesh();

	virtual void Update(const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options);
	virtual void QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options);
	void DrawBoundingBox(const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Core::Engine* engine, const uint ownerId, const RenderOptions* options);
	const std::vector<Triangle>* GetTrianglesList() const { return &m_triangles; }
	const uint GetNbTriangles() const { return m_nbFaces; }

	const std::string& GetName() const { return m_name; }
	const std::string& GetPath() const { return m_path; }
	const std::string& GetFileName() const { return m_fileName; }

protected:
	Mesh(std::string& parentName, std::string& path, fbxsdk::FbxMesh* mesh);
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	void CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* t, size_t& tArrayIdx, const ZobMaterial* tex);
	void LoadOBJ(const std::string& fullPath);
	void LoadFbx(const std::string& fullPath);
	void FbxMultT(FbxNode* node, FbxVector4 &vector);
	inline bool RejectTriangle(const Triangle* t, const float znear, const float zfar, const float width, const float height);
	std::vector<Mesh*> m_subMeshes;
	uint m_nbVertices = 0;
	uint m_nbUvs = 0;
	uint m_nbNormals = 0;
	uint m_nbFaces = 0;
	
	bool m_hasNormals = false;
	std::string m_name;
	std::string m_fileName;
	std::string m_path;
	ZobVector3 m_minBouding;
	ZobVector3 m_maxBouding;
	ZobVector3 m_pivot;
	ZobVector3* m_vertices = NULL;
	ZobVector3* m_verticesTmp = NULL;
	ZobVector3* m_verticesData = NULL;

	ZobVector3* m_projectedVertices = NULL;
	ZobVector3* m_projectedVerticesTmp = NULL;

	ZobVector3* m_verticesNormals = NULL;
	ZobVector3* m_verticesNormalsData = NULL;
	ZobVector3* m_verticesNormalsTmp = NULL;

	ZobVector3* m_trianglesNormals = NULL;
	ZobVector3* m_trianglesNormalsData = NULL;
	ZobVector3* m_trianglesNormalsTmp = NULL;

	ZobVector2* m_uvs = NULL;

	std::vector<Triangle> m_triangles;
	bool m_cullUsingFrustrum;
};


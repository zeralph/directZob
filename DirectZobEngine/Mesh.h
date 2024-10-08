#pragma once
#undef PropertyNotify
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "Rendering/ZobVector3.h"
#include "Rendering/ZobVector2.h"
#include "Types.h"
#include "ZobObjects/ZobEntity.h"
#include "Rendering/Triangle.h"
#include "ZobObjects/Camera.h"
#include "Events.h"
#include "Rendering/Engine.h"
#ifdef ENABLE_FBX_SUPPORT
#include <fbxsdk.h>
#endif
#include "../dependencies/tinygltf/include/tiny_gltf.h"
class ZobObject;
class ZobComponent;
class Mesh
{
public:

										Mesh(const std::string& name, ZobComponent* zm);
										Mesh(Mesh* m, ZobComponent* zm);
										Mesh(ZobFilePath* zfp, ZobComponent* zm);
	virtual								~Mesh();

	virtual void						Update(const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const Triangle::RenderOptions* options);
	virtual void						QueueForDrawing(ZobObject* z, const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine, const Triangle::RenderOptions* options);
	void								DrawBoundingBox(const ZobMatrix4x4& modelMatrix, const ZobMatrix4x4& rotationMatrix, const Camera* camera, Engine* engine);

	const uint							GetNbTriangles() const { return m_nbFaces; }
	const uint							GetNbVertices() const { return m_nbVertices; }
	const uint							GetNbUvs() const { return m_nbUvs; }
	const uint							GetNbNormals() const { return m_nbNormals; }
	const ZobVector3*					GetVertices() const { return m_meshVertices; }
	const std::vector<Triangle>*		GetTriangles() const { return &m_triangles; }
	const uint*							GetIndices() const { return m_indices; }
	const std::string&					GetName() const { return m_name; }
	const std::string&					GetPath() const { return m_path; }
	const std::string&					GetFileName() const { return m_fileName; }
	inline bool							IsDrawn() const { return m_bDrawn; }
	const int							GetNbSubMeshes() const { return m_subMeshes.size(); }
	const Mesh*							GetSubMesh(int idx) const { return (m_subMeshes.size()>idx)?m_subMeshes.at(idx):NULL; }
	const Box*							GetOBB() const { return &m_OBB; }
	const Box*							GetAABB() const { return &m_AABB; }
	void								SetVisible(bool v) { m_visible = v; }
	const bool							IsVisible() const { return m_visible; }
	const long							GetSize() const { return m_size; }
protected:
										Mesh(std::string& name, tinygltf::Model& model, tinygltf::Mesh& mesh, ZobComponent* zm);
#ifdef ENABLE_FBX_SUPPORT
										Mesh(std::string& parentName, ZobFilePath* zfp, fbxsdk::FbxMesh* mesh);
#endif
	void								SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim) const;
	void								CreateTriangles(const std::vector<std::string>* line, std::vector<Triangle>* t, size_t& tArrayIdx, const ZobMaterial* tex);
	void								LoadOBJ(ZobFilePath* zfp);
	void								LoadGlTF(ZobFilePath* zfp);
#ifdef ENABLE_FBX_SUPPORT
	void								LoadFbx(ZobFilePath* zfp);
	void								FbxMultT(FbxNode* node, FbxVector4 &vector);
#endif
	inline bool							RejectTriangle(const Triangle* t, const float znear, const float zfar, const float width, const float height);
	void								ParseObjFaceData(const std::string* s, int &v, int &u, int &n, bool& hasUv, bool& hasNormals) const;
	const long							ComputeSize() const;

	void 								bindGlTFModelNodes(ZobObject*z, tinygltf::Model &model, tinygltf::Node &node);
	ZobComponent* m_component;
	std::vector<Mesh*> m_subMeshes;
	uint m_nbVertices = 0;
	uint m_nbUvs = 0;
	uint m_nbNormals = 0;
	uint m_nbFaces = 0;
	bool m_bDrawn;
	bool m_visible;
	bool m_hasNormals;
	std::string m_name;
	std::string m_fileName;
	std::string m_path;
	ZobVector3 m_minBoundingBox;
	ZobVector3 m_maxBoundingBox;
	ZobVector3 m_pivot;

	uint* m_indices = NULL;

	ZobVector3* m_meshVertices = NULL;
	ZobVector3* m_verticesTmp = NULL;
	ZobVector3* m_verticesData = NULL;

	ZobVector3* m_triangleProjectedVertices = NULL;
	//ZobVector3* m_projectedVerticesTmp = NULL;

	ZobVector3* m_triangleVerticesNormals = NULL;
	ZobVector3* m_verticesNormalsData = NULL;
	ZobVector3* m_verticesNormalsTmp = NULL;

	ZobVector3* m_trianglesNormals = NULL;
	ZobVector3* m_trianglesNormalsData = NULL;
	ZobVector3* m_trianglesNormalsTmp = NULL;

	ZobVector2* m_triangleUvs = NULL;
	ZobColor* m_triangleColors = NULL;
	
	std::vector<Triangle> m_triangles;
	Box m_OBB;
	Box m_AABB;
	long m_size;
};


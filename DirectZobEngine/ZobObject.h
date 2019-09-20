#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>
#include "Camera.h"
#include "Types.h"
#include "Mesh.h"
#include "Engine.h"
#include "ZOBGUID.h"

class ZobObject:public ZOBGUID
{
public:

	ZobObject(ZOBGUID::Type t, ZOBGUID::SubType s, std::string& name, Mesh* mesh, ZobObject* parent = NULL);
	~ZobObject();

	virtual void Update(const Matrix4x4& parentMatrix);
	virtual void Draw(const Camera* camera, Core::Engine* engine);

	//void AddChild(ZobObject* z) { m_children.push_back(z); }
	const std::vector<ZobObject*>* getChildren() const { return &m_children; }
	ZobObject* GetParent() const { return m_parent; }
	void SetParent(ZobObject* o);
	ZobObject* GetChild(const std::string& name);
	ZobObject* GetChild(const int i);
	RenderOptions& GetRenderOptions() { return m_renderOptions; }
	inline void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	inline void SetTranslation(float x, float y, float z) { m_translation.x = x; m_translation.y = y; m_translation.z = z; }
	inline const Vector3& GetTransform() const { return m_translation; }
	inline const Vector3& GetRotation() const { return m_rotation; }
	inline const Vector3& GetScale() const { return m_scale; }
	inline const std::string& GetName() const { return m_name; }
	inline void SetName(const std::string &name) { m_name = name; }
	const void GetFullNodeName(std::string &fullName) const;
	inline const int GetNbChildren() const { return m_children.size(); }
	void RemoveChildReference(const ZobObject* z);
	void AddChildReference(ZobObject* z);
	int GetChildPosition(const ZobObject* z);
	const std::string GetMeshName() const ;
	void SetMesh(std::string name);
protected:
	
	bool HasChild(const ZobObject* o);

	ZobObject* m_parent = NULL;
	Mesh* m_mesh = NULL;
	std::vector<ZobObject*> m_children;
	Matrix4x4 m_modelMatrix;
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	std::string m_name;
	RenderOptions m_renderOptions;
};
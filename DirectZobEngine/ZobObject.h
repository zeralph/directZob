#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>
#include "Types.h"
#include "Engine.h"
#include "ZOBGUID.h"
#include "tinyxml.h"

class Mesh;
class ZobObject:public ZOBGUID
{
public:

	ZobObject(ZOBGUID::Type t, ZOBGUID::SubType s, const std::string& name, Mesh* mesh, ZobObject* parent = NULL);
	ZobObject(ZOBGUID::Type t, ZOBGUID::SubType s, TiXmlElement* node, Mesh* mesh, ZobObject* parent);
	~ZobObject();

	virtual void Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix);
	virtual void Draw(const Camera* camera, Core::Engine* engine);
	
	//void AddChild(ZobObject* z) { m_children.push_back(z); }
	const std::vector<ZobObject*>* getChildren() const { return &m_children; }
	ZobObject* GetParent() const { return m_parent; }
	void SetParent(ZobObject* o);
	ZobObject* GetChild(const std::string& name);
	ZobObject* GetChild(const int i);
	virtual inline void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }
	virtual inline void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	virtual inline void SetTranslation(float x, float y, float z) { m_translation.x = x; m_translation.y = y; m_translation.z = z; }
	inline const Vector3& GetTransform() const { return m_translation; }
	inline const Vector3 GetWorldPosition() const 
	{ 
		return m_modelMatrix.GetTranslation();
	}
	inline const Vector3 GetWorldScale() const
	{
		return m_modelMatrix.GetScale();
	}
	inline const Vector3& GetRotation() const { return m_rotation; }
	inline const Vector3& GetScale() const { return m_scale; }
	inline const std::string& GetName() const { return m_name; }
	inline void SetName(const std::string &name) { m_name = name; }
	const void GetFullNodeName(std::string &fullName) const;
	inline const int GetNbChildren() const { return (int)m_children.size(); }
	void RemoveChildReference(const ZobObject* z);
	void AddChildReference(ZobObject* z);
	int GetChildPosition(const ZobObject* z);
	RenderOptions* GetRenderOptions() { return &m_renderOptions; };
	const std::string GetMeshName() const ;
	void SetMesh(std::string name);
	void SetLightingMode(RenderOptions::eLightMode l);
	const bool IsMarkedForDeletion() const { return m_markedForDeletion; };
	void MarkForDeletion() { m_markedForDeletion=true; };
	const RenderOptions::eLightMode GetLightingMode() const { return m_renderOptions.LightMode(); }
protected:
	
	bool HasChild(const ZobObject* o);
	void DrawGizmos(const Camera* camera, Core::Engine* engine);
	ZobObject* m_parent;
	Mesh* m_mesh = NULL;
	std::vector<ZobObject*> m_children;
	Matrix4x4 m_modelMatrix;
	Matrix4x4 m_rotationScaleMatrix;
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	std::string m_name;
	RenderOptions m_renderOptions;
	bool m_markedForDeletion;
};
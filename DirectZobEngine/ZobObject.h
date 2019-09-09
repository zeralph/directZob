#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>
#include "Camera.h"
#include "Types.h"
#include "Mesh.h"
#include"Engine.h"

class ZobObject
{
public:
	ZobObject(std::string& name, Mesh* mesh, const ZobObject* parent = NULL) 
	{
		m_name = name;
		m_parent = parent;
		m_mesh = mesh;
		m_translation = Vector3(0, 0, 0);
		m_rotation = Vector3(0, 0, 0);
		m_scale = Vector3(1, 1, 1);
	}
	~ZobObject() {}



	virtual void Update(const Matrix4x4& parentMatrix);
	virtual void Draw(const Camera* camera, Core::Engine* engine);

	void AddChild(ZobObject* z) { m_children.push_back(z); }
	const std::vector<ZobObject*>* getChildren() const { return &m_children; }
	const ZobObject* GetParent() const { return m_parent; }
	inline void SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; }
	inline void SetRotation(float x, float y, float z) { m_rotation.x = x; m_rotation.y = y; m_rotation.z = z; }
	inline void SetTranslation(float x, float y, float z) { m_translation.x = x; m_translation.y = y; m_translation.z = z; }
	inline const std::string& GetName() const { return m_name; }


protected:
	const ZobObject* m_parent = NULL;
	Mesh* m_mesh = NULL;
	std::vector<ZobObject*> m_children;
	Matrix4x4 m_modelMatrix;
	Vector3 m_translation;
	Vector3 m_rotation;
	Vector3 m_scale;
	std::string m_name;
};
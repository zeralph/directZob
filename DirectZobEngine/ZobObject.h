#pragma once
#include "ZobVector3.h"
#include "ZobMatrix4x4.h"
#include <string>
#include <vector>
#include "Types.h"
#include "Engine.h"
#include "ZOBGUID.h"
#include "tinyxml.h"

class Mesh;
class ZobPhysicComponent;
class ZobObject:public ZOBGUID
{
public:

	ZobObject(ZOBGUID::Type t, ZOBGUID::SubType s, const std::string& name, Mesh* mesh, ZobObject* parent = NULL);
	ZobObject(ZOBGUID::Type t, ZOBGUID::SubType s, TiXmlElement* node, Mesh* mesh, ZobObject* parent);
	virtual ~ZobObject();

	//virtuals
	virtual void					Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix);
	virtual void					UpdateMesh(const Camera* camera, Core::Engine* engine);
	virtual void					QueueForDrawing(const Camera* camera, Core::Engine* engine);
	inline const ZobVector3&		GetScale() const { return m_scale; }
	virtual inline void				SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z = z; };

	void							SetRotation(float x, float y, float z);
	void							SetPosition(float x, float y, float z);
	const ZobVector3*				GetRotation() const;
	const ZobVector3*				GetPosition() const;

	virtual void					DrawGizmos(const Camera* camera, Core::Engine* engine);
	virtual TiXmlNode*				SaveUnderNode(TiXmlNode* node);
	//
	const std::vector<ZobObject*>*	GetChildren() const { return &m_children; };
	ZobObject*						GetParent() const { return m_parent; };
	void							SetParent(ZobObject* o);
	ZobObject*						GetChild(const std::string& name);
	ZobObject*						GetChild(const int i);

	inline const ZobVector3			GetWorldPosition() const
	{
		return m_modelMatrix.GetTranslation();
	};
	inline const ZobVector3			GetWorldScale() const
	{
		return m_modelMatrix.GetScale();
	};

	
	inline const std::string&		GetName() const { return m_name; }
	inline void						SetName(const std::string &name) { m_name = name; }
	inline ZobVector3				GetForward() const { return &m_forward; }
	inline ZobVector3				GetLeft() const { return &m_left; }
	inline ZobVector3				GetUp() const { return &m_up; }
	const void						GetFullNodeName(std::string &fullName) const;
	inline const int				GetNbChildren() const { return (int)m_children.size(); }
	void							RemoveChildReference(const ZobObject* z);
	void							AddChildReference(ZobObject* z);
	int								GetChildPosition(const ZobObject* z);
	RenderOptions*					GetRenderOptions() { return &m_renderOptions; };
	virtual const std::string		GetMeshName() const ;
	void							SetMesh(std::string name);
	void							SetLightingMode(RenderOptions::eLightMode l);
	const bool						IsMarkedForDeletion() const { return m_markedForDeletion; };
	void							MarkForDeletion() { m_markedForDeletion=true; };
	const RenderOptions::eLightMode GetLightingMode() const { return m_renderOptions.lightMode; };
	bool							HasChild(const ZobObject* o);
	void							CreateSprite();
protected:

	ZobObject* m_parent;
	ZobPhysicComponent* m_physicComponent;
	Mesh* m_mesh = NULL;
	std::vector<ZobObject*> m_children;
	ZobMatrix4x4 m_modelMatrix;
	ZobMatrix4x4 m_rotationScaleMatrix;
	ZobVector3 m_scale;
	std::string m_name;
	RenderOptions m_renderOptions;
	bool m_markedForDeletion;
	ZobVector3 m_left;
	ZobVector3 m_forward;
	ZobVector3 m_up;
};
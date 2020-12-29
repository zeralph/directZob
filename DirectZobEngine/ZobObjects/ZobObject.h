#pragma once
#include "../Rendering/ZobVector3.h"
#include "../Rendering/ZobMatrix4x4.h"
#include <string>
#include <vector>
#include "../Types.h"
#include "../Rendering/Engine.h"
#include "ZOBGUID.h"
#include "tinyxml.h"
#include "../Behaviors/ZobBehaviorFactory.h"

class Mesh;
class ZobPhysicComponent;
class ZobObject:public ZOBGUID
{
friend class ZobBehavior;
friend class ZobBehaviorCar;
public:

	ZobObject(ZOBGUID::Type t, ZOBGUID::SubType s, const std::string& name, ZobObject* parent = NULL, const std::string* factoryFile =NULL);
	ZobObject(DirectZobType::guid id, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile = NULL);
	virtual ~ZobObject();

	//virtuals
	//virtual void					Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix);
	virtual void					Init();
	virtual void					PreUpdate();
	virtual void					Update();
	virtual void					UpdateBehavior(float dt);
	virtual void					UpdateMesh(const Camera* camera, Core::Engine* engine);
	virtual void					QueueForDrawing(const Camera* camera, Core::Engine* engine);
	ZobVector3						GetScale() const;
	void 							SetScale(float x, float y, float z);
	void							SetWorldRotation(float x, float y, float z);
	void							SetWorldPosition(float x, float y, float z);
	void							LookAt(const ZobVector3* target, bool addToCurrentRotation);
	void							LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up, bool addToCurrentRotation);
	inline ZobVector3				GetLocalRotation() const;
	inline ZobVector3				GetLocalPosition() const;
	ZobVector3						GetWorldRotation() const;
	ZobVector3						GetWorldPosition() const;
	void							SaveTransform();
	void							RestoreTransform();
	void							ResetPhysic();

	virtual void					DrawGizmos(const Camera* camera, Core::Engine* engine);
	virtual TiXmlNode*				SaveUnderNode(TiXmlNode* node);
	//
	const std::vector<ZobObject*>*	GetChildren() const { return &m_children; };
	ZobObject*						GetParent() const { return m_parent; };
	void							SetParent(ZobObject* o);
	ZobObject*						GetChild(const std::string& name);
	ZobObject*						GetChild(const int i);

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
	bool							RemoveChildReference(const ZobObject* z);
	bool							AddChildReference(ZobObject* z);
	int								GetChildPosition(const ZobObject* z);
	RenderOptions*					GetRenderOptions() { return &m_renderOptions; };
	virtual const std::string		GetMeshName() const ;
	virtual const std::string		GetMeshFileName() const;
	virtual const std::string		GetMeshPath() const;
	void							SetMesh(std::string name);
	void							LoadMesh(std::string name, std::string file, std::string path);
	void							SetLightingMode(RenderOptions::eLightMode l);
	const bool						IsMarkedForDeletion() const { return m_markedForDeletion; };
	void							MarkForDeletion() { m_markedForDeletion=true; };
	const RenderOptions::eLightMode GetLightingMode() const { return m_renderOptions.lightMode; };
	bool							HasChild(const ZobObject* o);
	void							CreateSprite();
	void							SetPhysicComponent(int i);
	bool							IsFromFactoryFile() {return m_factoryFile.length()>0;}
	std::string&					FactoryFile() { return m_factoryFile; }
	void							SaveToFactoryFile(std::string& file);
	const ZobPhysicComponent*		GetPhysicComponent() const { return m_physicComponent; };
	ZobPhysicComponent*				GetPhysicComponentNoConst() { return m_physicComponent; };
	//temp ?
	const ZobMatrix4x4*				GetModelMatrix() const { return &m_modelMatrix; };
	const ZobMatrix4x4*				GetRotationScaleMatrix() const { return &m_rotationScaleMatrix; };

	//Physic interface, mainly for editor
	void							GetPhysicComponentInfo(std::string& type, std::string& shapeType) const;
	void							SetPhysicComponentInfo(std::string& type, std::string& shapeType);
	void							GetPhysicComponentShapeInfo(float& radius, float& height, float& hx, float& hy, float& hz, std::string&  mesh);
	void							SetPhysicComponentShapeInfo(float radius, float height, float hx, float hy, float hz, std::string& mesh);
	void							GetPhysicComponentColliderInfo(float& bounciness, float& frictionCoeff, float& massDensity, float& RollingResistance);
	void							SetPhysicComponentColliderInfo(float bounciness, float frictionCoeff, float massDensity, float RollingResistance);
	void							GetPhysicComponentScaleWithObject(bool& b);
	void							SetPhysicComponentScaleWithObject(bool b);

private:
	void							SaveRecusrive(TiXmlNode* node, ZobObject* z);
	void							SetParentInternal();
	void							DeleteInternal();
protected:

	ZobObject* m_parent;
	ZobObject* m_newParent;
	ZobPhysicComponent* m_physicComponent;
	Mesh* m_mesh = NULL;
	std::vector<ZobObject*> m_children;
	ZobBehavior* m_behavior;
	ZobMatrix4x4 m_modelMatrix;
	ZobMatrix4x4 m_rotationScaleMatrix;
	std::string m_name;
	RenderOptions m_renderOptions;
	bool m_markedForDeletion;
	ZobVector3 m_left;
	ZobVector3 m_forward;
	ZobVector3 m_up;
	std::string m_factoryFile;
};
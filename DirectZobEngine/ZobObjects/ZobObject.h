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
class m_varExposer;
class ZobBehaviorMesh;
class ZobBehaviorSprite;
class ZobObject:public ZOBGUID
{
friend class ZobBehavior;
friend class ZobBehaviorCar;
public:

	ZobObject(ZOBGUID::ZobType t, ZOBGUID::ZobSubType s, const std::string& name, ZobObject* parent = NULL, const std::string* factoryFile =NULL);
	ZobObject(std::string id, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile = NULL);
	virtual ~ZobObject();

	//Mechanics
	virtual void						Init();
	virtual void						PreUpdate(float dt);
	virtual void						UpdatePhysic(float dt);
	virtual void						Update(float dt);
	virtual void						PostUpdate();
	virtual void						EditorUpdate();
	virtual void						QueueForDrawing(const Camera* camera,Engine* engine);
	virtual void						DrawGizmos(const Camera* camera, Engine* engine);
	virtual								TiXmlNode* SaveUnderNode(TiXmlNode* node);
	//
	void								SaveTransform();
	void								RestoreTransform();
	void								ResetPhysic();
	void								RegenerateZobIds();
	//World
	ZobVector3							GetWorldPosition() const;
	void								SetWorldPosition(float x, float y, float z);
	ZobVector3							GetWorldRotation() const;
	ZobVector3							GetWorldScale() const;
	void								SetWorldRotation(float x, float y, float z);
	ZobVector3							GetLocalScale() const;
	void 								SetLocalScale(float x, float y, float z);
	void 								SetWorldScale(float x, float y, float z);
	//Local
	ZobVector3							GetLocalPosition() const;
	void								SetLocalPosition(float x, float y, float z);
	ZobVector3							GetLocalRotation() const;
	void								SetLocalRotation(float x, float y, float z, bool add);
	void								SetLocalOrientation(const ZobVector3* axis, float angle, bool add);
	void								GetLocalAxisAngleRotation(ZobVector3& axis, float& angle);
	//Parenting
	const std::vector<ZobObject*>*		GetChildren() const { return &m_children; };
	ZobObject*							GetParent() const { return m_parent; };
	void								SetParent(ZobObject* o);
	ZobObject*							GetChild(const std::string& name);
	ZobObject*							GetChild(const int i);

	void								LookAt(const ZobVector3* target, bool addToCurrentRotation);
	void								LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up, bool addToCurrentRotation);
	
	inline const std::string&			GetName() const { return m_name; }
	inline void							SetName(const std::string &name) { m_name = name; }
	inline ZobVector3					GetForward() const { return &m_forward; }
	inline ZobVector3					GetLeft() const { return &m_left; }
	inline ZobVector3					GetUp() const { return &m_up; }
	const void							GetFullNodeName(std::string &fullName) const;
	inline const int					GetNbChildren() const { return (int)m_children.size(); }
	bool								RemoveChildReference(const ZobObject* z);
	bool								AddChildReference(ZobObject* z);
	int									GetChildPosition(const ZobObject* z);
	ZobBehaviorMesh*					LoadMesh(ZobFilePath& zfp, bool bEditorZobBehavior);
	ZobBehaviorSprite*					LoadSprite(ZobFilePath& zfp, bool bEditorZobBehavior);
	const bool							IsMarkedForDeletion() const { return m_markedForDeletion; };
	void								MarkForDeletion() { m_markedForDeletion=true; };
	bool								HasChild(const ZobObject* o);
	void								CreateSprite();
	bool								IsFromFactoryFile() {return m_factoryFile.length()>0;}
	std::string&						FactoryFile() { return m_factoryFile; }
	void								SaveToFactoryFile(std::string& file);
	const ZobPhysicComponent*			GetPhysicComponent() const { return m_physicComponent; };
	ZobPhysicComponent*					GetPhysicComponentNoConst() { return m_physicComponent; };
	//temp ?
	const ZobMatrix4x4*					GetModelMatrix() const { return &m_modelMatrix; };
	const ZobMatrix4x4*					GetRotationMatrix() const { return &m_rotationMatrix; };
	void								AddBehavior(ZobBehavior* b) { m_behaviors.push_back(b); }
	ZobVariablesExposer*				GetVariablesExposer() { return m_varExposer; }

	//behaviors
	const std::vector<ZobBehavior*>*	GetBehaviors() const { return &m_behaviors; }
	const ZobBehavior*					GetBehaviorById(zobId zid) const;

	static void							ReloadVariablesFromLocalData(zobId id);
	static void							ReloadVariablesFromWorldData(zobId id);
private:
	void								SaveRecusrive(TiXmlNode* node, ZobObject* z);
	void								DeleteInternal();
	
protected:

	virtual void						InitVariablesExposer();

	ZobObject* m_parent;
	ZobVariablesExposer* m_varExposer;
	ZobPhysicComponent* m_physicComponent;
	std::vector<ZobObject*> m_children;
	std::vector <ZobBehavior*> m_behaviors;
	ZobMatrix4x4 m_modelMatrix;
	ZobMatrix4x4 m_rotationMatrix;
	std::string m_name;
	bool m_markedForDeletion;
	ZobVector3 m_left;
	ZobVector3 m_forward;
	ZobVector3 m_up;
	std::string m_factoryFile;
};
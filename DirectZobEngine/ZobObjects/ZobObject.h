#pragma once
#include "../Rendering/ZobVector3.h"
#include "../Rendering/ZobMatrix4x4.h"
#include <string>
#include <vector>
#include "../Types.h"
#include "ZobEntity.h"
#include "../Components/ZobComponentFactory.h"
#include "../Misc/ZobFilePath.h"

#include "../Components/GraphicComponents/ZobComponentSprite.h"
#include "../Components/GraphicComponents/ZobComponentMesh.h"
#include "../Components/GraphicComponents/ZobComponentSkybox.h"
#include "../Components/HudComponents/ZobComponentText.h"
#include "../Components/HudComponents/ZobComponentImage.h"
#include "../Components/ZobComponentRotator.h"

class Engine;
class TiXmlNode;
class TiXmlElement;
class ZobPhysicComponent;
class ZobVariablesExposer;
class ZobObject:public ZobEntity
{
friend class ZobComponent;
friend class ZobComponentCar;
public:

	ZobObject(ZobEntity::ZobType t, ZobEntity::ZobSubType s, const std::string& name, ZobObject* parent = NULL);
	ZobObject(zobId id, TiXmlElement* node, ZobObject* parent);
	virtual ~ZobObject();

	//Mechanics
	virtual void						Init(DirectZobType::sceneLoadingCallback cb) override;
	virtual void						Start();
	virtual void						Stop();
	virtual void						PreUpdate(float dt, bool isPlaying);
	virtual void						UpdatePhysic(float dt);
	virtual void						Update(float dt, bool isPlaying);
	virtual void						PostUpdate(bool isPlaying);
	virtual void						EditorUpdate(bool isPlaying);
	virtual void						QueueForDrawing(const Camera* camera,Engine* engine);
	virtual void						DrawGizmos(const Camera* camera, Engine* engine);
	virtual								TiXmlNode* SaveUnderNode(TiXmlNode* node);
	ZobObject*							Duplicate() override;
	void								SaveAsAsset(std::string& file);
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
	void								SetLocalFromOpenglMatrix(double* mat);
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
	
	inline ZobVector3					GetForward() const { return &m_forward; }
	inline ZobVector3					GetLeft() const { return &m_left; }
	inline ZobVector3					GetUp() const { return &m_up; }
	const void							GetFullNodeName(std::string &fullName) const;
	inline const int					GetNbChildren() const { return (int)m_children.size(); }
	bool								RemoveChildReference(const ZobObject* z);
	bool								AddChildReference(ZobObject* z);
	int									GetChildPosition(const ZobObject* z);
	ZobComponentMesh*					LoadMesh(ZobFilePath& zfp, bool bEditorZobComponent);
	ZobComponentSprite*					LoadSprite(ZobFilePath& zfp, bool bEditorZobComponent);
	const bool							IsMarkedForDeletion() const { return m_markedForDeletion; };
	void								MarkForDeletion() { m_markedForDeletion=true; };
	bool								HasChild(const ZobObject* o);
	void								CreateSprite();
	void								SaveToFactoryFile(std::string& file);
	const ZobPhysicComponent*			GetPhysicComponent() const { return m_physicComponent; };
	ZobPhysicComponent*					GetPhysicComponentNoConst() { return m_physicComponent; };
	void								SetVisible(bool v, bool propagate);
	bool								IsVisible() const { return m_visible; };
	void								DeleteMarkedObjects();
	//temp ?
	const ZobMatrix4x4*					GetModelMatrix() const { return &m_modelMatrix; };
	const ZobMatrix4x4*					GetRotationMatrix() const { return &m_rotationMatrix; };
	void								AddComponent(ZobComponent* b) { m_Components.push_back(b); }


	//Components
	const std::vector<ZobComponent*>*	GetComponents() const { return &m_Components; }
	template<class T>
	T* GetComponent() const
	{
		std::vector<ZobComponent*>::const_iterator it = m_Components.begin();
		while (it != m_Components.end())
		{
			ZobComponent* ptr = (*it);
			bool b = typeid(T) == typeid(*ptr);
			if (b)
			{
				return static_cast<T*>(*it);
			}
			it++;
		}
		return NULL;
	}
	template<class T>
	std::vector<T*> GetComponents() const
	{
		std::vector<T*> v;
		std::vector<ZobComponent*>::const_iterator it = m_Components.begin();
		while (it != m_Components.end())
		{
			ZobComponent* ptr = (*it);
			bool b = typeid(T) == typeid(*ptr);
			if (b)
			{
				v.push_back(static_cast<T*>(*it));
			}
			it++;
		}
		return v;
	}

	static void							ReloadVariablesFromLocalData(zobId id);
	static void							ReloadVariablesFromWorldData(zobId id);
private:
	void								SaveRecusrive(TiXmlNode* node, ZobObject* z);

protected:

	virtual void						InitVariablesExposer();

	ZobObject*							m_parent;
	ZobPhysicComponent*					m_physicComponent;
	std::vector<ZobObject*>				m_children;
	std::vector <ZobComponent*>			m_Components;
	ZobMatrix4x4						m_modelMatrix;
	ZobMatrix4x4						m_rotationMatrix;
	bool								m_markedForDeletion;
	ZobVector3							m_left;
	ZobVector3							m_forward;
	ZobVector3							m_up;
	bool								m_visible;
};
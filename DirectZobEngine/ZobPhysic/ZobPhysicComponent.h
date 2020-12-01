#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>
#include "Rendering/ZobVector3.h"
#include "ZobObjects/Camera.h"
#include "tinyxml.h"

using namespace reactphysics3d;

class ZobPhysicComponent
{
public:

	enum ePhysicComponentType
	{
		ePhysicComponentType_none = 0,
		ePhysicComponentType_static,
		ePhysicComponentType_dynamic,
	};

	//used only for drawing shapes
	struct sShapeDraw
	{
		enum eShapeType
		{
			eShapeType_unknown = 0,
			eShapeType_sphere,
			eShapeType_capsule,
			eShapeType_box,
			eShapeType_convexMesh,
			__eShapeType_MAX__
		};
		eShapeType _type = eShapeType_unknown;
		ZobVector3 _halfExtends = ZobVector3();
		float _radius;
		float _height;
		//a voir pour le mesh
	};

	ZobPhysicComponent(TiXmlNode* t);
	~ZobPhysicComponent();
	void								Init(const ZobVector3* position, const ZobVector3* rotation);
	TiXmlNode*							SaveUnderNode(TiXmlNode* node);
	void								Set(ePhysicComponentType t);
	void								SetPosition(float x, float y, float z);
	void								SetOrientation(float x, float y, float z);
	void								SetQuaternion(float x, float y, float z, float w);
	void								SetQuaternion(const ZobVector3* left, const ZobVector3* up, const ZobVector3* fw);
	ZobVector3							GetPosition() const ;
	ZobVector3							GetOrientation() const ;
	ZobMatrix4x4						GetRotationMatrix() const;
	ZobVector3							GetScale() const { return ZobVector3(m_scale.x, m_scale.y, m_scale.z); }
	void								SetScale(float x, float y, float z) { m_scale.x = x; m_scale.y = y; m_scale.z=z; }
	void								LookAt(const ZobVector3* target);
	void								LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up);
	void								AddBoxCollider(const ZobVector3* halfExtends );
	void								AddSphereCollider(float radius);
	void								AddCapsuleCollider(float radius, float height);
	void								Update();
	void								SaveTransform();
	void								RestoreTransform();
	void								ResetPhysic();
	void								DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation);
	ZobVector3							GetTotalScale() const { return ZobVector3(m_totalScale.x, m_totalScale.y, m_totalScale.z); }
	void								SetTotalScale(float x, float y, float z) { m_totalScale.x = x; m_totalScale.y = y; m_totalScale.z = z; }
	const Transform						GetWorldTransform() const { return m_rigidBody?m_rigidBody->getTransform():Transform();/* Transform(m_worldTransform);*/ };
	Transform							GetLocalTransform() const { return Transform(m_localTransform); };
	void								SetWorldTransform(Transform t) { m_worldTransform = Transform(t); };
	void								SetLocalTransform(Transform t) { m_localTransform = t; };
	void								SetLocalPosition(Vector3 p) { m_localTransform.setPosition(p); };
	void								SetLocalOrientation(Quaternion q) { m_localTransform.setOrientation(q); };
	Quaternion							QuaternionFromAxisAngle(Vector3* axis, float angle);
private:
	void								AddColliderInternal(CollisionShape* c);
	void								CreateCollider();
	float								ClampAngle(float a) const;
	
	ePhysicComponentType m_type;
	RigidBody* m_rigidBody;
	Collider* m_collider;	
	Transform m_savedTransform;
	sShapeDraw m_shapeDraw;
	Transform m_localTransform;
	Transform m_worldTransform;
	Vector3 m_scale; 
	Vector3 m_totalScale;
};
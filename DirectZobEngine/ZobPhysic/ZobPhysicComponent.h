#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#undef None
#include <reactphysics3d/reactphysics3d.h>
#include "../Rendering/ZobVector3.h"
#include "../ZobObjects/Camera.h"
#include "tinyxml.h"

using namespace reactphysics3d;
class ZobObject;
class ZobPhysicComponent
{
public:

	enum eContactType
	{
		ContactStart,
		ContactStay,
		ContactExit
	};

	enum eLayer
	{
		eLayer_none = 0x0000,
		eLayer_ground = 0x0001,
		eLayer_wall = 0x0002,
		eLayer_objects = 0x0004,
	};

	struct collision
	{
		const ZobPhysicComponent* other;
		ZobVector3 collisionWorldPosition;
		ZobVector3 collisionWorldNormal;
		ZobVector3 collisionWorldDirection;
		ZobPhysicComponent::eLayer collisionLayer;
		eContactType contactType;
		float penetration;
		bool handled;
		void Reset()
		{
			other = NULL;
			handled = true;
		};
	};

	ZobPhysicComponent(ZobObject* z);
	~ZobPhysicComponent();
	void								Init(const ZobVector3* position, const ZobVector3* rotation);
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
	void								Update();
	void								SaveTransform();
	void								RestoreTransform();
	void								ResetPhysic();
	void								DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation);
	ZobVector3							GetTotalScale() const { return ZobVector3(m_totalScale.x, m_totalScale.y, m_totalScale.z); }
	void								SetTotalScale(float x, float y, float z);
	const Transform						GetWorldTransform() const { return Transform(m_worldTransform); };
	Transform							GetLocalTransform() const { return Transform(m_localTransform); };
	void								SetWorldTransform(Transform t) { m_worldTransform = t; };
	void								SetLocalTransform(Transform t) { m_localTransform = t; };
	void								SetLocalPosition(Vector3 p) { m_localTransform.setPosition(p); };
	void								SetLocalOrientation(Quaternion q) { m_localTransform.setOrientation(q); };
	Quaternion							QuaternionFromAxisAngle(Vector3* axis, float angle);
	CollisionBody*						GetCollisionBody() { return m_collisionBody; }
	void								SetScaleWithObject(bool b) { m_scaleWithObject = b; m_bUpdateSize = true; }
	bool								GetScaleWithObject() const { return m_scaleWithObject; }
	void								OnCollide(collision coll);
	collision*							GetLastCollision() { return &m_lastCollision; }
	void								SetBodyType(ZobObject::eBodyType bt);
	ZobObject::eBodyType				GetBodyType() const { return m_bodyType; }
private:

	float								ClampAngle(float a) const;

	ZobObject::eBodyType m_bodyType;
	const ZobObject* m_zobObject;
	CollisionBody* m_collisionBody;
	Collider* m_collider;	
	Transform m_savedTransform;
	Transform m_localTransform;
	Transform m_worldTransform;
	collision m_lastCollision;
	Vector3 m_scale;
	Vector3 m_totalScale;
	bool m_scaleWithObject;
	bool m_bUpdateSize;
};
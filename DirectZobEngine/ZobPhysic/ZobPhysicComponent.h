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
		ZobVector3 collisionLocalContactPoint;
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
	//Mechanics
	void								Init();
	void								Update();
	void								EditorUpdate();
	void								SaveTransform();
	void								RestoreTransform();
	void								ResetPhysic();
	void								DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation);
	//World
	Transform							GetWorldTransform() const;
	void								SetWorldTransform(Transform t);
	ZobVector3							GetWorldOrientation() const;
	void								SetWorldOrientation(float x, float y, float z);
	Vector3								GetWorldScale() const;
	void								SetWorldScale(float x, float y, float z);
	//Local
	Transform							GetLocalTransform() const { return Transform(m_localTransform); };
	void								SetLocalTransform(Transform t);
	ZobVector3							GetLocalOrientation() const;
	void								SetLocalOrientation(Quaternion q) { m_localTransform.setOrientation(q); };
	ZobVector3							GetLocalScale() const { return ZobVector3(m_localScale.x, m_localScale.y, m_localScale.z); }
	void								SetLocalScale(float x, float y, float z) { m_localScale.x = x; m_localScale.y = y; m_localScale.z = z; }
	ZobMatrix4x4						GetRotationMatrix() const;
	ZobMatrix4x4						GetModelMatrix() const;
	void								LookAt(const ZobVector3* target);
	void								LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up);
	Quaternion							QuaternionFromAxisAngle(Vector3* axis, float angle);
	CollisionBody*						GetCollisionBody() { return m_collisionBody; }
	void								OnCollide(collision coll);
	//Serialization & editor
	collision*							GetLastCollision() { return &m_lastCollision; }
	ZobVector3*							GetLocalPositionAddress() { return &m_editorLocalPosition; }
	ZobVector3*							GetLocalRotationAddress() { return &m_editorLocalRotation; }
	ZobVector3*							GetLocalScaleAddress() { return &m_editorLocalScale; }
	ZobVector3*							GetWorldPositionAddress() { return &m_editorWorldPosition; }
	ZobVector3*							GetWorldRotationAddress() { return &m_editorWorldRotation; }
	ZobVector3*							GetWorldScaleAddress() { return &m_editorWorldScale; }
	void								WorldOrientationToAxis(ZobVector3& left, ZobVector3& up, ZobVector3& forward) const;
private:

	float								ClampAngle(float a) const;
	Transform							GetParentWorldTransform() const;
	Vector3								GetParentWorldScale() const;
	const ZobObject*					m_zobObject;
	CollisionBody*						m_collisionBody;
	Collider*							m_collider;	
	Transform							m_savedTransform;
	Transform							m_localTransform;
	collision							m_lastCollision;
	Vector3								m_localScale;
	ZobVector3							m_editorLocalPosition;
	ZobVector3							m_editorLocalRotation;
	ZobVector3							m_editorLocalScale;
	bool								m_scaleWithObject;

	//editor purpose only
	ZobVector3							m_editorWorldPosition;
	ZobVector3							m_editorWorldRotation;
	ZobVector3							m_editorWorldScale;
};
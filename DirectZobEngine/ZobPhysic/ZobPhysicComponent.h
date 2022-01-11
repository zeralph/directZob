#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#undef None
#include <reactphysics3d/reactphysics3d.h>
#include "../Rendering/ZobVector3.h"
#include "../ZobObjects/Camera.h"

using namespace reactphysics3d;
class ZobObject;
class ZobPhysicComponent
{
public:

	friend class ZobObject;

	enum eRigidBodyType
	{
		eRigidBody_static = 0,
		eRigidBody_kinematic = 1,
		eRigidBody_dynamic = 2,
	};

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
		eLayer_4 = 0x0008,
		eLayer_5 = 0x0010,
		eLayer_6 = 0x0020,
		eLayer_7 = 0x0040,
		eLayer_8 = 0x0080,
		eLayer_9 = 0x0100,
		eLayer_10 = 0x0200,
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
	void								GetLocalAxisAngleRotation(ZobVector3& axis, float& angle);
	void								SetLocalOrientation(Quaternion q, bool add) 
										{ 
											Quaternion qn = m_localTransform.getOrientation();
											if (add)
											{
												q = qn * q;
											}
											m_localTransform.setOrientation(q); 
										};
	ZobVector3							GetLocalScale() const { return ZobVector3(m_localScale.x, m_localScale.y, m_localScale.z); }
	void								SetLocalScale(float x, float y, float z) { m_localScale.x = x; m_localScale.y = y; m_localScale.z = z; }
	ZobMatrix4x4						GetRotationMatrix() const;
	ZobMatrix4x4						GetModelMatrix() const;
	void								LookAt(const ZobVector3* target);
	void								LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up);
	static Quaternion					QuaternionFromAxisAngle(Vector3* axis, float angle);
	RigidBody*							GetRigidBody() { return m_rigidBody; }
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
	void								SetBodyType(eRigidBodyType t) { m_rigidBody->setType((BodyType)t); }
	eRigidBodyType						GetBodyType() { return (eRigidBodyType)m_rigidBody->getType(); }
	const ZobObject*					GetZObObject() const { return m_zobObject; }
	static void							UpdateProperties(zobId zid);
private:
	void								UpdatePropertiesInternal();
	float								ClampAngle(float a) const;
	Transform							GetParentWorldTransform() const;
	Vector3								GetParentWorldScale() const;
	const ZobObject*					m_zobObject;
	RigidBody*							m_rigidBody;
	//Collider*							m_collider;	
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
	//
	eRigidBodyType						m_bodyType;
	float								m_mass;
	bool								m_active;
};
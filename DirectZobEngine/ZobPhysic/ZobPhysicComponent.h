#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>
#include "../Rendering/ZobVector3.h"
#include "../ZobObjects/Camera.h"
#include "tinyxml.h"

using namespace reactphysics3d;
class ZobObject;
class ZobPhysicComponent
{
public:

	struct collision
	{
		const ZobPhysicComponent* other;
		ZobVector3 worldPosition;
		ZobVector3 worldNormal;
		float penetration;
		bool handled;
		void Reset()
		{
			other = NULL;
			handled = true;
		};
	};

	enum ePhysicComponentType
	{
		ePhysicComponentType_static= (int)rp3d::BodyType::STATIC,
		ePhysicComponentType_dynamic = (int)rp3d::BodyType::DYNAMIC,
	};

	enum eShapeType
	{
		eShapeType_uninit=-1,
		eShapeType_none = 0,
		eShapeType_sphere,
		eShapeType_capsule,
		eShapeType_box,
		eShapeType_convexMesh,
		__eShapeType_MAX__
	};

	ZobPhysicComponent(ZobObject* z, TiXmlNode* t);
	~ZobPhysicComponent();
	void								Init(const ZobVector3* position, const ZobVector3* rotation);
	TiXmlNode*							SaveUnderNode(TiXmlNode* node);
	void								SetType(ePhysicComponentType t);
	ePhysicComponentType				GetType() const { return m_type; };
	void								SetShapeType(eShapeType t);
	eShapeType							GetShapeType() const { return m_shapeType; };
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
	void								AddBoxCollider();
	void								AddSphereCollider();
	void								AddCapsuleCollider();
	void								AddMeshCollider();
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
	bool								SetRadius(float f);
	bool								SetHalfextends(float x, float y, float z);
	bool								SetHeight(float h);
	float								GetRadius() const { return m_radius; }
	float								GetHeight() const { return m_height; }
	ZobVector3							GetHalfExtends() const { return m_halfExtends; }
	std::string							GetMesh() const { return std::string("not set"); }
	Collider*							GetCollider() { return m_collider; }
	RigidBody*							GetRigicBody() { return m_rigidBody; }
	void								SetScaleWithObject(bool b) { m_scaleWithObject = b; m_bUpdateSize = true; }
	bool								GetScaleWithObject() const { return m_scaleWithObject; }
	void								OnCollide(collision coll);
	collision*							GetLastCollision() { return &m_lastCollision; }
private:
	void								AddColliderInternal(CollisionShape* c);
	float								ClampAngle(float a) const;
	bool								UpdateColliderSize();
	void								UpdateShapeType();
	void								RemoveCollider();
	void								WriteColliderNode(TiXmlNode* node);
	void								ReadColliderNode(TiXmlNode* node);
	void								WriteMaterialNode(TiXmlNode* node);
	void								ReadMaterialNode(TiXmlNode* node);
	float*	m_concaveMeshVertices;
	uint* m_concaveMeshIndices;
	int m_concaveMeshNbTriangles;
	const ZobObject* m_zobObject;
	ePhysicComponentType m_type;
	RigidBody* m_rigidBody;
	Collider* m_collider;	
	Transform m_savedTransform;
	eShapeType m_shapeType;
	eShapeType m_nextShapeType;
	Transform m_localTransform;
	Transform m_worldTransform;
	Vector3 m_scale; 
	Vector3 m_totalScale;
	bool m_scaleWithObject;
	float m_radius;
	ZobVector3 m_halfExtends;
	std::string m_convexMeshName;
	std::string m_convexMeshPath;
	std::string m_convexMeshFile;
	float m_height;
	bool m_bUpdateSize;
	collision m_lastCollision;
};
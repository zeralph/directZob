#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>
#include "ZobVector3.h"
#include "Camera.h"
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
	void								Init();
	TiXmlNode*							SaveUnderNode(TiXmlNode* node);
	void								Set(ePhysicComponentType t);
	void								SetPosition(float x, float y, float z);
	void								SetOrientation(float x, float y, float z);
	void								SetQuaternion(float x, float y, float z, float w);
	void								SetQuaternion(const ZobVector3* left, const ZobVector3* up, const ZobVector3* fw);
	const ZobVector3*					GetPosition();
	const ZobVector3*					GetOrientation();
	const ZobMatrix4x4					GetRotationMatrix();
	void								AddBoxCollider(const ZobVector3* halfExtends );
	void								AddSphereCollider(float radius);
	void								AddCapsuleCollider(float radius, float height);

	void								SaveTransform();
	void								RestoreTransform();
	void								ResetPhysic();
	void								DrawGizmos(const Camera* camera, const ZobMatrix4x4* mat);
private:
	void								AddColliderInternal(CollisionShape* c);
	void								CreateCollider();
	ePhysicComponentType m_type;
	RigidBody* m_rigidBody;
	Collider* m_collider;	
	Transform m_savedTransform;
	sShapeDraw m_shapeDraw;
	ZobVector3 m_position;
	ZobVector3 m_orientation;
};
#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>
#include "ZobVector3.h"
#include "Camera.h"

using namespace reactphysics3d;

class ZobPhysicComponent
{
public:

	enum ePhysicComponentType
	{
		ePhysicComponentType_none = 0,
		ePhysicComponentType_collisionBody,
		ePhysicComponentType_rigidBody,
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

	ZobPhysicComponent();
	~ZobPhysicComponent();
	void								Set(ePhysicComponentType t);
	void								UnSet();
	void								SetPosition(float x, float y, float z);
	void								SetOrientation(float x, float y, float z);
	const ZobVector3*					GetPosition();
	const ZobVector3*					GetOrientation();

	void								AddBoxCollider(const ZobVector3* halfExtends );
	void								AddSphereCollider(float radius);
	void								AddCapsuleCollider(float radius, float height);

	void								SaveTransform();
	void								RestoreTransform();
	void								ResetPhysic();
	void								DrawGizmos(const Camera* camera, const ZobMatrix4x4* mat);
private:
	void								AddColliderInternal(CollisionShape* c);
	ePhysicComponentType m_type;
	CollisionBody* m_collisionBody;
	RigidBody* m_rigidBody;
	ZobVector3 m_position;
	ZobVector3 m_orientation;
	Transform m_savedTransform;
	sShapeDraw m_shapeDraw;
};
#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>
#include "ZobVector3.h"

using namespace reactphysics3d;

class ZobPhysicsEngine
{
public :
	ZobPhysicsEngine();
	~ZobPhysicsEngine();
	void StartUpdatePhysic(float dt);
	float WaitForUpdatePhysicEnd();
	RigidBody* CreateRigidBody(const ZobVector3* position, const ZobVector3* orientation);
	void DestroyRigidBody(RigidBody* rb);
	PhysicsCommon* GetPhysicsCommon() { return &m_physicsCommon; }
private:
	void Update(float dt);
	PhysicsCommon m_physicsCommon;
	PhysicsWorld* m_world;
	clock_t m_timeStep;
	float m_accumulator;
};
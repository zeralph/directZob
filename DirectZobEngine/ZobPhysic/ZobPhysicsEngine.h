#pragma once
// no idea where these are defined; undefine them, as reactphysics will use methods which have these macro names
#undef min
#undef max
#undef None
#include <reactphysics3d/reactphysics3d.h>
#include "Rendering/ZobVector3.h"
#include <vector>

using namespace reactphysics3d;
class ZobPhysicComponent;
class ZobPhysicsContactsListener;
class ZobPhysicsEngine
{
public:

public :
	ZobPhysicsEngine();
	~ZobPhysicsEngine();
	void ReInit();
	void StartUpdatePhysic(float dt);
	float WaitForUpdatePhysicEnd();
	RigidBody* CreateRigidBody(const ZobVector3* position, const ZobVector3* orientation);
	void DestroyRigidBody(RigidBody* rb);
	PhysicsCommon* GetPhysicsCommon() { return &m_physicsCommon; }
	const PhysicsWorld* GetWorld() const { return m_world; }
	void ResetAccumulator() { m_accumulator = 0; }
	const std::vector<ZobPhysicComponent*>* GetBodies() const { return &m_worldCollisionBodies; }
	void AddBody(ZobPhysicComponent* c);
	void RemoveBody(const ZobPhysicComponent* b);
	ZobPhysicComponent* GetZobComponentFromCollisionBody(const CollisionBody* rb) const;
	void DrawGizmos() const;
	bool HasRigiBodyCollision(std::string& a, std::string& b);
private:
	void Init();
	void UnInit();
	void Update(float dt);
	int GetBodyWorldIndex(const ZobPhysicComponent* b) const;
	PhysicsCommon m_physicsCommon;
	PhysicsWorld* m_world;
	PhysicsWorld::WorldSettings m_worldSettings;
	clock_t m_timeStep;
	float m_accumulator;
	std::vector<ZobPhysicComponent*> m_worldCollisionBodies;
	ZobPhysicsContactsListener* m_contactsListener;

};
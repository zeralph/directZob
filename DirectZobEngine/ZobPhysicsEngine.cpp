#include "ZobPhysicsEngine.h"


ZobPhysicsEngine::ZobPhysicsEngine()
{
    m_world = m_physicsCommon.createPhysicsWorld();
    m_timeStep = 0;
}

ZobPhysicsEngine::~ZobPhysicsEngine()
{

}

void ZobPhysicsEngine::Update()
{
    m_timeStep = clock() - m_timeStep;
    m_timeStep = 1.0f / 20.0f;
    m_world->update(m_timeStep);
}

CollisionBody* ZobPhysicsEngine::CreateCollisionBody(const ZobVector3* position, const ZobVector3* orientation)
{
    Vector3 p(position->x, position->y, position->z);
    Quaternion o = Quaternion::identity();
    o.fromEulerAngles(orientation->x, orientation->y, orientation->z);
    Transform t(p, o);
    return m_world->createCollisionBody(t);
}

RigidBody* ZobPhysicsEngine::CreateRigidBody(const ZobVector3* position, const ZobVector3* orientation)
{
    Vector3 p(position->x, position->y, position->z);
    Quaternion o = Quaternion::identity();
    o.fromEulerAngles(orientation->x, orientation->y, orientation->z);
    Transform t(p, o);
    return m_world->createRigidBody(t);
}

void ZobPhysicsEngine::DestroyCollisionBody(CollisionBody* cb)
{
    m_world->destroyCollisionBody(cb);
}

void ZobPhysicsEngine::DestroyRigidBody(RigidBody* rb)
{
    m_world->destroyRigidBody(rb);
}
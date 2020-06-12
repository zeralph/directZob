#include "ZobPhysicsEngine.h"
#include <thread>
static std::thread g_physicThread;

ZobPhysicsEngine::ZobPhysicsEngine()
{
    m_world = m_physicsCommon.createPhysicsWorld();
    m_timeStep = clock();
}

ZobPhysicsEngine::~ZobPhysicsEngine()
{

}


void ZobPhysicsEngine::StartUpdatePhysic(float dt)
{
	
	g_physicThread = std::thread(&ZobPhysicsEngine::Update, this, dt);
}

float ZobPhysicsEngine::WaitForUpdatePhysicEnd()
{
	if (g_physicThread.joinable())
		g_physicThread.join();
	return m_timeStep;
}


void ZobPhysicsEngine::Update(float dt)
{
	m_timeStep = clock();
    m_world->update(1.0f / dt);
	m_timeStep = (float)(clock() - m_timeStep) / CLOCKS_PER_SEC;
}

RigidBody* ZobPhysicsEngine::CreateRigidBody(const ZobVector3* position, const ZobVector3* orientation)
{
    Vector3 p(position->x, position->y, position->z);
    Quaternion o = Quaternion::identity();
    o.fromEulerAngles(orientation->x, orientation->y, orientation->z);
    Transform t(p, o);
    return m_world->createRigidBody(t);
}

void ZobPhysicsEngine::DestroyRigidBody(RigidBody* rb)
{
    m_world->destroyRigidBody(rb);
}
#include "ZobPhysicsEngine.h"
#include <thread>
static std::thread g_physicThread;

static bool sPhysicThreaded = false;

ZobPhysicsEngine::ZobPhysicsEngine()
{
    m_world = m_physicsCommon.createPhysicsWorld(m_worldSettings);
    m_timeStep = clock();
    m_accumulator = 0;
}

ZobPhysicsEngine::~ZobPhysicsEngine()
{

}


void ZobPhysicsEngine::StartUpdatePhysic(float dt)
{
    if (sPhysicThreaded)
    {
        g_physicThread = std::thread(&ZobPhysicsEngine::Update, this, dt);
    }
    else
    {
        ZobPhysicsEngine::Update(dt);
    }
}

float ZobPhysicsEngine::WaitForUpdatePhysicEnd()
{
    if (sPhysicThreaded)
    {
        if (g_physicThread.joinable())
            g_physicThread.join();
    }
	return m_timeStep;
}


void ZobPhysicsEngine::Update(float dt)
{
	m_timeStep = clock();
    // Constant physics time step 
    const float timeStep = 1.0f / 60.0f;
    // Add the time difference in the accumulator 
    m_accumulator += dt;
    // While there is enough accumulated time to take 
    // one or several physics steps 
    while (m_accumulator >= timeStep)
    {
        // Update the Dynamics world with a constant time step 
        m_world->update(timeStep);
        // Decrease the accumulated time 
        m_accumulator -= timeStep;
    }
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
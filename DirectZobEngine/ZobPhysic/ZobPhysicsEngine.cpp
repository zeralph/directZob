#include "ZobPhysicsEngine.h"
#include "ZobPhysicComponent.h"
#include "ZobPhysicsContactsListener.h"
#include "../Managers/CameraManager.h"
#include "../ZobObjects/Camera.h"
#include <thread>
#undef None
#include "../../dependencies/optick/include/optick.h"
static std::thread g_physicThread;

static bool sPhysicThreaded = false;

ZobPhysicsEngine::ZobPhysicsEngine()
{
    m_contactsListener = NULL;
    m_world = NULL;
    UnInit();
    Init();
}

ZobPhysicsEngine::~ZobPhysicsEngine()
{
    UnInit();
}

void ZobPhysicsEngine::ReInit()
{
    UnInit();
    Init();
}

void ZobPhysicsEngine::Init()
{
    m_worldSettings.worldName = "World";
    m_world = m_physicsCommon.createPhysicsWorld(m_worldSettings);
    m_world->enableSleeping(true);
    m_contactsListener = new ZobPhysicsContactsListener();
    m_world->setEventListener(m_contactsListener);
    m_timeStep = clock();
    m_accumulator = 0;
    m_worldCollisionBodies.resize(0);
    m_world->setIsDebugRenderingEnabled(true);
}

void ZobPhysicsEngine::UnInit()
{
    if(m_contactsListener)
    {
        delete m_contactsListener;
        m_contactsListener = NULL;
    }
    if(m_world)
    {
        m_physicsCommon.destroyPhysicsWorld(m_world);
        m_world = NULL;
    }
}

void ZobPhysicsEngine::AddBody(ZobPhysicComponent* c)
{ 
    if (GetBodyWorldIndex(c) == -1)
    {
        m_worldCollisionBodies.push_back(c);
    }
}

void ZobPhysicsEngine::RemoveBody(const ZobPhysicComponent* b)
{ 
    int i = GetBodyWorldIndex(b);
    assert(i > -1);
    m_worldCollisionBodies.erase(m_worldCollisionBodies.begin()+i);
}

ZobPhysicComponent* ZobPhysicsEngine::GetZobComponentFromCollisionBody(const CollisionBody* rb) const
{
    std::vector<ZobPhysicComponent*>::const_iterator iter;
    for (iter = m_worldCollisionBodies.begin(); iter != m_worldCollisionBodies.end(); iter++)
    {
        CollisionBody* t = static_cast<CollisionBody*>((*iter)->GetRigidBody());
        if (t == rb)
        {
            return *iter;
        }
    }
    return NULL;
}

int ZobPhysicsEngine::GetBodyWorldIndex(const ZobPhysicComponent* b) const
{
    int s = m_worldCollisionBodies.size();
    for (int i=0; i<s; i++)
    {
        if (m_worldCollisionBodies.at(i) == b)
        {
            return i;
        }
    }
    return  -1;
}

void ZobPhysicsEngine::StartUpdatePhysic(float dt)
{
    OPTICK_EVENT();
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

bool ZobPhysicsEngine::HasRigiBodyCollision(std::string &a, std::string &b)
{
    uint32 nr = m_world->getNbRigidBodies();
    if (nr > 0)
    {
        for (uint32 i = 0; i < nr; i++)
        {
            RigidBody* r = m_world->getRigidBody(i);
            if (r->getType() == BodyType::DYNAMIC)
            {
                uint32 nc = m_world->getNbRigidBodies();
                for (uint32 j = 0; j < nc; j++)
                {
                    RigidBody* c = m_world->getRigidBody(j);
                    if (r != c && m_world->testOverlap(r, c))
                    {
                        a = ZobPhysicsEngine::GetZobComponentFromCollisionBody(r)->GetZObObject()->GetName();
                        b = ZobPhysicsEngine::GetZobComponentFromCollisionBody(c)->GetZObObject()->GetName();
                        return true;
                    }
                }
            }
        }
    }
    return false;
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
    int cnt = 0;
    while (m_accumulator >= timeStep)
    {
        // Update the Dynamics world with a constant time step 
        m_world->update(timeStep);
        // Decrease the accumulated time 
        m_accumulator -= timeStep;
        cnt++;
    }
	m_timeStep = (float)(clock() - m_timeStep) / CLOCKS_PER_SEC;
}

RigidBody* ZobPhysicsEngine::CreateRigidBody(const ZobVector3* position, const ZobVector3* orientation)
{
    Vector3 p(position->x, position->y, position->z);
    Quaternion o = Quaternion::identity();
    o.fromEulerAngles(orientation->x, orientation->y, orientation->z);
    o.normalize();
    Transform t(p, o);
    //return m_world->createCollisionBody(t);
    RigidBody* r = m_world->createRigidBody(t);
    r->setType(BodyType::STATIC);
    return r;
}

void ZobPhysicsEngine::DestroyRigidBody(RigidBody* rb)
{
    m_world->destroyRigidBody(rb);
}

void ZobPhysicsEngine::DrawGizmos() const
{
    Engine* engine = DirectZob::GetInstance()->GetEngine();
    const Camera* camera = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
    if (engine && camera && engine->DrawGizmos())
    {
        reactphysics3d::DebugRenderer debugRenderer = m_world->getDebugRenderer();
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_POINT, true);
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_NORMAL, true);
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
        debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_AABB, true);
        debugRenderer.computeDebugRenderingPrimitives(*m_world);
        int nbLines = debugRenderer.getNbLines();
        int nbTriangles= debugRenderer.getNbTriangles();
        ZobVector3 v1;
        ZobVector3 v2;
        ZobVector3 v3;
        ZobColor zc;
        for (int i = 0; i < nbLines; i++)
        {
            reactphysics3d::DebugRenderer::DebugLine l = debugRenderer.getLinesArray()[i];
            v1.x = l.point1.x;
            v1.y = l.point1.y;
            v1.z = l.point1.z;
            v2.x = l.point2.x;
            v2.y = l.point2.y;
            v2.z = l.point2.z;
            zc = ZobColor(l.color1);
            engine->QueueLine(camera, &v1, &v2, &zc, false, false);
        }
        for (int i = 0; i < nbTriangles; i++)
        {
            reactphysics3d::DebugRenderer::DebugTriangle t = debugRenderer.getTrianglesArray()[i];
            v1.x = t.point1.x;
            v1.y = t.point1.y;
            v1.z = t.point1.z;
            v2.x = t.point2.x;
            v2.y = t.point2.y;
            v2.z = t.point2.z;
            v3.x = t.point3.x;
            v3.y = t.point3.y;
            v3.z = t.point3.z;
            zc = ZobColor(t.color1);
            engine->QueueLine(camera, &v1, &v2, &zc, false, false);
            engine->QueueLine(camera, &v2, &v3, &zc, false, false);
            engine->QueueLine(camera, &v3, &v1, &zc, false, false);
        }
    }
}
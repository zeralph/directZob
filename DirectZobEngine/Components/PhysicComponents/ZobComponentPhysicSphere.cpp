#include "ZobComponentPhysicSphere.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobComponentPhysicSphere::~ZobComponentPhysicSphere()
{

}

ZobComponentPhysicSphere::ZobComponentPhysicSphere(ZobObject* zobObject) : ZobComponentPhysicShape(zobObject)
{
	SET_CLASS_AND_NAME
	m_componentType = eComponent_physicSphere;
	m_sphereShape = NULL;
	m_localPostion = ZobVector3(0,0,0);
	m_varExposer->WrapVariable<float>("Radius", &m_radius, NULL, false, true);
	m_radius = 1.0f;
}

void ZobComponentPhysicSphere::Init(DirectZobType::sceneLoadingCallback cb)
{	
	ReLoadVariables();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_sphereShape = pc->createSphereShape(m_radius);
	AddColliderInternal(m_sphereShape);
	ZobComponentPhysicShape::Init(cb);
}

void ZobComponentPhysicSphere::EditorUpdate()
{
	if (m_isInit)
	{
		ZobComponentPhysicShape::EditorUpdate();
		if (m_sphereShape->getRadius() != m_radius)
		{
			m_sphereShape->setRadius(m_radius);
		}
	}
}
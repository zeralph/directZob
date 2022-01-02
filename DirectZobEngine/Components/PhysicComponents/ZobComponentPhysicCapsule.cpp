#include "ZobComponentPhysicCapsule.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobComponentPhysicCapsule::~ZobComponentPhysicCapsule()
{
	
}

ZobComponentPhysicCapsule::ZobComponentPhysicCapsule(ZobObject* zobObject) : ZobComponentPhysicShape(zobObject)
{
	m_type = eComponent_physicCapsule;
	m_capsuleShape = NULL;
	m_varExposer->WrapVariable<float>("Radius", &m_radius, NULL, false, true);
	m_varExposer->WrapVariable<float>("Height", &m_height, NULL, false, true);
	m_radius = 1.0f;
	m_height = 2.0f;
}


void ZobComponentPhysicCapsule::Init()
{
	ReLoadVariables();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_capsuleShape = pc->createCapsuleShape(m_radius, m_height);
	AddColliderInternal(m_capsuleShape);
	ZobComponentPhysicShape::Init();
}

void ZobComponentPhysicCapsule::EditorUpdate()
{
	if (m_isInit)
	{
		ZobComponentPhysicShape::EditorUpdate();
		if (m_capsuleShape->getRadius() != m_radius)
		{
			m_capsuleShape->setRadius(m_radius);
		}
		if (m_capsuleShape->getHeight() != m_height)
		{
			m_capsuleShape->setHeight(m_height);
		}
	}
}

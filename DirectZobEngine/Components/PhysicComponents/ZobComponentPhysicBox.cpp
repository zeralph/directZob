#include "ZobComponentPhysicBox.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobComponentPhysicBox::~ZobComponentPhysicBox()
{
	m_boxShape = NULL;
}

ZobComponentPhysicBox::ZobComponentPhysicBox(ZobObject* zobObject) : ZobComponentPhysicShape(zobObject)
{
	SET_CLASS_AND_NAME
	m_halfExtends = ZobVector3(1, 1, 1);
	m_type = eComponent_physicBox;
	m_varExposer->WrapVariable<ZobVector3>("Height", &m_halfExtends, NULL, false, true);
	m_halfExtends = ZobVector3(1, 1, 1);
}

void ZobComponentPhysicBox::Init()
{
	ReLoadVariables();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	Vector3 h = Vector3(m_halfExtends.x, m_halfExtends.y, m_halfExtends.z);
	m_boxShape = pc->createBoxShape(h);
	AddColliderInternal(m_boxShape);
	ZobComponentPhysicShape::Init();
}

void ZobComponentPhysicBox::EditorUpdate()
{
	if (m_isInit)
	{
		ZobComponentPhysicShape::EditorUpdate();
		Vector3 h = m_boxShape->getHalfExtents();
		if (h.x != m_halfExtends.x || h.y != m_halfExtends.y || h.z != m_halfExtends.z )
		{
			h.x = m_halfExtends.x;
			h.y = m_halfExtends.y;
			h.z = m_halfExtends.z;
			m_boxShape->setHalfExtents(h);
		}
	}
}

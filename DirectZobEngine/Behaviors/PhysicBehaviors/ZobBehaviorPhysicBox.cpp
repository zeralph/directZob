#include "ZobBehaviorPhysicBox.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicBox::~ZobBehaviorPhysicBox()
{
	m_boxShape = NULL;
}

ZobBehaviorPhysicBox::ZobBehaviorPhysicBox(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_halfExtends = ZobVector3(1, 1, 1);
	m_type = eBehavior_physicBox;
	WrapVariable(eWrapperType_ZobVector3, "Height", &m_halfExtends, false, true);
	m_halfExtends = ZobVector3(1, 1, 1);
}

void ZobBehaviorPhysicBox::Init()
{
	ReLoadVariables();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	Vector3 h = Vector3(m_halfExtends.x, m_halfExtends.y, m_halfExtends.z);
	m_boxShape = pc->createBoxShape(h);
	AddColliderInternal(m_boxShape);
	ZobBehaviorPhysicShape::Init();
}

void ZobBehaviorPhysicBox::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
	Vector3 h = m_boxShape->getHalfExtents();
	if (h.x != m_halfExtends.x || h.y != m_halfExtends.y || h.z != m_halfExtends.z )
	{
		h.x = m_halfExtends.x;
		h.y = m_halfExtends.y;
		h.z = m_halfExtends.z;
		m_boxShape->setHalfExtents(h);
	}
}

#include "ZobBehaviorPhysicBox.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicBox::~ZobBehaviorPhysicBox()
{

}

ZobBehaviorPhysicBox::ZobBehaviorPhysicBox(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_type = eBehavior_physicBox;
	Init();
}

void ZobBehaviorPhysicBox::Init()
{
	ZobBehaviorPhysicShape::Init();
	m_halfExtends = ZobVector3(1, 1, 1);
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	Vector3 h = Vector3(m_halfExtends.x, m_halfExtends.y, m_halfExtends.z);
	BoxShape* s = pc->createBoxShape(h);
	AddColliderInternal(s);
}

void ZobBehaviorPhysicBox::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
}

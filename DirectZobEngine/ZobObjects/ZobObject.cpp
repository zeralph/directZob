#include "ZobObject.h"
#include "DirectZob.h"
#include "Mesh.h"
#include "Sprite.h"
#include "ZobPhysic/ZobPhysicComponent.h"
#include "SceneLoader.h"
#include "Behaviors/ZobBehaviorMesh.h"
#include "../Misc/ZobXmlHelper.h"
#include "../../dependencies/optick/include/optick.h"

static int sObjectNumber = 0;
ZobObject::ZobObject(ZobType t, ZobSubType s, const std::string& name, ZobObject* parent /*= NULL*/, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(t,s)
{
	DirectZob::LogInfo("ZobObject %s creation", name.c_str()); 
	DirectZob::AddIndent();
	m_varExposer = new ZobVariablesExposer(GetIdValue());
	sObjectNumber++;
	m_behaviors.clear();
	m_factoryFile = factoryFile?factoryFile->c_str():"";
	m_physicComponent = new ZobPhysicComponent(this);
	if (name.length() == 0)
	{
		std::string n = "newObject_";
		n.append(std::to_string(sObjectNumber));
		m_name = n;
	}
	else
	{
		m_name = name;
	}
	DirectZob::LogInfo("Adding new ZobObject %s", m_name.c_str());
	m_children.clear();
	if(m_name != "root")
	{
		if (!parent)
		{
			m_parent = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
		}
		else
		{
			m_parent = parent;
		}
		SetParent(m_parent);
	}
	else
	{
		m_parent = NULL;
	}
	m_markedForDeletion = false;
	m_mesh = NULL;		
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_physicComponent->GetLocalPositionAddress()->x = 0;
	m_physicComponent->GetLocalPositionAddress()->y = 0;
	m_physicComponent->GetLocalPositionAddress()->z = 0;
	m_physicComponent->GetLocalRotationAddress()->x = 0;
	m_physicComponent->GetLocalRotationAddress()->y = 0;
	m_physicComponent->GetLocalRotationAddress()->z = 0;
	m_physicComponent->GetLocalScaleAddress()->x = 1;
	m_physicComponent->GetLocalScaleAddress()->y = 1;
	m_physicComponent->GetLocalScaleAddress()->z = 1;
	InitVariablesExposer();
	m_physicComponent->Init();
	DirectZob::RemoveIndent();
}

ZobObject::ZobObject(std::string id, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(id)
{
	m_varExposer = new ZobVariablesExposer(GetIdValue());
	m_behaviors.clear();
	sObjectNumber++;
	m_factoryFile = factoryFile ? factoryFile->c_str() : "";
	float x, y, z;
	TiXmlElement* f;
	DirectZob::LogInfo("ZobObject %s creation", id.c_str());
	DirectZob::AddIndent();
	if (!parent && m_name != "root")
	{
		m_parent = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
		//DirectZob::LogError("Cannot add %s because it has no parent", name.c_str());
	}
	else
	{
		m_parent = parent;
	}
	m_markedForDeletion = false;
	m_mesh = NULL;
	m_physicComponent = new ZobPhysicComponent(this);
	//parenting
	m_children.clear();
	SetParent(m_parent);
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	InitVariablesExposer();
	m_varExposer->ReadNode(node);
	//Behavior
	TiXmlElement* behaviorsNode = node->FirstChildElement("Behaviors");
	if(behaviorsNode)
	{
		for (TiXmlElement* e = behaviorsNode->FirstChildElement("Behavior"); e != NULL; e = e->NextSiblingElement("Behavior"))
		{
			ZobBehavior* b = ZobBehaviorFactory::CreateBehavior(this, e);
		}
	}	
	//Init
	m_physicComponent->Init();
	DirectZob::RemoveIndent();
}

void ZobObject::InitVariablesExposer()
{
	m_varExposer->WrapVariable<zobId>("GUID", GetIdAddress(), NULL, true, false);
	m_varExposer->WrapVariable<std::string>("Name", &m_name, NULL, false, true);
	m_varExposer->WrapVariable<ZobVector3>("Position", GetPhysicComponentNoConst()->GetLocalPositionAddress(), &ZobObject::ReloadVariables, false, true);
	m_varExposer->WrapVariable<ZobVector3>("Rotation", GetPhysicComponentNoConst()->GetLocalRotationAddress(), &ZobObject::ReloadVariables, false, true);
	m_varExposer->WrapVariable<ZobVector3>("Scale", GetPhysicComponentNoConst()->GetLocalScaleAddress(), &ZobObject::ReloadVariables, false, true);

}

void ZobObject::ReloadVariables(zobId id)
{
	ZobObjectManager* zm = DirectZob::GetInstance()->GetZobObjectManager();
	if (zm)
	{
		ZobObject* z = zm->GetZobObjectFromlId(id);
		if (z)
		{
			ZobVector3* zp = z->GetPhysicComponentNoConst()->GetLocalPositionAddress();
			ZobVector3* zr = z->GetPhysicComponentNoConst()->GetLocalRotationAddress();
			ZobVector3* zs = z->GetPhysicComponentNoConst()->GetLocalScaleAddress();
			Transform t = z->GetPhysicComponentNoConst()->GetLocalTransform();
			Vector3 vp = Vector3(zp->x, zp->y, zp->z);
			t.setPosition(vp);
			Quaternion q = Quaternion::fromEulerAngles(DEG_TO_RAD(zr->x), DEG_TO_RAD(zr->y), DEG_TO_RAD(zr->z));
			t.setOrientation(q);

			z->GetPhysicComponentNoConst()->SetLocalTransform(t);
		}
	}
}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("delete ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();

	DirectZob::GetInstance()->GetZobObjectManager()->AddIdToDeleted(GetIdValue());
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		delete m_behaviors[i];
	}
	delete m_physicComponent;
	if (m_parent != NULL)
	{
		m_parent->RemoveChildReference(this);
	}
	m_parent = NULL;
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		m_children[i] = NULL;
		delete z;
	}
	delete m_varExposer;
	m_children.resize(0);
	DirectZob::RemoveIndent();
}

ZobBehaviorMesh* ZobObject::LoadMesh(ZobFilePath &zfp)
{
	ZobBehavior* b = ZobBehaviorFactory::CreateBehavior(this, "Mesh");
	ZobBehaviorMesh* bm = static_cast<ZobBehaviorMesh*>(b);
	bm->Set(zfp);
	return bm;
}

void ZobObject::SetMesh(std::string name)
{
	Mesh* m = DirectZob::GetInstance()->GetMeshManager()->GetMesh(name);
	m_mesh = m;
}

const std::string ZobObject::GetMeshName() const
{
	if (m_mesh)
	{
		return m_mesh->GetName();
	}
	return "";
}

const std::string ZobObject::GetMeshPath() const
{
	if (m_mesh)
	{
		return m_mesh->GetPath();
	}
	return "";
}

const std::string ZobObject::GetMeshFileName() const
{
	if (m_mesh)
	{
		return m_mesh->GetFileName();
	}
	return "";
}

void ZobObject::Init()
{
	m_varExposer->Load();
	m_physicComponent->Init();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->Init();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Init();
	}
}

void ZobObject::EditorUpdate()
{
	m_physicComponent->EditorUpdate();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->EditorUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->EditorUpdate();
	}
}

void ZobObject::PreUpdate(float dt)
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->PreUpdate(dt);

	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->PreUpdate(dt);
	}
	DeleteInternal();
}

void ZobObject::PostUpdate()
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->PostUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if(z)
			z->PostUpdate();
	}
}

void ZobObject::QueueForDrawing(const Camera* camera, Engine* engine)
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->QueueForDrawing(camera, engine);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if (z)
			z->QueueForDrawing(camera, engine);
	}
}

void ZobObject::DeleteInternal()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if (z->IsMarkedForDeletion())
		{
			RemoveChildReference(z);
			delete z;
		}
//		else
//		{
//			z->PreUpdate(0);
//		}
	}
}

void ZobObject::UpdatePhysic(float dt)
{
	m_physicComponent->Update();
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->UpdatePhysic(dt);
	}
}

//void ZobObject::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
void ZobObject::Update(float dt)
{
	OPTICK_EVENT();
	UpdatePhysic(dt);
	Transform parentTransform;
	ZobVector3 parentScale;
	if (m_parent)
	{
		const ZobPhysicComponent* zpc = m_parent->GetPhysicComponent();
		parentTransform = zpc->GetWorldTransform();
		parentScale = zpc->GetWorldScale();
	}
	else
	{
		parentTransform = Transform::identity();
		parentScale = ZobVector3(1, 1, 1);
	}
	ZobVector3 scale = parentScale * m_physicComponent->GetLocalScale();
	m_physicComponent->SetWorldScale(scale.x, scale.y, scale.z);
	Quaternion q = m_physicComponent->GetWorldTransform().getOrientation();
	Vector3 l = q * Vector3(1, 0, 0);
	Vector3 u = q * Vector3(0, 1, 0);
	Vector3 f = q * Vector3(0, 0, 1);
	l.normalize();
	u.normalize();
	f.normalize();
	m_left = ZobVector3(l.x, l.y, l.z);
	m_forward = ZobVector3(f.x, f.y, f.z);
	m_up = ZobVector3(u.x, u.y, u.z);
	m_modelMatrix = m_physicComponent->GetModelMatrix();
	m_rotationMatrix = m_physicComponent->GetRotationMatrix();
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Update(dt);
	}
}

ZobVector3 ZobObject::GetScale() const 
{ 
	return m_physicComponent->GetWorldScale(); 
}

void ZobObject::SetScale(float x, float y, float z)
{ 
	ZobVector3 s = m_parent->GetScale();
	x /= s.x;
	y /= s.y;
	z /= s.z;
	m_physicComponent->SetLocalScale(x, y, z);
}

void ZobObject::DrawGizmos(const Camera* camera, Engine* engine)
{
	uint c;
	ZobVector3 x = m_left;
	ZobVector3 y = m_up;
	ZobVector3 z = m_forward;
	ZobVector3 t = GetWorldPosition();
	x = x + t;
	y = y + t;
	z = z + t;
	c = 0x00FF0000;
	engine->QueueLine(camera, &t, &x, c, true, false);
	c = 0x0000FF00;
	engine->QueueLine(camera, &t, &y, c, true, false);
	c = 0x000000FF;
	engine->QueueLine(camera, &t, &z, c, true, false);
	ZobVector3 p = GetWorldPosition();
	ZobVector3 r = GetWorldRotation();
	m_physicComponent->DrawGizmos(camera, &p, &r);
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->DrawGizmos(camera, &p, &r);
	}
}

int ZobObject::GetChildPosition(const ZobObject* z)
{
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) == z)
		{
			return i;
		}
	}
	return -1;
}

bool ZobObject::RemoveChildReference(const ZobObject* z)
{
	int i = GetChildPosition(z);
	if (i >= 0)
	{
		int size = m_children.size();
		std::swap(m_children.at(i), m_children.at(size - 1));
		m_children.pop_back();
		m_children.resize(size - 1);
		return true;
	}
	return false;
}

bool ZobObject::AddChildReference(ZobObject* z)
{
	m_children.push_back(z);
	return true;
}

ZobObject* ZobObject::GetChild(const std::string& name)
{
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return (*iter);
		}
	}
	return NULL;
}

ZobObject* ZobObject::GetChild(const int i)
{
	if (i >= 0 && i < m_children.size())
	{
		return m_children.at(i);
	}
	return NULL;
}

const void ZobObject::GetFullNodeName(std::string& fullname) const
{
	std::string s = GetName();
	std::string s2 = "";
	const ZobObject* z = this;
	while (z->GetParent())
	{
		z = z->GetParent();
		s2 = z->GetName();
		s2.append("/");
		s2.append(s);
		s = s2;
	}
	fullname = s;
}

void ZobObject::SetParent(ZobObject* p)
{
	if (!HasChild(p))
	{
		DirectZob::LogInfo("Reparented %s from %s to %s", ZobGuidToString().c_str(), m_parent->ZobGuidToString().c_str(), p->ZobGuidToString().c_str());
		ZobVector3 pos = GetWorldPosition();
		ZobVector3 rot = GetWorldRotation();
		ZobVector3 sca = GetScale();
		ZobObject* parent = GetParent();
		if (parent != p)
		{
			if (parent->RemoveChildReference(this))
			{
				if (p->AddChildReference(this))
				{
					m_parent = p;
					SetWorldPosition(pos.x, pos.y, pos.z);
					SetWorldRotation(rot.x, rot.y, rot.z);
				}
				else
				{
					throw("fuck");
				}
			}
			else
			{
				throw("fuck");
			}
		}
	}
	else
	{
		DirectZob::LogWarning("Trying to reparent an object with one of its descendants !");
	}
}

bool ZobObject::HasChild(const ZobObject* o)
{
	if (o == this)
	{
		return true;
	}
	else
	{
		bool bRet = false;
		for (int i = 0; i < m_children.size(); i++)
		{
			bRet |= m_children.at(i)->HasChild(o);
		}
		return bRet;
	}
}

void ZobObject::SaveToFactoryFile(std::string& file)
{
	TiXmlDocument doc(XML_ELEMENT_ZOBOBJECT);
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	SaveRecusrive(&doc, this);
	//doc.LinkEndChild(&n);
	if (!doc.SaveFile(file.c_str()))
	{
		DirectZob::LogError("Error saving zobObject : ", doc.ErrorDesc());
	}
}

void ZobObject::SaveRecusrive(TiXmlNode* node, ZobObject* z)
{
	if (z->GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	TiXmlNode* newNode = z->SaveUnderNode(node);
	for (int i = 0; i < z->GetNbChildren(); i++)
	{
		SaveRecusrive(newNode, z->GetChild(i));
	}
	node->InsertEndChild(*newNode);
}

TiXmlNode* ZobObject::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement* objectNode = new TiXmlElement(XML_ELEMENT_ZOBOBJECT);
	std::string guid = ZobGuidToString();
	objectNode->SetAttribute(XML_ATTR_GUID, guid.c_str());
	std::string meshName = GetMeshName();
	std::string meshFileName = GetMeshFileName();
	std::string meshPath = GetMeshPath();
	m_varExposer->SaveUnderNode(objectNode);
	TiXmlElement behaviors = TiXmlElement(XML_ELEMENT_BEHAVIORS);
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->SaveUnderNode(&behaviors);
	}
	objectNode->InsertEndChild(behaviors);
	return (TiXmlNode*)objectNode;
}

void ZobObject::CreateSprite()
{
	Sprite* s = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	m_mesh = s;
}
/*
void ZobObject::SetQuaternion(const ZobVector3* left, const ZobVector3* up, const ZobVector3* fw)
{
	m_physicComponent->SetQuaternion(left, up, fw);
}

void ZobObject::SetQuaternion(float x, float y, float z, float w)
{
	m_physicComponent->SetQuaternion(x, y, z, w);
}
*/
void ZobObject::LookAt(const ZobVector3* target, bool addToCurrentRotation)
{
	//m_physicComponent->LookAt(target);
	Transform t = Transform(m_physicComponent->GetWorldTransform());
	ZobVector3 v = ZobVector3(t.getPosition().x, t.getPosition().y, t.getPosition().z);
	v.x = target->x - v.x;
	v.y = target->y - v.y;
	v.z = target->z - v.z;
	v.Normalize();
	if (v.y > 0.99f)
	{
		int g = 0;
		g++;
	}
	//TODO if v.y > 0.99 ...
	ZobVector3 forward = v;
	ZobVector3 left = ZobVector3::Cross(&forward, &ZobVector3::Vector3Y);
	left.Normalize();
	left.Mul(-1.0f);
	ZobVector3 up = ZobVector3::Cross(&forward, &left);
	up.Normalize();
	LookAt(&forward, &left, &up, addToCurrentRotation);
}

void ZobObject::LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up, bool addToCurrentRotation)
{
	//m_physicComponent->LookAt(forward, left, up);
	Quaternion q;
	q.w = sqrt(fmaxf(0, 1 + left->x + up->y + forward->z)) / 2.0f;
	q.x = sqrt(fmaxf(0, 1 + left->x - up->y - forward->z)) / 2.0f;
	q.y = sqrt(fmaxf(0, 1 - left->x + up->y - forward->z)) / 2.0f;
	q.z = sqrt(fmaxf(0, 1 - left->x - up->y + forward->z)) / 2.0f;

	q.x = copysignf(q.x, up->z - forward->y);
	q.y = copysignf(q.y, forward->x - left->z);
	q.z = copysignf(q.z, left->y - up->x);

	//q.normalize();
	if (q.isValid())
	{
		if (false && addToCurrentRotation)
		{
			Quaternion q2 = m_physicComponent->GetLocalTransform().getOrientation();
			q = q2 * q;
			q.normalize();

		}
		Vector3 v = m_physicComponent->GetWorldTransform().getPosition();
		Transform t = Transform(v, q);
		m_physicComponent->SetWorldTransform(t);
		m_physicComponent->WorldOrientationToAxis(m_left, m_up, m_forward);
		//m_physicComponent->SetLocalOrientation(q);
	}
}

void ZobObject::SetWorldRotation(float x, float y, float z)
{
	//m_physicComponent->SetOrientation(x, y, z);
	float dy = DEG_TO_RAD(y);
	float dz = DEG_TO_RAD(z);
	float dx = DEG_TO_RAD(x);
	Quaternion q;
	ZobVector3 v = ZobMatrix4x4::EulerToQuaternion(dx, dy, dz);
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = v.w;
	m_physicComponent->SetLocalOrientation(q);
}

void ZobObject::SetLocalRotation(float x, float y, float z)
{
	//m_physicComponent->SetOrientation(x, y, z);
	float dy = DEG_TO_RAD(y);
	float dz = DEG_TO_RAD(z);
	float dx = DEG_TO_RAD(x);
	Quaternion q;
	ZobVector3 v = ZobMatrix4x4::EulerToQuaternion(dx, dy, dz);
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = v.w;
	Transform parentTransform = Transform::identity();
	if (m_parent)
	{
		parentTransform = m_parent->GetPhysicComponent()->GetWorldTransform();
	}
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetLocalTransform();
	newTransform.setOrientation(q);
	//newTransform = parentTransform.getInverse() * newTransform;
	m_physicComponent->SetLocalTransform(newTransform);
}

void ZobObject::SetLocalPosition(float x, float y, float z)
{
	Vector3 position = Vector3(x, y, z);
	Transform t = GetPhysicComponent()->GetLocalTransform();
	t.setPosition(position);
	m_physicComponent->SetLocalTransform(t);
}

void ZobObject::SetWorldPosition(float x, float y, float z)
{
	/*
	Transform parentTransform = Transform::identity();
	if (m_parent)
	{
		parentTransform = m_parent->GetPhysicComponent()->GetWorldTransform();
	}
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetWorldTransform();
	newTransform.setPosition(position);
	newTransform = parentTransform.getInverse() * newTransform;
	m_physicComponent->SetLocalTransform(newTransform);
	*/
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetWorldTransform();
	newTransform.setPosition(position);
	m_physicComponent->SetWorldTransform(newTransform);
}

ZobVector3 ZobObject::GetLocalRotation() const
{
	Quaternion q= m_physicComponent->GetLocalTransform().getOrientation();
	ZobVector3 v = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	v.x = RAD_TO_DEG(v.x);
	v.y = RAD_TO_DEG(v.y);
	v.z = RAD_TO_DEG(v.z);
	return v;
}

ZobVector3 ZobObject::GetLocalPosition() const
{
	Vector3 p = m_physicComponent->GetLocalTransform().getPosition();
	ZobVector3 v = ZobVector3(p.x, p.y, p.z);
	return v;
}

ZobVector3 ZobObject::GetWorldRotation() const
{
	Quaternion q = m_physicComponent->GetWorldTransform().getOrientation();
	ZobVector3 v = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	v.x = RAD_TO_DEG(v.x);
	v.y = RAD_TO_DEG(v.y);
	v.z = RAD_TO_DEG(v.z);
	return v;
}

ZobVector3 ZobObject::GetWorldPosition() const
{
	Vector3 p = m_physicComponent->GetWorldTransform().getPosition();
	ZobVector3 v = ZobVector3(p.x, p.y, p.z);
	return v;
}

void ZobObject::RegenerateZobIds()
{
	ZobGuidRegenerate();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->ZobGuidRegenerate();
	}
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RegenerateZobIds();
	}
}

void ZobObject::SaveTransform()
{
	m_physicComponent->SaveTransform();
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->SaveTransform();
	}
}

void ZobObject::RestoreTransform()
{
	m_physicComponent->RestoreTransform();
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RestoreTransform();
	}
}

void ZobObject::ResetPhysic()
{
	m_physicComponent->ResetPhysic();
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->ResetPhysic();
	}
}
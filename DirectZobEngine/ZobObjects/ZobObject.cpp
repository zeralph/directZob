#include "ZobObject.h"
#include "../DirectZob.h"
#include "../Mesh.h"
#include "../Sprite.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../SceneLoader.h"
#include "../Components/GraphicComponents/ZobComponentMesh.h"
#include "../Components/GraphicComponents/ZobComponentSprite.h"
#include "../Components/GraphicComponents/ZobComponentSkybox.h"
#include "../Misc/ZobXmlHelper.h"
#include "../../dependencies/optick/include/optick.h"

static int sObjectNumber = 0;
ZobObject::ZobObject(ZobType t, ZobSubType s, const std::string& name, ZobObject* parent /*= NULL*/)
	:ZobEntity(t,s)
{
	SET_CLASS_AND_NAME
	DirectZob::LogInfo("ZobObject %s creation", name.c_str()); 
	DirectZob::AddIndent();
	sObjectNumber++;
	m_Components.clear();
	m_visible = true;
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

ZobObject::ZobObject(zobId id, TiXmlElement* node, ZobObject* parent)
	:ZobEntity(id)
{
	SET_CLASS_AND_NAME
	m_varExposer = new ZobVariablesExposer(GetIdValue());
	m_Components.clear();
	sObjectNumber++;
	float x, y, z;
	TiXmlElement* f;
	DirectZob::LogInfo("ZobObject %i creation", id);
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
	m_visible = true;
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
	//Component
	TiXmlElement* ComponentsNode = node->FirstChildElement("Components");
	if(ComponentsNode)
	{
		for (TiXmlElement* e = ComponentsNode->FirstChildElement("Component"); e != NULL; e = e->NextSiblingElement("Component"))
		{
			ZobComponent* b = ZobComponentFactory::CreateComponent(this, e);
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
	m_varExposer->StartGroup("Local");
	m_varExposer->WrapVariable<ZobVector3>("Position", GetPhysicComponentNoConst()->GetLocalPositionAddress(), &ZobObject::ReloadVariablesFromLocalData, false, true);
	m_varExposer->WrapVariable<ZobVector3>("Rotation", GetPhysicComponentNoConst()->GetLocalRotationAddress(), &ZobObject::ReloadVariablesFromLocalData, false, false);
	m_varExposer->WrapVariable<ZobVector3>("Scale", GetPhysicComponentNoConst()->GetLocalScaleAddress(), &ZobObject::ReloadVariablesFromLocalData, false, true);
	m_varExposer->WrapVariable<ZobVector3>("Quaternion", GetPhysicComponentNoConst()->GetQuaternionAddress(), &ZobObject::ReloadVariablesFromLocalData, true, true, false);
	m_varExposer->EndGroup();
	//
	m_varExposer->StartGroup("World");
	m_varExposer->WrapVariable<ZobVector3>("WPosition", GetPhysicComponentNoConst()->GetWorldPositionAddress(), &ZobObject::ReloadVariablesFromWorldData, false, false);
	m_varExposer->WrapVariable<ZobVector3>("WRotation", GetPhysicComponentNoConst()->GetWorldRotationAddress(), &ZobObject::ReloadVariablesFromWorldData, false, false);
	m_varExposer->WrapVariable<ZobVector3>("WScale", GetPhysicComponentNoConst()->GetWorldScaleAddress(), &ZobObject::ReloadVariablesFromWorldData, false, false);
	m_varExposer->EndGroup();
	//
	m_varExposer->StartGroup("Physic");
	ZobPhysicComponent::eRigidBodyType bt[3] = { ZobPhysicComponent::eRigidBody_static, ZobPhysicComponent::eRigidBody_kinematic, ZobPhysicComponent::eRigidBody_dynamic };
	const char* btStr[3] = { "Static", "Kinematic", "Dynamic" };
	m_varExposer->WrapEnum<ZobPhysicComponent::eRigidBodyType>("Body type", GetPhysicComponentNoConst()->GetBodyTypeAddress(), 3, bt, btStr, &ZobPhysicComponent::UpdateProperties, false, false, true);
	m_varExposer->WrapVariable<bool>("Active", GetPhysicComponentNoConst()->GetActiveAddress(), &ZobPhysicComponent::UpdateProperties, false, true);
	m_varExposer->WrapVariable<float>("Mass", GetPhysicComponentNoConst()->GetMassAddress(), &ZobPhysicComponent::UpdateProperties, false, true);
	m_varExposer->EndGroup();
}

void ZobObject::ReloadVariablesFromWorldData(zobId id)
{
	ZobObject* z = ZobEntity::GetEntity<ZobObject>(id);
	if (z)
	{
		ZobVector3* zp = z->GetPhysicComponentNoConst()->GetWorldPositionAddress();
		ZobVector3* zr = z->GetPhysicComponentNoConst()->GetWorldRotationAddress();
		ZobVector3* zs = z->GetPhysicComponentNoConst()->GetWorldScaleAddress();
		Transform t;
		Vector3 vp = Vector3(zp->x, zp->y, zp->z);
		t.setPosition(vp);
		ZobVector3 qv = ZobMatrix4x4::EulerToQuaternion(DEG_TO_RAD(zr->x), DEG_TO_RAD(zr->y), DEG_TO_RAD(zr->z));
		Quaternion q = Quaternion(qv.x, qv.y, qv.z, qv.w);
		q.normalize();
		t.setOrientation(q);
		z->GetPhysicComponentNoConst()->SetWorldTransform(t);
		z->GetPhysicComponentNoConst()->SetWorldScale(zs->x, zs->y, zs->z);
	}
}

void ZobObject::ReloadVariablesFromLocalData(zobId id)
{
	ZobObject* z = ZobEntity::GetEntity<ZobObject>(id);
	{
		ZobVector3* zp = z->GetPhysicComponentNoConst()->GetLocalPositionAddress();
		ZobVector3* zr = z->GetPhysicComponentNoConst()->GetLocalRotationAddress();
		ZobVector3* zs = z->GetPhysicComponentNoConst()->GetLocalScaleAddress();
		Transform t = z->GetPhysicComponentNoConst()->GetLocalTransform();
		Vector3 vp = Vector3(zp->x, zp->y, zp->z);
		t.setPosition(vp);
		ZobVector3 qv = ZobMatrix4x4::EulerToQuaternion(DEG_TO_RAD(zr->x), DEG_TO_RAD(zr->y), DEG_TO_RAD(zr->z));
		Quaternion q = Quaternion(qv.x, qv.y, qv.z, qv.w);
		q.normalize();
		t.setOrientation(q);
		z->GetPhysicComponentNoConst()->SetLocalScale(zs->x, zs->y, zs->z);
		z->GetPhysicComponentNoConst()->SetLocalTransform(t);
	}
}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("delete ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();

	DirectZob::GetInstance()->GetZobObjectManager()->AddIdToDeleted(GetIdValue());
	for (int i = 0; i < m_Components.size(); i++)
	{
		delete m_Components[i];
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
	m_children.resize(0);
	DirectZob::RemoveIndent();
}

ZobComponentMesh* ZobObject::LoadMesh(ZobFilePath &zfp, bool bEditorZobComponent)
{
	ZobComponent* b = ZobComponentFactory::CreateComponent(this, "Mesh", bEditorZobComponent);
	ZobComponentMesh* bm = static_cast<ZobComponentMesh*>(b);
	bm->Set(zfp);
	return bm;
}

ZobComponentSprite* ZobObject::LoadSprite(ZobFilePath& zfp, bool bEditorZobComponent)
{
	ZobComponent* b = ZobComponentFactory::CreateComponent(this, "Sprite", bEditorZobComponent);
	ZobComponentSprite* bm = static_cast<ZobComponentSprite*>(b);
	bm->Set(zfp);
	return bm;
}

void ZobObject::Init(DirectZobType::sceneLoadingCallback cb)
{
	if (cb)
	{
		cb(0, 0, m_name);
	}
	if (m_name == "BodyTypeBall")
	{
		int uu = 0; 
		uu++;
	}
	m_varExposer->Load(true);
	m_physicComponent->Init();
	for (int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Init(cb);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Init(cb);
	}
}

void ZobObject::EditorUpdate()
{
	m_physicComponent->EditorUpdate();
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->IsEnabled())
			m_Components[i]->EditorUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if (z)
		{
			z->EditorUpdate();
		}
	}
}

void ZobObject::PreUpdate(float dt)
{
	OPTICK_EVENT();
	for (int i = 0; i < m_Components.size(); i++)
	{
		if(m_Components[i]->IsEnabled())
			m_Components[i]->PreUpdate(dt);

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
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->IsEnabled())
			m_Components[i]->PostUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if(z)
			z->PostUpdate();
	}
}

void ZobObject::SetVisible(bool v)
{
	//if (m_visible != v)
	{
		m_visible = v;
		std::vector<ZobComponentMesh*> m = GetComponents<ZobComponentMesh>();
		for (int i = 0; i < m.size(); i++)
		{
			m[i]->SetVisible(v);
		}
		for (int i = 0; i < m_children.size(); i++)
		{
			ZobObject* z = m_children[i];
			if (z)
				z->SetVisible(v);
		}
	}

}

void ZobObject::QueueForDrawing(const Camera* camera, Engine* engine)
{
	OPTICK_EVENT();
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->IsEnabled())
			m_Components[i]->QueueForDrawing(camera, engine);
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
	//ZobVector3 parentScale;
	if (m_parent)
	{
		const ZobPhysicComponent* zpc = m_parent->GetPhysicComponent();
		parentTransform = zpc->GetWorldTransform();
		//parentScale = zpc->GetWorldScale();
	}
	else
	{
		parentTransform = Transform::identity();
		//parentScale = ZobVector3(1, 1, 1);
	}
	//ZobVector3 scale = parentScale * m_physicComponent->GetLocalScale();
	//m_physicComponent->SetWorldScale(scale.x, scale.y, scale.z);
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

ZobVector3 ZobObject::GetLocalScale() const 
{ 
	return m_physicComponent->GetLocalScale();
}

void ZobObject::SetWorldScale(float x, float y, float z)
{
	m_physicComponent->SetWorldScale(x, y, z);
}

void ZobObject::SetLocalScale(float x, float y, float z)
{ 
	m_physicComponent->SetLocalScale(x, y, z);
}

void ZobObject::DrawGizmos(const Camera* camera, Engine* engine)
{
	if (m_markedForDeletion)
	{
		return;
	}
	uint c;
	ZobVector3 x = m_left;
	ZobVector3 y = m_up;
	ZobVector3 z = m_forward;
	ZobVector3 t = GetWorldPosition();
	x = x + t;
	y = y + t;
	z = z + t;
	engine->QueueLine(camera, &t, &x, &ZobColor::Red, true, false);
	engine->QueueLine(camera, &t, &y, &ZobColor::Green, true, false);
	engine->QueueLine(camera, &t, &z, &ZobColor::Blue, true, false);
	ZobVector3 p = GetWorldPosition();
	ZobVector3 r = GetWorldRotation();
	m_physicComponent->DrawGizmos(camera, &p, &r);
	for (int i = 0; i < m_Components.size(); i++)
	{
		if (m_Components[i]->IsEnabled())
			m_Components[i]->DrawGizmos(camera, &p, &r);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->DrawGizmos(camera, engine);
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
		if (!p->IsEditorObject())
		{
			DirectZob::LogInfo("Reparented %s from %s to %s", this->GetName().c_str(), m_parent->GetName().c_str(), p->GetName().c_str());
			//DirectZob::LogInfo("Reparented %s from %s to %s", ZobIdToString().c_str(), m_parent->ZobIdToString().c_str(), p->ZobIdToString().c_str());
		}
		ZobVector3 pos = GetWorldPosition();
		ZobVector3 rot = GetWorldRotation();
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
	delete decl;
}

void ZobObject::SaveRecusrive(TiXmlNode* node, ZobObject* z)
{
	if (ZobEntity::GetType(z->GetIdValue()) == ZobEntity::type_editor)
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
	std::string guid = ZobIdToString();
	objectNode->SetAttribute(XML_ATTR_GUID, guid.c_str());
	m_varExposer->SaveUnderNode(objectNode);
	TiXmlElement Components = TiXmlElement(XML_ELEMENT_ComponentS);
	for (int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->SaveUnderNode(&Components);
	}
	objectNode->InsertEndChild(Components);
	return (TiXmlNode*)objectNode;
}

void ZobObject::CreateSprite()
{
	assert(0);
	//Sprite* s = DirectZob::GetInstance()->GetMeshManager()->CreateSprite();
	//m_mesh = s;
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
	m_physicComponent->SetWorldOrientation(x, y, z);
}

void ZobObject::SetLocalOrientation(const ZobVector3* axis, float angle, bool add)
{
	Vector3 a = Vector3(axis->x, axis->y, axis->z);
	Quaternion q = ZobPhysicComponent::QuaternionFromAxisAngle(&a, angle);
	m_physicComponent->SetLocalOrientation(q, add);
}

void ZobObject::GetLocalAxisAngleRotation(ZobVector3& axis, float& angle)
{
	m_physicComponent->GetLocalAxisAngleRotation(axis, angle);
}

void ZobObject::SetLocalRotation(float x, float y, float z, bool add)
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
	q.normalize();
	Transform parentTransform = Transform::identity();
	Vector3 position = Vector3(x, y, z);
	Transform newTransform = GetPhysicComponent()->GetLocalTransform();
	if (add)
	{
		Quaternion curO = GetPhysicComponent()->GetLocalTransform().getOrientation();
		q = curO * q;
	}
	if (!q.isValid())
	{
		int t = 0;
		t++;
	}
	newTransform.setOrientation(q);
	//newTransform = parentTransform.getInverse() * newTransform;
	m_physicComponent->SetLocalTransform(newTransform);
}

void ZobObject::SetLocalFromOpenglMatrix(double* mat)
{
	m_physicComponent->SetLocalFromOpenglMatrix(mat);
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
	q.normalize();
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
	q.normalize();
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

ZobVector3 ZobObject::GetWorldScale() const
{
	Vector3 p = m_physicComponent->GetWorldScale();
	ZobVector3 v = ZobVector3(p.x, p.y, p.z);
	return v;
}

void ZobObject::RegenerateZobIds()
{
	/*
	ZobEntityRegenerate();
	for (int i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->ZobEntityRegenerate();
	}
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RegenerateZobIds();
	}
	*/
	assert(0);
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
	//this->Init(NULL);
	for (std::vector<ZobObject*>::const_iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->ResetPhysic();
	}
}

ZobObject* ZobObject::Duplicate()
{
	TiXmlElement n = TiXmlElement("tmp_root");
	
	TiXmlDocument doc(XML_ELEMENT_SCENE);
	//TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	//doc.LinkEndChild(decl);
	SaveRecusrive(&doc, this);
	doc.SaveFile("D:\\text.xml");	
	ZobObject* z = SceneLoader::LoadAsset(this->GetParent(), (TiXmlElement*)doc.FirstChild());
	z->SetName(z->GetName() + "_duplicate");
	return z;
}

void ZobObject::SaveAsAsset(std::string& file)
{
	TiXmlDocument doc(XML_ELEMENT_SCENE);
	SaveRecusrive(&doc, this);
	doc.SaveFile(file.c_str());
}
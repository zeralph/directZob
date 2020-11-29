#include "ZobObject.h"
#include "DirectZob.h"
#include "Mesh.h"
#include "Sprite.h"
#include "ZobPhysicComponent.h"
#include "SceneLoader.h"

static int sObjectNumber = 0;
ZobObject::ZobObject(Type t, SubType s, const std::string& name, ZobObject* parent /*= NULL*/, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(t,s)
{
	DirectZob::LogInfo("ZobObject %s creation", name.c_str());
	DirectZob::AddIndent();
	sObjectNumber++;
	m_factoryFile = factoryFile?factoryFile->c_str():"";
	m_physicComponent = new ZobPhysicComponent(NULL);
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
	m_children.clear();
	SetParent(m_parent);
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	ZobVector3 p = ZobVector3::Vector3Zero;
	m_physicComponent->Init(&p, &p);
	Update();
	DirectZob::RemoveIndent();
}

ZobObject::ZobObject(DirectZobType::guid id, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(id)
{
	sObjectNumber++;
	m_factoryFile = factoryFile ? factoryFile->c_str() : "";
	ZobVector3 position, rotation, scale, orientation = ZobVector3();
	std::string name;
	float x, y, z;
	TiXmlElement* f;
	name = node->Attribute("name");
	DirectZob::LogInfo("ZobObejct %s creation", name.c_str());
	DirectZob::AddIndent();
	f = node->FirstChildElement("Physic");
	m_physicComponent = new ZobPhysicComponent(f);
	f = node->FirstChildElement("Position");
	x = atof(f->Attribute("x"));
	y = atof(f->Attribute("y"));
	z = atof(f->Attribute("z"));
	position = ZobVector3(x, y, z);
	f = node->FirstChildElement("Rotation");
	if (f)
	{
		x = atof(f->Attribute("x"));
		y = atof(f->Attribute("y"));
		z = atof(f->Attribute("z"));
		rotation = ZobVector3(x, y, z);
	}
	f = node->FirstChildElement("Scale");
	if (f)
	{
		x = atof(f->Attribute("x"));
		y = atof(f->Attribute("y"));
		z = atof(f->Attribute("z"));
		scale = ZobVector3(x, y, z);
	}
	m_name = name;
	DirectZob::LogInfo("Adding new ZobObject %s", m_name.c_str());
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
	m_children.clear();
	SetParent(m_parent);
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	m_physicComponent->Init(&position, &rotation);
	SetScale(scale.x, scale.y, scale.z);
	Update();
	DirectZob::RemoveIndent();
}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("delete ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();

	DirectZob::GetInstance()->GetZobObjectManager()->AddIdToDeleted(GetId());

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
	m_children.clear();
	DirectZob::RemoveIndent();
}

void ZobObject::LoadMesh(std::string name, std::string path/* = ""*/)
{
	std::string file;
	int i = path.rfind('\\');
	if (i == -1)	//no path, only filename
	{
		file = path;
		path = std::string(SceneLoader::GetResourcePath());
	}
	else
	{
		file = path.substr(i+1, path.size() - i);
		path = path.substr(0, i + 1);
	}
	//std::string p = std::string(SceneLoader::GetResourcePath());
	Mesh* m = DirectZob::GetInstance()->GetMeshManager()->LoadMesh(name, path, file);
	m_mesh = m;
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

const std::string ZobObject::GetMeshFile() const
{
	if (m_mesh)
	{
		return m_mesh->GetFile();
	}
	return "";
}

void ZobObject::UpdateMesh(const Camera* camera, Core::Engine* engine)
{
	if (m_mesh)
	{
		m_mesh->Update(&m_modelMatrix, &m_rotationScaleMatrix, camera, engine, GetId(), &m_renderOptions);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->UpdateMesh(camera, engine);
	}
}

void ZobObject::PreUpdate()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		if (z->IsMarkedForDeletion())
		{
			RemoveChildReference(z);
			delete z;
		}
		else
		{
			z->PreUpdate();
		}
	}
}

void ZobObject::UpdateBehavior()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->UpdateBehavior();
	}
}

//void ZobObject::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
void ZobObject::Update()
{
	Transform parentTransform;
	ZobVector3 parentScale;
	if (m_parent)
	{
		const ZobPhysicComponent* zpc = m_parent->GetPhysicComponent();
		parentTransform = zpc->GetWorldTransform();
		parentScale = zpc->GetTotalScale();
	}
	else
	{
		parentTransform = Transform::identity();
		parentScale = ZobVector3(1, 1, 1);
	}
	ZobVector3 scale = parentScale * m_physicComponent->GetScale();
	Transform newTransform = m_physicComponent->GetLocalTransform();
	Vector3 p = parentTransform.getPosition();
	p.x *= scale.x;
	p.y *= scale.y;
	p.z *= scale.z;
	parentTransform.setPosition(p);
	newTransform = parentTransform * newTransform;
	m_physicComponent->SetWorldTransform(newTransform);
	m_physicComponent->Update();
	m_physicComponent->SetTotalScale(scale.x, scale.y, scale.z);
	const ZobMatrix4x4* parentMatrix = m_parent?m_parent->GetModelMatrix():&ZobMatrix4x4::IdentityMatrix;
	const ZobMatrix4x4* parentRSMatrix = m_parent?m_parent->GetRotationScaleMatrix():&ZobMatrix4x4::IdentityMatrix;
	ZobVector3 t = GetWorldPosition();
	ZobVector3 wpos = GetWorldRotation();	
	m_modelMatrix.Identity();
	m_rotationScaleMatrix.Identity();
	m_rotationScaleMatrix.SetRotation(GetWorldRotation());
	m_rotationScaleMatrix.SetScale(scale);
	m_modelMatrix.SetPosition(&t);
	m_modelMatrix.SetRotation(&wpos);
	m_modelMatrix.SetScale(scale);
	m_left = ZobVector3(1, 0, 0);
	m_forward = ZobVector3(0, 0, 1);
	m_up = ZobVector3(0, 1, 0);
	m_rotationScaleMatrix.Mul(&m_left);
	m_rotationScaleMatrix.Mul(&m_forward);
	m_rotationScaleMatrix.Mul(&m_up);
	m_left.Normalize();
	m_forward.Normalize();
	m_up.Normalize();
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Update();
	}
}

void ZobObject::QueueForDrawing(const Camera* camera, Core::Engine* engine)
{
	if(GetType() == ZOBGUID::type_editor)
	{
//		return;
	}
	if (m_mesh)
	{
		m_mesh->QueueForDrawing(this, m_modelMatrix, m_rotationScaleMatrix, camera, engine, GetId(), &m_renderOptions);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->QueueForDrawing(camera, engine);
	}
	if(engine->DrawGizmos())
	{
		DrawGizmos(camera, engine);
	}
}

ZobVector3 ZobObject::GetScale() const 
{ 
	return m_physicComponent->GetScale(); 
}

void ZobObject::SetScale(float x, float y, float z)
{ 
	m_physicComponent->SetScale(x, y, z);
}

void ZobObject::DrawGizmos(const Camera* camera, Core::Engine* engine)
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

void ZobObject::RemoveChildReference(const ZobObject* z)
{
	int i = GetChildPosition(z);
	if (i >= 0)
	{
		std::swap(m_children.at(i), m_children.at(m_children.size() - 1));
		m_children.pop_back();
	}
}

void ZobObject::AddChildReference(ZobObject* z)
{
	m_children.push_back(z);
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
	ZobObject* parent = GetParent();
	bool bOk = false;
	if (parent != NULL && p != NULL && p!= this && p != parent)
	{
		if (!HasChild(p))
		{
			parent->RemoveChildReference(this);
			p->AddChildReference(this);
			m_parent = p;
			bOk = true;
		}
		else
		{
			DirectZob::LogWarning("trying to reparent an object with one of its descendants !");
		}
	}
	else
	{
		if (p)
		{
			m_parent = p;
			bOk = true;
		}
	}
	if (bOk)
	{
		ZobVector3 p = GetWorldPosition();
		ZobVector3 r = GetWorldRotation();
		SetWorldPosition(p.x, p.y, p.z);
		SetWorldRotation(r.x, r.y, r.z);
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

void ZobObject::SetLightingMode(RenderOptions::eLightMode l)
{
	m_renderOptions.lightMode = l;
}

void ZobObject::SaveToFactoryFile(std::string& file)
{
	TiXmlDocument doc("ZobObject");
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
	TiXmlElement* o = new TiXmlElement("ZobObject");
	TiXmlElement p = TiXmlElement("Position");
	TiXmlElement r = TiXmlElement("Rotation");
	TiXmlElement s = TiXmlElement("Scale");
	o->SetAttribute("name", GetName().c_str());
	o->SetAttribute("guid", GetId());
	std::string meshName = GetMeshName();
	std::string meshFile = GetMeshFile();
	p.SetDoubleAttribute("x", GetLocalPosition().x);
	p.SetDoubleAttribute("y", GetLocalPosition().y);
	p.SetDoubleAttribute("z", GetLocalPosition().z);
	r.SetDoubleAttribute("x", GetLocalRotation().x);
	r.SetDoubleAttribute("y", GetLocalRotation().y);
	r.SetDoubleAttribute("z", GetLocalRotation().z);
	s.SetDoubleAttribute("x", GetScale().x);
	s.SetDoubleAttribute("y", GetScale().y);
	s.SetDoubleAttribute("z", GetScale().z);
	o->InsertEndChild(p);
	o->InsertEndChild(r);
	o->InsertEndChild(s);
	if (meshName.length() > 0 && meshFile.length() > 0)
	{
		TiXmlElement m = TiXmlElement("Mesh");
		m.SetAttribute("name", meshName.c_str());
		m.SetAttribute("file", meshFile.c_str());
		o->InsertEndChild(m);
		o->SetAttribute("type", "mesh");
	}
	m_physicComponent->SaveUnderNode(o);
	return (TiXmlNode*)o;
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
		m_physicComponent->SetLocalOrientation(q);
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

void ZobObject::SetWorldPosition(float x, float y, float z)
{
	//m_physicComponent->SetPosition(x, y, z);
	Transform parentTransform = Transform::identity();
	if (m_parent)
	{
		parentTransform = m_parent->GetPhysicComponent()->GetWorldTransform();
	}
	Vector3 position = Vector3(x, y, z);
//	
	Transform newTransform = GetPhysicComponent()->GetWorldTransform();
	newTransform.setPosition(position);
	newTransform = parentTransform.getInverse() * newTransform;
	m_physicComponent->SetLocalTransform(newTransform);
//
//	position = position - t.getPosition();
//	m_physicComponent->SetLocalPosition(position);
}

inline ZobVector3 ZobObject::GetLocalRotation() const
{
	Quaternion q= m_physicComponent->GetLocalTransform().getOrientation();
	ZobVector3 v = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	v.x = RAD_TO_DEG(v.x);
	v.y = RAD_TO_DEG(v.y);
	v.z = RAD_TO_DEG(v.z);
	return v;
}

inline ZobVector3 ZobObject::GetLocalPosition() const
{
	Vector3 p = m_physicComponent->GetLocalTransform().getPosition();
	ZobVector3 v = ZobVector3(p.x, p.y, p.z);
	return v;
}

inline ZobVector3 ZobObject::GetWorldRotation() const
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

void ZobObject::SetPhysicComponent(int i)
{
	m_physicComponent->Set((ZobPhysicComponent::ePhysicComponentType)i);
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
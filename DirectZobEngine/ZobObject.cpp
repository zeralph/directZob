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
	m_scale = ZobVector3(1, 1, 1);
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
	DirectZob::RemoveIndent();
}

ZobObject::ZobObject(Type t, SubType s, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(t, s)
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
	m_scale = ZobVector3(1, 1, 1);
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
	SetParent(m_parent);
	DirectZob::RemoveIndent();
}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("delete ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();
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

//void ZobObject::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
void ZobObject::Update()
{
	Transform tr;
	if (m_parent)
	{
		const ZobPhysicComponent* zpc = m_parent->GetPhysicComponent();
		tr = zpc->GetWorldTransform();
	}
	else
	{
		tr = Transform::identity();

	}
	Transform newTransform = tr * m_physicComponent->GetLocalTransform();
	//m_physicComponent->

	Vector3 pt = tr.getPosition();
	Quaternion pr = tr.getOrientation();
	m_physicComponent->SetWorldTransform(newTransform);

	m_physicComponent->Update();

	const ZobMatrix4x4* parentMatrix = m_parent?m_parent->GetModelMatrix():&ZobMatrix4x4::IdentityMatrix;
	const ZobMatrix4x4* parentRSMatrix = m_parent?m_parent->GetRotationScaleMatrix():&ZobMatrix4x4::IdentityMatrix;

	ZobVector3 t = GetPosition();
	ZobVector3 r = GetRotation();
/*
	pt = newTransform.getPosition();
	//Vector3 pt = newTransform.getPosition();

	Quaternion q = newTransform.getOrientation();
	q.normalize();
	ZobVector3 z = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	float ax = RAD_TO_DEG(z.x);
	float ay = RAD_TO_DEG(z.y);
	float az = RAD_TO_DEG(z.z);
	//ClampAngle(ax);
	//ClampAngle(ay);
	//ClampAngle(az);
	r = ZobVector3(ax, ay, az);

	t.x = pt.x;
	t.y = pt.y;
	t.z = pt.z;
*/	
	ZobVector3 s = m_scale;
	m_modelMatrix.Identity();
	m_rotationScaleMatrix.Identity();
	m_rotationScaleMatrix.SetRotation(GetRotation());
	m_rotationScaleMatrix.SetScale(&m_scale);
	//m_rotationScaleMatrix.Mul(parentRSMatrix);
	//parentRSMatrix->Mul(&t);
	m_modelMatrix.SetPosition(&t);
	m_modelMatrix.SetRotation(&r);
	m_modelMatrix.SetScale(&s);
	//m_modelMatrix.Mul(&parentMatrix);

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
		if(z->IsMarkedForDeletion())
		{
			RemoveChildReference(z);
			delete z;
		}
		else
		{
			//z->Update(m_modelMatrix, m_rotationScaleMatrix);
			z->Update();
		}
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

void ZobObject::DrawGizmos(const Camera* camera, Core::Engine* engine)
{
	uint c;
	ZobVector3 x = m_left;
	ZobVector3 y = m_up;
	ZobVector3 z = m_forward;
	const ZobVector3* t = GetPosition();
	x = x + t;
	y = y + t;
	z = z + t;
	c = 0x00FF0000;
	engine->QueueLine(camera, t, &x, c, true);
	c = 0x0000FF00;
	engine->QueueLine(camera, t, &y, c, true);
	c = 0x000000FF;
	engine->QueueLine(camera, t, &z, c, true);
	ZobVector3 p = GetPosition();
	ZobVector3 r = GetRotation();
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
	if (parent != NULL && p != NULL && p!= this && p != parent)
	{
		if (!HasChild(p))
		{
			parent->RemoveChildReference(this);
			p->AddChildReference(this);
			m_parent = p;
		}
		else
		{
			DirectZob::LogWarning("trying to reparent an object with one of its descendants !");
		}
	}
	else
	{
		//m_parent.reset();
		if (p)
		{
			m_parent = p;
			//p->AddChildReference(this);
		}
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
	std::string meshName = GetMeshName();
	std::string meshFile = GetMeshFile();
	p.SetDoubleAttribute("x", GetPosition()->x);
	p.SetDoubleAttribute("y", GetPosition()->y);
	p.SetDoubleAttribute("z", GetPosition()->z);
	r.SetDoubleAttribute("x", GetRotation()->x);
	r.SetDoubleAttribute("y", GetRotation()->y);
	r.SetDoubleAttribute("z", GetRotation()->z);
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
	return o;
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
void ZobObject::LookAt(const ZobVector3* target)
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
	LookAt(&forward, &left, &up);
}

void ZobObject::LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up)
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
		m_physicComponent->SetLocalOrientation(q);
	}
}

void ZobObject::SetRotation(float x, float y, float z)
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

void ZobObject::SetPosition(float x, float y, float z)
{
	//m_physicComponent->SetPosition(x, y, z);
	Transform t = Transform::identity();
	if (m_parent)
	{
		t = m_parent->GetPhysicComponent()->GetWorldTransform();
	}
	//Transform t = Transform::identity();
	Vector3 position = Vector3(x, y, z);
	position = position - t.getPosition();
	m_physicComponent->SetLocalPosition(position);
}

const ZobVector3* ZobObject::GetRotation() const
{
	return m_physicComponent->GetOrientation();
}

const ZobVector3* ZobObject::GetPosition() const
{
	return m_physicComponent->GetPosition();
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
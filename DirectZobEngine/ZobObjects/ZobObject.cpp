#include "ZobObject.h"
#include "DirectZob.h"
#include "Mesh.h"
#include "Sprite.h"
#include "ZobPhysic/ZobPhysicComponent.h"
#include "SceneLoader.h"
#include "../../dependencies/optick/include/optick.h"

static int sObjectNumber = 0;
ZobObject::ZobObject(ZobType t, ZobSubType s, const std::string& name, ZobObject* parent /*= NULL*/, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(t,s)
{
	DirectZob::LogInfo("ZobObject %s creation", name.c_str());
	DirectZob::AddIndent();
	sObjectNumber++;
	m_behaviors.clear();
	m_factoryFile = factoryFile?factoryFile->c_str():"";
	m_physicComponent = new ZobPhysicComponent(this, NULL);
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
	m_newParent = m_parent;
	SetParentInternal();
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	ZobVector3 p = ZobVector3::Vector3Zero;
	m_physicComponent->Init(&p, &p);
	Update(0);
	DirectZob::RemoveIndent();
}

ZobObject::ZobObject(DirectZobType::guid id, TiXmlElement* node, ZobObject* parent, const std::string* factoryFile /*=NULL*/)
	:ZOBGUID(id)
{
	m_behaviors.clear();
	sObjectNumber++;
	m_factoryFile = factoryFile ? factoryFile->c_str() : "";
	ZobVector3 position, rotation, scale, orientation = ZobVector3();
	std::string name;
	float x, y, z;
	TiXmlElement* f;
	name = node->Attribute("name");
	DirectZob::LogInfo("ZobObject %s creation", name.c_str());
	DirectZob::AddIndent();
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
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	//mesh loading section
	TiXmlElement* meshNode = node->FirstChildElement("Mesh");
	if (meshNode)
	{
		std::string meshName = meshNode->Attribute("name");
		std::string meshPath = meshNode->Attribute("path") ? meshNode->Attribute("path") : std::string("");
		std::string meshFile = meshNode->Attribute("file");
		if (meshName.length() > 0 && meshFile.length() > 0)
		{
			LoadMesh(meshName, meshFile, meshPath);
		}
		f = meshNode->FirstChildElement("RenderOptions");
		if (f)
		{
			TiXmlElement* n = f->FirstChildElement("Lighting");
			if (n)
			{
				std::string l = n->GetText();
				if (l == "None")
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_none;
				}
				else if (l == "Flat")
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_flat;
				}
				else if (l == "FlatPhong")
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_flatPhong;
				}
				else if (l == "Gouraud")
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_gouraud;
				}
				else if (l == "Phong")
				{
					m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
				}
			}
			n = f->FirstChildElement("ZBuffer");
			if (n)
			{
				std::string l = n->GetText();
				if (l == "True")
				{
					m_renderOptions.zBuffered = true;
				}
				else
				{
					m_renderOptions.zBuffered = false;
				}
			}
			n = f->FirstChildElement("Transparency");
			if (n)
			{
				std::string l = n->GetText();
				if (l == "True")
				{
					m_renderOptions.bTransparency = true;
				}
				else
				{
					m_renderOptions.bTransparency = false;
				}
			}
			n = f->FirstChildElement("CullMode");
			if (n)
			{
				std::string l = n->GetText();
				if (l == "Clockwise")
				{
					m_renderOptions.cullMode = eCullMode_ClockwiseFace;
				}
				else if (l == "CounterClockwise")
				{
					m_renderOptions.cullMode = eCullMode_CounterClockwiseFace;
				}
				else
				{
					m_renderOptions.cullMode = eCullMode_None;
				}
			}
		}
	}
	//Physics
	f = node->FirstChildElement("Physic");
	m_physicComponent = new ZobPhysicComponent(this, f);
	//parenting
	m_children.clear();
	m_newParent = m_parent;
	SetParentInternal();
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	//Behavior
	TiXmlElement* behaviorsNode = node->FirstChildElement("Behaviors");
	if(behaviorsNode)
	{
		for (TiXmlElement* e = behaviorsNode->FirstChildElement("Behavior"); e != NULL; e = e->NextSiblingElement("Behavior"))
		{
			ZobBehavior* b = ZobBehaviorFactory::CreateBehavior(this, e);
			m_behaviors.push_back(b);
		}
	}
	//Init
	m_physicComponent->Init(&position, &rotation);
	SetScale(scale.x, scale.y, scale.z);
	Update(0);
	DirectZob::RemoveIndent();
}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("delete ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();

	DirectZob::GetInstance()->GetZobObjectManager()->AddIdToDeleted(GetId());
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
	m_children.clear();
	DirectZob::RemoveIndent();
}

void ZobObject::LoadMesh(std::string name, std::string file, std::string path)
{
/*
	if (!path.length())	//no path, only filename
	{
		file = path;
		path = std::string(SceneLoader::GetResourcePath());
	}
	else
	{
		file = path.substr(i+1, path.size() - i);
		path = path.substr(0, i + 1);
	}
*/
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

void ZobObject::UpdateMesh(const Camera* camera, Core::Engine* engine)
{
	OPTICK_EVENT();
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

void ZobObject::Init()
{
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		//m_behaviors[i]->Init();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->Init();
	}
}

void ZobObject::PreUpdate()
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->PreUpdate();
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->PreUpdate();
	}
	SetParentInternal();
	DeleteInternal();
}

void ZobObject::UpdateBehavior(float dt)
{
	OPTICK_EVENT();
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->Update(dt);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		ZobObject* z = m_children[i];
		z->UpdateBehavior(dt);
	}
}

void ZobObject::SetParentInternal()
{
	if (m_newParent != NULL)
	{
		if (!HasChild(m_newParent))
		{
			DirectZob::LogInfo("Reparented %s \tfrom %s \tto %s", GetName().c_str(), m_parent->GetName().c_str(), m_newParent->GetName().c_str());
			ZobVector3 pos = GetWorldPosition();
			ZobVector3 rot = GetWorldRotation();
			ZobVector3 sca = GetScale();
			if (GetParent()->RemoveChildReference(this))
			{
				if (m_newParent->AddChildReference(this))
				{
					m_parent = m_newParent;
					//remove inherited scale factor
					//ZobVector3 ps = parent->GetScale();
					//s.x *= ps.x;
					//s.y *= ps.y;
					//s.z *= ps.z;
					SetWorldPosition(pos.x, pos.y, pos.z);
					SetWorldRotation(rot.x, rot.y, rot.z);
					//SetScale(sca.x, sca.y, sca.z);
				}
			}
		}
		else
		{
			DirectZob::LogWarning("Trying to reparent an object with one of its descendants !");
		}
		m_newParent = NULL;
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
		else
		{
			z->PreUpdate();
		}
	}
}

//void ZobObject::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
void ZobObject::Update(float dt)
{
	OPTICK_EVENT();
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
	//p.x *= scale.x;
	//p.y *= scale.y;
	//p.z *= scale.z;
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
		z->Update(dt);
	}
}

void ZobObject::QueueForDrawing(const Camera* camera, Core::Engine* engine)
{
	OPTICK_EVENT();
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
	if(engine->DrawGizmos() && engine->DrawZobObjectGizmos())
	{
		DrawGizmos(camera, engine);
	}
}

ZobVector3 ZobObject::GetScale() const 
{ 
	return m_physicComponent->GetTotalScale(); 
}

void ZobObject::SetScale(float x, float y, float z)
{ 
	ZobVector3 s = m_parent->GetScale();
	x /= s.x;
	y /= s.y;
	z /= s.z;
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
		std::swap(m_children.at(i), m_children.at(m_children.size() - 1));
		m_children.pop_back();
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
	m_newParent = p;
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
	TiXmlElement* objectNode = new TiXmlElement("ZobObject");
	TiXmlElement p = TiXmlElement("Position");
	TiXmlElement r = TiXmlElement("Rotation");
	TiXmlElement s = TiXmlElement("Scale");
	objectNode->SetAttribute("name", GetName().c_str());
	objectNode->SetAttribute("guid", GetId());
	std::string meshName = GetMeshName();
	std::string meshFileName = GetMeshFileName();
	std::string meshPath = GetMeshPath();
	p.SetDoubleAttribute("x", GetLocalPosition().x);
	p.SetDoubleAttribute("y", GetLocalPosition().y);
	p.SetDoubleAttribute("z", GetLocalPosition().z);
	r.SetDoubleAttribute("x", GetLocalRotation().x);
	r.SetDoubleAttribute("y", GetLocalRotation().y);
	r.SetDoubleAttribute("z", GetLocalRotation().z);
	s.SetDoubleAttribute("x", GetScale().x);
	s.SetDoubleAttribute("y", GetScale().y);
	s.SetDoubleAttribute("z", GetScale().z);
	objectNode->InsertEndChild(p);
	objectNode->InsertEndChild(r);
	objectNode->InsertEndChild(s);
	if (meshName.length() > 0 && meshFileName.length() > 0)
	{
		TiXmlElement meshNode = TiXmlElement("Mesh");
		meshNode.SetAttribute("name", meshName.c_str());
		meshNode.SetAttribute("file", meshFileName.c_str());
		meshNode.SetAttribute("path", meshPath.c_str());
		objectNode->SetAttribute("type", "mesh");
		TiXmlElement renderOptions = TiXmlElement("RenderOptions");
		TiXmlElement lighting = TiXmlElement("Lighting");
		TiXmlText t("None");
		if (m_renderOptions.lightMode == RenderOptions::eLightMode_flat)
		{
			t = "Flat";
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_flat)
		{
			t = "Flat";
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_flatPhong)
		{
			t = "FlatPhong";
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_gouraud)
		{
			t = "Gouraud";
		}
		else if (m_renderOptions.lightMode == RenderOptions::eLightMode_phong)
		{
			t = "Phong";
		}
		lighting.InsertEndChild(t);
		renderOptions.InsertEndChild(lighting);
		TiXmlElement zBuffer = TiXmlElement("ZBuffer");
		if (m_renderOptions.zBuffered)
		{
			t = "True";
		}
		else
		{
			t = "False";
		}
		zBuffer.InsertEndChild(t);
		renderOptions.InsertEndChild(zBuffer);
		TiXmlElement transparency = TiXmlElement("Transparency");
		if (m_renderOptions.bTransparency)
		{
			t = "True";
		}
		else
		{
			t = "False";
		}
		transparency.InsertEndChild(t);
		renderOptions.InsertEndChild(transparency);

		TiXmlElement cullMode = TiXmlElement("CullMode");
		t = "None";
		if (m_renderOptions.cullMode == eCullMode_ClockwiseFace)
		{
			t = "Clockwise";
		}
		else if (m_renderOptions.cullMode == eCullMode_CounterClockwiseFace)
		{
			t = "CounterClockwise";
		}
		cullMode.InsertEndChild(t);
		renderOptions.InsertEndChild(cullMode);
		meshNode.InsertEndChild(renderOptions);
		objectNode->InsertEndChild(meshNode);
	}
	TiXmlElement behaviors = TiXmlElement("Behaviors");
	for (int i = 0; i < m_behaviors.size(); i++)
	{
		m_behaviors[i]->SaveUnderNode(&behaviors);
	}
	objectNode->InsertEndChild(behaviors);
	m_physicComponent->SaveUnderNode(objectNode);
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

void ZobObject::SetPhysicComponent(int i)
{
	m_physicComponent->SetType((ZobPhysicComponent::ePhysicComponentType)i);
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

//- Physic editor interface
void ZobObject::GetPhysicComponentInfo(std::string& type, std::string& shapeType) const
{
	type = "Error";
	shapeType = "Error";
	ZobPhysicComponent::ePhysicComponentType t = GetPhysicComponent()->GetType();
	ZobPhysicComponent::eShapeType st = GetPhysicComponent()->GetShapeType();
	switch (t)
	{
	case ZobPhysicComponent::ePhysicComponentType_dynamic:
		type = std::string("Dynamic");
		break;
	case ZobPhysicComponent::ePhysicComponentType_static:
		type = std::string("Static");
		break;
	default:
		type = std::string("Error");
		break;
	}
	switch (st)
	{
	case ZobPhysicComponent::eShapeType_box:
		shapeType = std::string("Box");
		break;
	case ZobPhysicComponent::eShapeType_capsule:
		shapeType = std::string("Capsule");
		break;
	case ZobPhysicComponent::eShapeType_convexMesh:
		shapeType = std::string("Mesh");
		break;
	case ZobPhysicComponent::eShapeType_sphere:
		shapeType = std::string("Sphere");
		break;
	case ZobPhysicComponent::eShapeType_none:
		shapeType = std::string("None");
		break;
	default:
		shapeType = std::string("Error");
		break;
	}
}

void ZobObject::SetPhysicComponentInfo(std::string& type, std::string& shapeType)
{
	ZobPhysicComponent::ePhysicComponentType t;
	if (type == "Dynamic")
	{
		t = ZobPhysicComponent::ePhysicComponentType_dynamic;
	}
	else if (type == "Static")
	{
		t = ZobPhysicComponent::ePhysicComponentType_static;
	}
	else
	{
		t = ZobPhysicComponent::ePhysicComponentType_static;
	}
	GetPhysicComponentNoConst()->SetType(t);
	ZobPhysicComponent::eShapeType st;
	if (shapeType == "Box")
	{
		st = ZobPhysicComponent::eShapeType_box;
		GetPhysicComponentNoConst()->SetShapeType(st);
	}
	else if (shapeType == "Capsule")
	{
		st = ZobPhysicComponent::eShapeType_capsule;
		GetPhysicComponentNoConst()->SetShapeType(st);
	}
	else if (shapeType == "Mesh")
	{
		st = ZobPhysicComponent::eShapeType_convexMesh;
		GetPhysicComponentNoConst()->AddMeshCollider();
	}
	else if (shapeType == "Sphere")
	{
		st = ZobPhysicComponent::eShapeType_sphere;
		GetPhysicComponentNoConst()->SetShapeType(st);
	}
	else
	{
		//return error ?
		GetPhysicComponentNoConst()->SetShapeType(ZobPhysicComponent::eShapeType_none);
	}
}

void ZobObject::GetPhysicComponentShapeInfo(float& radius, float& height, float& hx, float& hy, float& hz, std::string& mesh)
{
	radius = m_physicComponent->GetRadius();
	height = m_physicComponent->GetHeight();
	ZobVector3 halfExtends = m_physicComponent->GetHalfExtends();
	hx = halfExtends.x;
	hy = halfExtends.y;
	hz = halfExtends.z;
	mesh = m_physicComponent->GetMesh();
}

void ZobObject::SetPhysicComponentShapeInfo(float radius, float height, float hx, float hy, float hz, std::string& mesh)
{
	m_physicComponent->SetRadius(radius);
	m_physicComponent->SetHeight(height);
	m_physicComponent->SetHalfextends(hx, hy, hz);
	//m_physicComponent->mesh
}

void ZobObject::GetPhysicComponentColliderInfo(float& bounciness, float& frictionCoeff, float& massDensity, float& RollingResistance)
{
	Collider* c = m_physicComponent->GetCollider();
	if (c)
	{
		reactphysics3d::Material& material = c->getMaterial();
		bounciness = material.getBounciness();
		frictionCoeff =  material.getFrictionCoefficient();
		massDensity = material.getMassDensity();
		RollingResistance = material.getRollingResistance();
	}
}

void ZobObject::SetPhysicComponentColliderInfo(float bounciness, float frictionCoeff, float massDensity, float RollingResistance)
{
	Collider* c = m_physicComponent->GetCollider();
	if (c)
	{
		reactphysics3d::Material& material = c->getMaterial();
		material.setBounciness(bounciness);
		material.setFrictionCoefficient(frictionCoeff);
		material.setMassDensity(massDensity);
		material.setRollingResistance(RollingResistance);
		c->setMaterial(material);
	}
}

void ZobObject::SetPhysicComponentScaleWithObject(bool b)
{
	m_physicComponent->SetScaleWithObject(b);
}

void ZobObject::GetPhysicComponentScaleWithObject(bool& b)
{
	b = m_physicComponent->GetScaleWithObject();
}
#include "ZobObject.h"
#include "DirectZob.h"
#include "Mesh.h"
static int sObjectNumber = 0;
ZobObject::ZobObject(Type t, SubType s, const std::string& name, Mesh* mesh, ZobObject* parent /*= NULL*/)
	:ZOBGUID(t,s)
{
	DirectZob::LogInfo("ZobObejct %s creation", name.c_str());
	DirectZob::AddIndent();
	sObjectNumber++;
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
	m_mesh = mesh;
	m_translation = Vector3(0, 0, 0);
	m_rotation = Vector3(0, 0, 0);
	m_scale = Vector3(1, 1, 1);
	m_children.clear();
	SetParent(m_parent);
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	DirectZob::RemoveIndent();
}

ZobObject::ZobObject(Type t, SubType s, TiXmlElement* node, Mesh* mesh, ZobObject* parent)
	:ZOBGUID(t, s)
{
	sObjectNumber++;
	Vector3 position, rotation, scale, orientation = Vector3();
	std::string name;
	float x, y, z;
	TiXmlElement* f;
	name = node->Attribute("name");
	DirectZob::LogInfo("ZobObejct %s creation", name.c_str());
	DirectZob::AddIndent();
	f = node->FirstChildElement("Position");
	x = atof(f->Attribute("x"));
	y = atof(f->Attribute("y"));
	z = atof(f->Attribute("z"));
	position = Vector3(x, y, z);
	f = node->FirstChildElement("Rotation");
	if (f)
	{
		x = atof(f->Attribute("x"));
		y = atof(f->Attribute("y"));
		z = atof(f->Attribute("z"));
		rotation = Vector3(x, y, z);
	}
	f = node->FirstChildElement("Scale");
	if (f)
	{
		x = atof(f->Attribute("x"));
		y = atof(f->Attribute("y"));
		z = atof(f->Attribute("z"));
		scale = Vector3(x, y, z);
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
	m_mesh = mesh;
	m_translation = Vector3(0, 0, 0);
	m_rotation = Vector3(0, 0, 0);
	m_scale = Vector3(1, 1, 1);
	m_children.clear();
	SetParent(m_parent);
	if (m_parent != NULL)
	{
		m_parent->AddChildReference(this);
	}
	m_renderOptions.lightMode = RenderOptions::eLightMode_phong;
	m_renderOptions.zBuffered = true;
	m_renderOptions.bTransparency = false;
	//m_renderOptions.colorization = new Vector3(1, 0, 0);
	SetTranslation(position.x, position.y, position.z);
	SetRotation(rotation.x, rotation.y, rotation.z);
	SetScale(scale.x, scale.y, scale.z);
	SetParent(m_parent);
	DirectZob::RemoveIndent();
}

ZobObject::~ZobObject()
{
	DirectZob::LogInfo("deleet ZobObject %s", m_name.c_str());
	DirectZob::AddIndent();
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

void ZobObject::Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix)
{
	Vector3 t = m_translation;
	Vector3 r = m_rotation;
	Vector3 s = m_scale;
	m_modelMatrix.Identity();
	m_rotationScaleMatrix.Identity();
	m_rotationScaleMatrix.SetRotation(&m_rotation);
	m_rotationScaleMatrix.SetScale(&m_scale);
	m_rotationScaleMatrix.Mul(&parentRSMatrix);
	parentRSMatrix.Mul(&t);
	m_modelMatrix.SetTranslation(&t);
	m_modelMatrix.SetRotation(&r);
	m_modelMatrix.SetScale(&s);
	m_modelMatrix.Mul(&parentMatrix);

	m_left = Vector3(1, 0, 0);
	m_forward = Vector3(0, 0, 1);
	m_up = Vector3(0, 1, 0);
	m_rotationScaleMatrix.Mul(&m_left);
	m_rotationScaleMatrix.Mul(&m_forward);
	m_rotationScaleMatrix.Mul(&m_up);

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
			z->Update(m_modelMatrix, m_rotationScaleMatrix);
		}
	}
}

void ZobObject::QueueForDrawing(const Camera* camera, Core::Engine* engine)
{
	if(GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	if (m_mesh)
	{
		m_mesh->QueueForDrawing(m_modelMatrix, m_rotationScaleMatrix, camera, engine, GetId(), &m_renderOptions);
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
	Vector3 x = m_left;
	Vector3 y = m_up;
	Vector3 z = m_forward;
	x = x + m_translation;
	y = y + m_translation;
	z = z + m_translation;
	c = 0x00FF0000;
	engine->QueueLine(camera, &m_translation, &x, c, true);
	c = 0x0000FF00;
	engine->QueueLine(camera, &m_translation, &y, c, true);
	c = 0x000000FF;
	engine->QueueLine(camera, &m_translation, &z, c, true);
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

TiXmlNode* ZobObject::SaveUnderNode(TiXmlNode* node)
{
	char tmpBuffer[256];
	TiXmlElement* o = new TiXmlElement("ZobObject");
	TiXmlElement p = TiXmlElement("Position");
	TiXmlElement r = TiXmlElement("Rotation");
	TiXmlElement s = TiXmlElement("Scale");
	o->SetAttribute("name", GetName().c_str());
	std::string meshName = GetMeshName();
	p.SetDoubleAttribute("x", GetTransform().x);
	p.SetDoubleAttribute("y", GetTransform().y);
	p.SetDoubleAttribute("z", GetTransform().z);
	r.SetDoubleAttribute("x", GetRotation().x);
	r.SetDoubleAttribute("y", GetRotation().y);
	r.SetDoubleAttribute("z", GetRotation().z);
	s.SetDoubleAttribute("x", GetScale().x);
	s.SetDoubleAttribute("y", GetScale().y);
	s.SetDoubleAttribute("z", GetScale().z);
	o->InsertEndChild(p);
	o->InsertEndChild(r);
	o->InsertEndChild(s);
	if (meshName.length() > 0)
	{
		TiXmlElement m = TiXmlElement("Mesh");
		TiXmlText t = TiXmlText(meshName.c_str());
		m.InsertEndChild(t);
		o->InsertEndChild(m);
		o->SetAttribute("type", "mesh");
	}
	return o;
}
#include "ZobObject.h"
#include "DirectZob.h"

ZobObject::ZobObject(Type t, SubType s, std::string& name, Mesh* mesh, ZobObject* parent /*= NULL*/):ZOBGUID(t,s)
{
	m_name = name;
	m_parent = parent;
	m_mesh = mesh;
	m_translation = Vector3(0, 0, 0);
	m_rotation = Vector3(0, 0, 0);
	m_scale = Vector3(1, 1, 1);
	if (parent != NULL)
	{
		parent->AddChildReference(this);
	}
	std::string l = "Added new ZobObject ";
	l.append(m_name);
	DirectZob::LogInfo(l.c_str());
}

ZobObject::~ZobObject()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		delete m_children.at(i);
	}
	if (m_parent != NULL)
	{
		m_parent->RemoveChildReference(this);
	}
	std::string l = "Deleted ZobObject ";
	l.append(m_name);
	DirectZob::LogInfo(l.c_str());
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


void ZobObject::Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRotationMatrix)
{
	m_modelMatrix.Identity();
	m_modelMatrix.SetScale(&m_scale);
	m_modelMatrix.SetRotation(&m_rotation);
	m_modelMatrix.SetTranslation(&m_translation);
	m_modelMatrix.Mul(&parentMatrix);
	m_rotationMatrix.Mul(&parentRotationMatrix);
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->Update(m_modelMatrix, m_rotationMatrix);
	}
}

void ZobObject::Draw(const Camera* camera, Core::Engine* engine)
{
	if (m_mesh)
	{
		m_mesh->Draw(m_modelMatrix, m_rotationMatrix, camera, engine, GetId(), m_renderOptions);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->Draw(camera, engine);
	}
}

int ZobObject::GetChildPosition(const ZobObject* z)
{
	for (int i=0; i<m_children.size(); i++)
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
		std::swap(m_children.at(i), m_children.at(m_children.size()-1));
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
	if (p != NULL && p!= this && p != parent)
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
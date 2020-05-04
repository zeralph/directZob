#include "ZobObject.h"
#include "DirectZob.h"

ZobObject::ZobObject(Type t, SubType s, std::string& name, Mesh* mesh, ZobObject* parent /*= NULL*/):ZOBGUID(t,s)
{
	DirectZob::LogInfo("Adding new ZobObject %s", name.c_str());
	if (!parent && name != "root")
	{
		m_parent = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
		//DirectZob::LogError("Cannot add %s because it has no parent", name.c_str());
	}
	else
	{
		m_parent = parent;
	}
	m_markedForDeletion = false;
	m_name = name;
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
	m_renderOptions.LightMode(RenderOptions::eLightMode_phong);
	m_renderOptions.ZBuffered(true);
	m_renderOptions.bTransparency = false;
	//m_renderOptions.colorization = new Vector3(1, 0, 0);
    DirectZob::LogInfo("ZobObject %s added", name.c_str());
}

ZobObject::~ZobObject()
{
	Light* l = (Light*)this;
	if (l)
	{
		DirectZob::GetInstance()->GetLightManager()->RemoveLight(l);
	}
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
	DirectZob::LogInfo("Deleted ZobObject %s", m_name.c_str());
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

void ZobObject::Draw(const Camera* camera, Core::Engine* engine)
{
	if (m_mesh)
	{
		m_mesh->Draw(m_modelMatrix, m_rotationScaleMatrix, camera, engine, GetId(), &m_renderOptions);
	}
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children.at(i)->Draw(camera, engine);
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
	m_renderOptions.LightMode(l);
}

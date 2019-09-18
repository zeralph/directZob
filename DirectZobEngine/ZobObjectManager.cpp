#include "ZobObjectManager.h"

static int sObjectNumber = 1;

ZobObjectManager::ZobObjectManager()
{
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL, NULL);
}

ZobObjectManager::~ZobObjectManager()
{

}

void ZobObjectManager::AddZobObject(ZobObject* z)
{
	if (!m_rootObject->GetChild(z->GetName()))
	{
		m_rootObject->AddChild(z);
	}
}

void ZobObjectManager::CreateZobObject(ZobObject* parent)
{
	std::string n = "newObject_";
	n.append(std::to_string(sObjectNumber));
	sObjectNumber++;
	if (parent == NULL)
	{
		parent = m_rootObject;
	}
	ZobObject* z = new ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobOject, n, NULL, parent);
}

void ZobObjectManager::RemoveZobObject(ZobObject* z)
{
	delete z;
}

ZobObject* ZobObjectManager::GetZobObjectFromPartialId(const uint id) const
{
	ZobObject* ret = GetZobObjectFromPartialId(m_rootObject, id);
	return ret;
}

ZobObject* ZobObjectManager::GetZobObjectFromPartialId(ZobObject* z, const uint id) const
{
	if (z->GetId() == id)
	{
		return z;
	}
	else
	{
		for (int i = 0; i < z->GetNbChildren(); i++)
		{
			ZobObject* o = GetZobObjectFromPartialId(z->GetChild(i), id);
			if(o)
			{
				return o;
			}
		}
	}
}

ZobObject* ZobObjectManager::GetZobObject(const std::string& name) const
{

	std::size_t current, previous = 0;
	std::vector<std::string> v;
	current = name.find_first_of('/');
	while (current != std::string::npos) {
		v.push_back(name.substr(previous, current - previous));
		previous = current + 1;
		current = name.find_first_of('/', previous);
	}
	v.push_back(name.substr(previous, current - previous));
	ZobObject* z = m_rootObject;
	for(int i=0; i<v.size(); i++)
	{
		z = z->GetChild(v.at(i));
		if(z == NULL)
		{ 
			return NULL;
		}
	}
	return z;

}

void ZobObjectManager::UpdateObjects()
{
	Matrix4x4 m;
	m.Identity();
	m_rootObject->Update(&m);
}

void ZobObjectManager::DrawObjects(const Camera* camera, Core::Engine* engine)
{
	m_rootObject->Draw(camera, engine);
}

std::string ZobObjectManager::GetZobObjectList()
{
	std::string outStr = "";
	GetZobObjectListInternal(m_rootObject, outStr);
	outStr.append("");
	return outStr;
}

void ZobObjectManager::GetZobObjectListInternal(const ZobObject* z, std::string& str)
{
	str.append("{\"name\":\"");
	str.append(z->GetName());
	str.append("\"");
	const std::vector<ZobObject*>* c = z->getChildren();
	//if (c->size() > 0)
	{
		str.append(",\"children\":[");
		for (int i = 0; i < c->size(); i++)
		{
			GetZobObjectListInternal(c->at(i), str);
			if (i < (c->size() - 1))
			{
				str.append(",");
			}
		}
		str.append("]");
	}
	str.append("}");
}

void ZobObjectManager::UnloadAll()
{
	delete m_rootObject;
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL, NULL);
}
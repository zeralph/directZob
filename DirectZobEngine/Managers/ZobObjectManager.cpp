#include <thread>
#include "ZobObjectManager.h"
#include "DirectZob.h"
#include "MeshManager.h"
#include <mutex>
#include <condition_variable>
#undef None
#include "../../dependencies/optick/include/optick.h"


static std::string emptyStr = std::string("");

ZobObjectManager::ZobObjectManager()
{
	m_deletedIds.clear();
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL);
}

ZobObjectManager::~ZobObjectManager()
{
}

bool ZobObjectManager::IsDeleted(ulong id)
{
	for (std::vector<ulong>::const_iterator iter = m_deletedIds.begin(); iter != m_deletedIds.end(); iter++)
	{
		if (*iter == id)
		{
			return true;
		}
	}
	return false;
}

void ZobObjectManager::AddZobObject(ZobObject* z)
{
	if (!m_rootObject->GetChild(z->GetName()))
	{
		m_rootObject->AddChildReference(z);
	}
}

ZobObject* ZobObjectManager::CreateZobObject(ZobObject* parent)
{
	if (parent == NULL)
	{
		parent = m_rootObject;
	}
	return new ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobOject, emptyStr, parent);
}

ZobObject* ZobObjectManager::CreateEditorZobObject(ZobObject* parent)
{
	if (parent == NULL)
	{
		parent = m_rootObject;
	}
	return new ZobObject(ZOBGUID::type_editor, ZOBGUID::subtype_zobOject, emptyStr, parent);
}

ZobSprite* ZobObjectManager::CreateZobSprite(ZobObject* parent)
{
	if (parent == NULL)
	{
		parent = m_rootObject;
	}
	return new ZobSprite(emptyStr, parent);
}

void ZobObjectManager::RemoveZobObject(ZobObject* z)
{
	z->MarkForDeletion();
	//todo : delay deletion
	//delete z;
}

ZobObject* ZobObjectManager::GetZobObjectFromlId(const std::string& id) const
{
	ZOBGUID guid = ZOBGUID(id);
	ZobObject* ret = GetZobObjectFromId(m_rootObject, guid.GetIdValue());
	return ret;
}

ZobObject* ZobObjectManager::GetZobObjectFromlId(const zobId id) const
{
	ZobObject* ret = GetZobObjectFromId(m_rootObject, id);
	return ret;
}

ZobObject* ZobObjectManager::GetZobObjectFromId(ZobObject* z, zobId id) const
{
	if (z->GetIdValue() == id)
	{
		return z;
	}
	else
	{
		for (int i = 0; i < z->GetNbChildren(); i++)
		{
			ZobObject* o = GetZobObjectFromId(z->GetChild(i), id);
			if(o)
			{
				return o;
			}
		}
	}
	return NULL;
}

ZobObject* ZobObjectManager::GetRootObject() const
{
	return m_rootObject;
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

void ZobObjectManager::Init()
{
	m_rootObject->Init();
}

void ZobObjectManager::PreUpdate()
{
	OPTICK_EVENT();
	m_rootObject->PreUpdate();
}

void ZobObjectManager::PostUpdate()
{
	OPTICK_EVENT();
	m_rootObject->PostUpdate();
}

void ZobObjectManager::EditorUpdate()
{
	m_rootObject->EditorUpdate();
}

void ZobObjectManager::UpdateObjects(const Camera* camera, Core::Engine* engine, float dt)
{
	OPTICK_EVENT();
	m_rootObject->Update(dt);
	m_time = (float)(clock() - m_drawTick) / CLOCKS_PER_SEC * 1000;
}

void ZobObjectManager::GetZobObjectList(std::string& s)
{
	GetZobObjectListInternal(m_rootObject, s);
	s.append("");
}

 void ZobObjectManager::GetZobObjectList(std::vector<const ZobObject*>& v)
{
	v.clear();
	GetZobObjectListInternal(m_rootObject, v);
}

void ZobObjectManager::GetZobObjectListInternal(const ZobObject* z, std::vector<const ZobObject*>& v)
{
	if (z->GetType() != ZOBGUID::type_editor && !z->IsMarkedForDeletion())
	{
		v.push_back(z);
		const std::vector<ZobObject*>* c = z->GetChildren();
		for (int i = 0; i < c->size(); i++)
		{
			GetZobObjectListInternal(c->at(i), v);
		}
	}
}

void ZobObjectManager::GetZobObjectListInternal(const ZobObject* z, std::string& str)
{
	if (z->GetType() != ZOBGUID::type_editor && !z->IsMarkedForDeletion())
	{
		str.append("{\"name\":\"");
		str.append(z->GetName());
		str.append("\"");
		const std::vector<ZobObject*>* c = z->GetChildren();
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
		str.append("}");
	}
}

void ZobObjectManager::UnloadAll()
{
	delete m_rootObject;
	m_deletedIds.clear();
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL);
}

bool ZobObjectManager::Reparent(ZobObject*o, ZobObject* parent)
{
	if (o)
	{
		o->SetParent(parent);
		return true;
	}
	return false;
}

void ZobObjectManager::SaveTransforms()
{
	m_rootObject->SaveTransform();

}

void ZobObjectManager::RestoreTransforms()
{
	m_rootObject->RestoreTransform();
}

void ZobObjectManager::ResetPhysic()
{
	m_rootObject->ResetPhysic();
}
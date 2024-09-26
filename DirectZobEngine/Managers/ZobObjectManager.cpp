#include <thread>
#include "ZobObjectManager.h"
#include "../DirectZob.h"
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
	m_rootObject = new ZobObject(ZobEntity::type_internal, ZobEntity::subtype_zobOject, n, NULL);
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
	return new ZobObject(ZobEntity::type_scene, ZobEntity::subtype_zobOject, emptyStr, parent);
}

void ZobObjectManager::RemoveZobObject(ZobObject* z)
{
	z->MarkForDeletion();
	//todo : delay deletion
	//delete z;
}

ZobObject* ZobObjectManager::CreateEditorZobObject(ZobObject* parent)
{
	if (parent == NULL)
	{
		parent = m_rootObject;
	}
	return new ZobObject(ZobEntity::type_editor, ZobEntity::subtype_zobOject, emptyStr, parent);
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

void ZobObjectManager::Init(DirectZobType::sceneLoadingCallback cb)
{
	m_rootObject->Init(cb);
}

void ZobObjectManager::PreUpdate(float dt)
{
	OPTICK_EVENT();
	m_rootObject->PreUpdate(dt);
}

void ZobObjectManager::PostUpdate()
{
	OPTICK_EVENT();
	m_rootObject->PostUpdate();
}

void ZobObjectManager::DrawGizmos(const Camera* camera, Engine* engine)
{
	OPTICK_EVENT();
	m_rootObject->DrawGizmos(camera, engine);
}

void ZobObjectManager::QueueForDrawing(const Camera* camera, Engine* engine)
{
	OPTICK_EVENT();
	m_rootObject->QueueForDrawing(camera, engine);
}

void ZobObjectManager::EditorUpdate()
{
	m_rootObject->EditorUpdate();
}

void ZobObjectManager::UpdateObjects(const Camera* camera, Engine* engine, float dt)
{
	OPTICK_EVENT();
	m_rootObject->Update(dt);
	m_time = (float)(clock() - m_drawTick) / CLOCKS_PER_SEC * 1000;
}

void ZobObjectManager::GetZobObjectList(std::string& s) const
{
	GetZobObjectListInternal(m_rootObject, s);
	s.append("");
}

 void ZobObjectManager::GetZobObjectList(std::vector<const ZobObject*>& v) const
{
	v.clear();
	GetZobObjectListInternal(m_rootObject, v);
}

void ZobObjectManager::GetZobObjectListInternal(const ZobObject* z, std::vector<const ZobObject*>& v) const
{
	if (ZobEntity::GetType(z->GetIdValue()) != ZobEntity::type_editor && !z->IsMarkedForDeletion())
	{
		v.push_back(z);
		const std::vector<ZobObject*>* c = z->GetChildren();
		for (int i = 0; i < c->size(); i++)
		{
			GetZobObjectListInternal(c->at(i), v);
		}
	}
}

void ZobObjectManager::GetZobObjectListInternal(const ZobObject* z, std::string& str) const
{
	if (ZobEntity::GetType(z->GetIdValue()) != ZobEntity::type_editor && !z->IsMarkedForDeletion())
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
	m_rootObject = new ZobObject(ZobEntity::type_internal, ZobEntity::subtype_zobOject, n, NULL);
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
#include <thread>
#include "ZobObjectManager.h"
#include "DirectZob.h"
#include "MeshManager.h"

static std::thread g_geometryThread;
static std::string emptyStr = std::string("");

ZobObjectManager::ZobObjectManager()
{
	m_deletedIds.clear();
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL);
}

ZobObjectManager::~ZobObjectManager()
{
	WaitForUpdateObjectend();
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

void ZobObjectManager::PreUpdate()
{
	m_rootObject->PreUpdate();
}

void ZobObjectManager::UpdateBehavior()
{
	m_rootObject->UpdateBehavior();
}

void ZobObjectManager::StartUpdateScene(const Camera* camera, Core::Engine* engine)
{
	m_drawTick = clock();
	g_geometryThread = std::thread(&ZobObjectManager::UpdateObjects, this, camera, engine);
}

float ZobObjectManager::WaitForUpdateObjectend()
{
	if (g_geometryThread.joinable())
		g_geometryThread.join();
	return m_time;
}

void ZobObjectManager::UpdateObjects(const Camera* camera, Core::Engine* engine)
{
	m_rootObject->Update();
	m_rootObject->UpdateMesh(camera, engine);
	m_time = (float)(clock() - m_drawTick) / CLOCKS_PER_SEC * 1000;
}

void ZobObjectManager::QueueForDrawing(const Camera* camera, Core::Engine* engine)
{
	//m_rootObject->UpdateMesh(camera, engine);
	m_rootObject->QueueForDrawing(camera, engine);
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
	ZOBGUID::Reset();
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL);
}

void ZobObjectManager::CreateEditorGizmos(std::string& editorResourcesPath)
{
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
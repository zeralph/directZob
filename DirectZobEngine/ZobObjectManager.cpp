#include "ZobObjectManager.h"
#include "DirectZob.h"
#include "MeshManager.h"

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
		m_rootObject->AddChildReference(z);
	}
}

ZobObject* ZobObjectManager::CreateZobObject(ZobObject* parent)
{
	std::string n = "newObject_";
	n.append(std::to_string(sObjectNumber));
	sObjectNumber++;
	if (parent == NULL)
	{
		parent = m_rootObject;
	}
	return new ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobOject, n, NULL, parent);
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

void ZobObjectManager::UpdateObjects()
{
	Matrix4x4 m;
	m.Identity();
	m_rootObject->Update(&m, &m);
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
	if (z->GetType() != ZOBGUID::type_editor)
	{
		str.append("{\"name\":\"");
		str.append(z->GetName());
		str.append("\"");
		const std::vector<ZobObject*>* c = z->getChildren();
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
	delete m_editorGizmos;
	delete m_rootObject;
	m_editorGizmos = NULL;
	m_transform = NULL;
	m_transformX = NULL;
	m_transformY = NULL;
	m_transformZ = NULL;
	m_rotate = NULL;
	m_rotateX = NULL;
	m_rotateY = NULL;
	m_rotateZ = NULL;
	m_scale = NULL;
	m_scaleX = NULL;
	m_scaleY = NULL;
	m_scaleZ = NULL;
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL, NULL);
}

void ZobObjectManager::CreateEditorGizmos(std::string& editorResourcesPath)
{
	if (m_editorGizmos == NULL)
	{
		std::string name;
		name = "gizmos";
		m_editorGizmos = new ZobObject(ZOBGUID::type_editor, ZOBGUID::subtype_zobOject, name, NULL, m_rootObject);

		m_transform = LoadEditorMesh("transform", editorResourcesPath.c_str(), "transform.obj", m_editorGizmos);
		m_transformX = LoadEditorMesh("transformX", editorResourcesPath.c_str(), "transformX.obj", m_transform);
		m_transformY = LoadEditorMesh("transformY", editorResourcesPath.c_str(), "transformY.obj", m_transform);
		m_transformZ = LoadEditorMesh("transformZ", editorResourcesPath.c_str(), "transformZ.obj", m_transform);

		m_rotate = LoadEditorMesh("rotate", editorResourcesPath.c_str(), "rotate.obj", m_editorGizmos);
		m_rotateX = LoadEditorMesh("rotateX", editorResourcesPath.c_str(), "rotateX.obj", m_rotate);
		m_rotateY = LoadEditorMesh("rotateY", editorResourcesPath.c_str(), "rotateY.obj", m_rotate);
		m_rotateZ = LoadEditorMesh("rotateZ", editorResourcesPath.c_str(), "rotateZ.obj", m_rotate);

		m_scale = LoadEditorMesh("scale", editorResourcesPath.c_str(), "scale.obj", m_editorGizmos);
		m_scaleX = LoadEditorMesh("scaleX", editorResourcesPath.c_str(), "scaleX.obj", m_scale);
		m_scaleY = LoadEditorMesh("scaleY", editorResourcesPath.c_str(), "scaleY.obj", m_scale);
		m_scaleZ = LoadEditorMesh("scaleZ", editorResourcesPath.c_str(), "scaleZ.obj", m_scale);

	}
}

ZobObject* ZobObjectManager::LoadEditorMesh(const char* name, const char* meshPath, const char* meshFile, ZobObject* parent)
{
	MeshManager* meshMgr = DirectZob::GetInstance()->GetMeshManager();
	std::string n = std::string(name);
	std::string p = std::string(meshPath);
	std::string m = std::string(meshFile);
	Mesh* mesh = meshMgr->LoadMesh(n, p, m);
	ZobObject* transform = new ZobObject(ZOBGUID::type_editor, ZOBGUID::subtype_zobOject, n, mesh, parent);
	transform->GetRenderOptions()->LightMode(RenderOptions::eLightMode_none);
	transform->GetRenderOptions()->ZBuffered(false);
	return transform;
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
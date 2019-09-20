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
	return NULL;
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
	delete m_editorGizmos;
	delete m_rootObject;
	m_editorGizmos = NULL;
	std::string n = "root";
	m_rootObject = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, NULL, NULL);
}

void ZobObjectManager::CreateEditorGizmos(std::string& editorResourcesPath)
{
	if (m_editorGizmos == NULL)
	{
		std::string name;
		name = "gizmos";
		ZobObject* gizmos = new ZobObject(ZOBGUID::type_editor, ZOBGUID::subtype_zobOject, name, NULL, m_rootObject);

		ZobObject* transform = LoadEditorMesh("transform", editorResourcesPath.c_str(), "transform.obj", gizmos);
		ZobObject* transformX = LoadEditorMesh("transformX", editorResourcesPath.c_str(), "transformX.obj", transform);
		ZobObject* transformY = LoadEditorMesh("transformY", editorResourcesPath.c_str(), "transformY.obj", transform);
		ZobObject* transformZ = LoadEditorMesh("transformZ", editorResourcesPath.c_str(), "transformZ.obj", transform);

		ZobObject* rotate = LoadEditorMesh("rotate", editorResourcesPath.c_str(), "rotate.obj", gizmos);
		ZobObject* rotateX = LoadEditorMesh("rotateX", editorResourcesPath.c_str(), "rotateX.obj", rotate);
		ZobObject* rotateY = LoadEditorMesh("rotateY", editorResourcesPath.c_str(), "rotateY.obj", rotate);
		ZobObject* rotateZ = LoadEditorMesh("rotateZ", editorResourcesPath.c_str(), "rotateZ.obj", rotate);

		ZobObject* scale = LoadEditorMesh("scale", editorResourcesPath.c_str(), "scale.obj", gizmos);
		ZobObject* scaleX = LoadEditorMesh("scaleX", editorResourcesPath.c_str(), "scaleX.obj", scale);
		ZobObject* scaleY = LoadEditorMesh("scaleY", editorResourcesPath.c_str(), "scaleY.obj", scale);
		ZobObject* scaleZ = LoadEditorMesh("scaleZ", editorResourcesPath.c_str(), "scaleZ.obj", scale);

		m_editorGizmos = gizmos;
	}
}

ZobObject* ZobObjectManager::LoadEditorMesh(const char* name, const char* meshPath, const char* meshFile, ZobObject* parent)
{
	MeshManager* meshMgr = DirectZob::GetInstance()->GetMeshManager();
	std::string n = std::string(name);
	std::string p = std::string(meshPath);
	std::string m = std::string(meshFile);
	Mesh* mesh = meshMgr->LoadMesh(n, p, m);
	ZobObject* transform = new ZobObject(ZOBGUID::type_internal, ZOBGUID::subtype_zobOject, n, mesh, parent);
	transform->GetRenderOptions().Lighted(false);
	transform->GetRenderOptions().ZBuffered(false);
	return transform;
}

bool ZobObjectManager::Reparent(ZobObject *o, ZobObject* parent)
{
	if (o)
	{
		o->SetParent(parent);
		return true;
	}
	return false;
}
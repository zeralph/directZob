#include "SceneLoader.h"
#include "DirectZob.h"

std::string SceneLoader::m_path = "";
std::string SceneLoader::m_file = "";

void SceneLoader::LoadMesh(TiXmlElement* node, MeshManager* meshMgr, MaterialManager* materialManager)
{
	std::string name = node->Attribute("name");
	std::string file = node->Attribute("file");
	meshMgr->LoadMesh(name, m_path, file);
}

void SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent, ZobObjectManager* zobMgr, MeshManager* meshMgr)
{
	float x, y, z;
	std::string name;
	std::string mesh;
	TiXmlElement* f;
	
	name = node->Attribute("name");
	f = node->FirstChildElement("Position");
	x = atof(f->Attribute("x"));
	y = atof(f->Attribute("y"));
	z = atof(f->Attribute("z"));
	Vector3 position = Vector3(x, y, z);
	f = node->FirstChildElement("Rotation");
	x = atof(f->Attribute("x"));
	y = atof(f->Attribute("y"));
	z = atof(f->Attribute("z"));
	Vector3 rotation = Vector3(x, y, z);
	f = node->FirstChildElement("Scale");
	x = atof(f->Attribute("x"));
	y = atof(f->Attribute("y"));
	z = atof(f->Attribute("z"));
	Vector3 scale = Vector3(x, y, z);
	f = node->FirstChildElement("Mesh");
	std::string meshName = f ? f->GetText() : "";
	Mesh* m = meshMgr->GetMesh(meshName);
	if (parent == NULL)
	{
		parent = zobMgr->GetRootObject();
	}
	ZobObject* zob = new ZobObject(ZOBGUID::type_scene, ZOBGUID::subtype_zobOject, name, m, parent);
	zob->SetTranslation(position.x, position.y, position.z);
	zob->SetRotation(rotation.x, rotation.y, rotation.z);
	zob->SetScale(scale.x, scale.y, scale.z);
	for (TiXmlElement* e = node->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
	{
		LoadZobObject(e, zob, zobMgr, meshMgr);
	}
}

void SceneLoader::LoadScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* materialManager)
{
	m_path = path;
	m_file = file;
	float x, y, z, fov, znear, zfar;
	std::string name, texture, fullPath;
	TiXmlDocument doc("Scene");
	fullPath = path + file;
	doc.ClearError();
	doc.LoadFile(fullPath.c_str());
	if (doc.Error())
	{
		std::string err = "Error loading ";
		err.append(fullPath.c_str());
		err.append(" : ");
		err.append(doc.ErrorDesc());
		DirectZob::LogError(err.c_str());
		m_path = "";
		m_file = "";
	}
	else
	{
		TiXmlElement* root = doc.FirstChildElement("root");
		/*TiXmlElement* textures = root->FirstChildElement("Textures");
		for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
		{
			LoadTexture(e, MaterialManager);
		}*/
		TiXmlElement* meshes = root->FirstChildElement("Meshes");
		for (TiXmlElement* e = meshes->FirstChildElement("Mesh"); e != NULL; e = e->NextSiblingElement("Mesh"))
		{
			LoadMesh(e, meshManager, materialManager);
		}
		TiXmlElement* scene = root->FirstChildElement("Scene");
		for (TiXmlElement* e = scene->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
		{
			LoadZobObject(e, NULL, zobObjectManager, meshManager);
		}
	}
}

void SceneLoader::UnloadScene(Core::Engine* engine, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* MaterialManager)
{
	engine->Stop();
	Sleep(1000);	//ugly but ...
	zobObjectManager->UnloadAll();
	meshManager->UnloadAll();
	MaterialManager->UnloadAll();
}

void SceneLoader::NewScene(Core::Engine* engine, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* MaterialManager)
{
	UnloadScene(engine, zobObjectManager, meshManager, MaterialManager);
	m_path = "";
	m_file = "";
	engine->Start();
}

void SceneLoader::SaveScene(ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* MaterialManager)
{
	if (CanFastSave())
	{
		SaveScene(m_path, m_file, zobObjectManager, meshManager, MaterialManager);
	}
}

void SceneLoader::SaveScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* MaterialManager)
{
	std::string fullPath;
	m_file = file;
	m_path = path;
	fullPath = path + file;
	TiXmlDocument doc("Scene");
	TiXmlElement * root = new TiXmlElement("root");
	doc.LinkEndChild(root);
	TiXmlElement meshes = TiXmlElement("Meshes");
	int n = meshManager->GetNbMeshes();
	for (int i = 0; i < n; i++)
	{
		TiXmlElement e = TiXmlElement("Mesh");
		e.SetAttribute("name", meshManager->GetMesh(i)->GetName().c_str());
		e.SetAttribute("file", meshManager->GetMesh(i)->GetFile().c_str());
		meshes.InsertEndChild(e);
	}
	root->InsertEndChild(meshes);
	TiXmlElement scene = TiXmlElement("Scene");
	ZobObject* rootObj = zobObjectManager->GetRootObject();
	for (int i = 0; i < rootObj->GetNbChildren(); i++)
	{
		SaveZobObjectRecusrive(&scene, rootObj->GetChild(i));
	}
	root->InsertEndChild(scene);
	if (!doc.SaveFile(fullPath.c_str()))
	{
		std::string s = "Error saving scene : ";
		s.append(doc.ErrorDesc());
		DirectZob::LogError(s.c_str());
	}
}

void SceneLoader::SaveZobObjectRecusrive(TiXmlElement* node, ZobObject* z)
{
	if (z->GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	TiXmlElement o = TiXmlElement("ZobObject");
	TiXmlElement p = TiXmlElement("Position");
	TiXmlElement r = TiXmlElement("Rotation");
	TiXmlElement s = TiXmlElement("Scale");
	o.SetAttribute("name", z->GetName().c_str());
	p.SetDoubleAttribute("x", z->GetTransform().x);
	p.SetDoubleAttribute("y", z->GetTransform().y);
	p.SetDoubleAttribute("z", z->GetTransform().z);
	r.SetDoubleAttribute("x", z->GetRotation().x);
	r.SetDoubleAttribute("y", z->GetRotation().y);
	r.SetDoubleAttribute("z", z->GetRotation().z);
	s.SetDoubleAttribute("x", z->GetScale().x);
	s.SetDoubleAttribute("y", z->GetScale().y);
	s.SetDoubleAttribute("z", z->GetScale().z);
	std::string meshName = z->GetMeshName();
	if (meshName.length() >0 )
	{
		TiXmlElement m = TiXmlElement("Mesh");
		TiXmlText t = TiXmlText(meshName.c_str());
		m.InsertEndChild(t);
		o.InsertEndChild(m);
	}
	o.InsertEndChild(p);
	o.InsertEndChild(r);
	o.InsertEndChild(s);
	for (int i = 0; i < z->GetNbChildren(); i++)
	{
		SaveZobObjectRecusrive(&o, z->GetChild(i));
	}
	node->InsertEndChild(o);
}
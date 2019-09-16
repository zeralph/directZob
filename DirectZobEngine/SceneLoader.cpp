#include "SceneLoader.h"
#include "DirectZob.h"

std::string SceneLoader::m_path = "";
std::string SceneLoader::m_file = "";

void SceneLoader::LoadTexture(TiXmlElement* node, TextureManager* texMgr)
{
	std::string name = node->Attribute("name");
	std::string file = node->Attribute("file");
	texMgr->LoadTexture(name, m_path, file);
}

void SceneLoader::LoadMesh(TiXmlElement* node, MeshManager* meshMgr, TextureManager* texMgr)
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

void SceneLoader::LoadScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, MeshManager* meshManager, TextureManager* textureManager)
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
	}
	else
	{
		TiXmlElement* root = doc.FirstChildElement("root");
		/*TiXmlElement* textures = root->FirstChildElement("Textures");
		for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
		{
			LoadTexture(e, textureManager);
		}*/
		TiXmlElement* meshes = root->FirstChildElement("Meshes");
		for (TiXmlElement* e = meshes->FirstChildElement("Mesh"); e != NULL; e = e->NextSiblingElement("Mesh"))
		{
			LoadMesh(e, meshManager, textureManager);
		}
		TiXmlElement* scene = root->FirstChildElement("Scene");
		for (TiXmlElement* e = scene->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
		{
			LoadZobObject(e, NULL, zobObjectManager, meshManager);
		}
	}
}

void SceneLoader::UnloadScene(Core::Engine* engine, ZobObjectManager* zobObjectManager, MeshManager* meshManager, TextureManager* textureManager)
{
	engine->Stop();
//	Sleep(1000);	//ugly but ...
	zobObjectManager->UnloadAll();
	meshManager->UnloadAll();
	textureManager->UnloadAll();
}

void SceneLoader::NewScene(Core::Engine* engine, ZobObjectManager* zobObjectManager, MeshManager* meshManager, TextureManager* textureManager)
{
	UnloadScene(engine, zobObjectManager, meshManager, textureManager);
	m_path = "";
	m_file = "";
	engine->Start();
}

void SceneLoader::SaveScene(ZobObjectManager* zobObjectManager, MeshManager* meshManager, TextureManager* textureManager)
{
	if (CanFastSave())
	{
		SaveScene(m_path, m_file, zobObjectManager, meshManager, textureManager);
	}
}

void SceneLoader::SaveScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, MeshManager* meshManager, TextureManager* textureManager)
{
	std::string fullPath;
	m_file = file;
	m_path = path;
	fullPath = path + file;
	TiXmlDocument doc("Scene");
	TiXmlElement * root = new TiXmlElement("root");
	doc.LinkEndChild(root);
	TiXmlElement * e;
	e = new TiXmlElement("Textures");
	root->LinkEndChild(e);
	int n = textureManager->GetNbTextures();
	for (int i = 0; i < n; n++)
	{
		e = new TiXmlElement("Texture");
		e->SetAttribute("name", textureManager->GetTexture(i)->GetName().c_str());
		e->SetAttribute("file", textureManager->GetTexture(i)->GetFile().c_str());
	}
}
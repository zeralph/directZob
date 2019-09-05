#include "SceneLoader.h"
#include "DirectZob.h"

std::string SceneLoader::m_path = "";
std::string SceneLoader::m_file = "";
std::vector<Mesh*> SceneLoader::m_meshes;

void SceneLoader::LoadTexture(TiXmlElement* node, TextureManager* texMgr)
{
	std::string name = node->Attribute("name");
	std::string fullPath = m_path + node->Attribute("file");
	texMgr->LoadTexture(name, fullPath);
}

void SceneLoader::LoadMesh(TiXmlElement* node, TextureManager* texMgr)
{
	std::string name = node->Attribute("name");
	std::string fullPath = m_path + node->Attribute("file");
	std::string texStr = node->Attribute("texture");
	const Texture* tex = texMgr->GetTexture(texStr);
	Mesh* mesh = new Mesh(name, fullPath, tex);
	m_meshes.push_back(mesh);
}

void SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent, ZobObjectManager* zobMgr)
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
	Mesh* m = SceneLoader::GetMesh(meshName);
	ZobObject* zob = new ZobObject(name, m, parent);
	zob->SetTranslation(position.x, position.y, position.z);
	zob->SetRotation(rotation.x, rotation.y, rotation.z);
	zob->SetScale(scale.x, scale.y, scale.z);
	if (parent == NULL)
	{
		zobMgr->AddZobObject(zob);
	}
	else
	{
		parent->AddChild(zob);
	}
	for (TiXmlElement* e = node->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
	{
		LoadZobObject(e, zob, zobMgr);
	}
}

void SceneLoader::LoadScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, TextureManager* textureManager)
{
	m_path = path;
	m_file = file;
	float x, y, z, fov, znear, zfar;
	std::string name, texture, fullPath;
	TiXmlDocument doc("Scene");
	fullPath = path + file;
	doc.LoadFile(fullPath.c_str());

	if (doc.Error())
	{
		std::string err = "Error loading ";
		err.append(fullPath.c_str());
		DirectZob::Log(err);
	}
	else
	{
		TiXmlElement* root = doc.FirstChildElement("root");
		TiXmlElement* textures = root->FirstChildElement("Textures");
		for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
		{
			LoadTexture(e, textureManager);
		}
		TiXmlElement* meshes = root->FirstChildElement("Meshes");
		for (TiXmlElement* e = meshes->FirstChildElement("Mesh"); e != NULL; e = e->NextSiblingElement("Mesh"))
		{
			LoadMesh(e, textureManager);
		}
		TiXmlElement* scene = root->FirstChildElement("Scene");
		for (TiXmlElement* e = scene->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
		{
			LoadZobObject(e, NULL, zobObjectManager);
		}
	}
}

Mesh* SceneLoader::GetMesh(std::string& mesh)
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		if (m_meshes.at(i)->GetName() == mesh)
		{
			return m_meshes.at(i);
		}
	}
	return NULL;
}
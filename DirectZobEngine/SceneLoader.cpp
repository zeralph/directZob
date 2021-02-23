#include "SceneLoader.h"
#include "DirectZob.h"

std::string SceneLoader::m_path = "";
std::string SceneLoader::m_file = "";
char tmpBuffer[256];

void SceneLoader::LoadMesh(TiXmlElement* node)
{
	std::string name = node->Attribute("name");
	std::string file = node->Attribute("file");
	DirectZob::GetInstance()->GetMeshManager()->LoadMesh(name, m_path, file);
}

void SceneLoader::LoadZobObject(std::string& path, std::string& file)
{
	std::string fullPath;
	TiXmlDocument doc("ZobObject");
	fullPath = path + file;
	LoadZobObject(fullPath);
}

void SceneLoader::LoadZobObject(std::string& fullPath, ZobObject* parent /* = NULL*/)
{
	TiXmlDocument doc("ZobObject");
	doc.ClearError();
	doc.LoadFile(fullPath.c_str());
	if (doc.Error())
	{
		DirectZob::LogError("Error loading %s : %s", fullPath.c_str(), doc.ErrorDesc());
		m_path = "";
		m_file = "";
	}
	else
	{
		ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
		TiXmlElement* n = doc.FirstChildElement("ZobObject");
		LoadZobObject(n, parent, &fullPath);
	}
}

void SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent, const std::string* factoryPath)
{	
	std::string type;
	ZobObject* zob = NULL;
	if (parent == nullptr)
	{
		ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
		parent = zobObjectManager->GetRootObject();
	}
	type = node->Attribute("type")? node->Attribute("type"):"mesh";
	ulong id = node->Attribute("guid") ? strtoul(node->Attribute("guid"), NULL, 0) : 0;
	if (type == "mesh")
	{
		zob = new ZobObject(id, node, parent, factoryPath);
	}
	else if (type == "camera")
	{
		Camera* c = new Camera(id, node, parent);
		DirectZob::GetInstance()->GetCameraManager()->AddCamera(c);
		zob = c;
	}
	else if (type == "sprite")
	{
		ZobSprite* s = new ZobSprite(id, node, parent);
		zob = s;
	}
	else if (type == "light")
	{
		Light* l = new Light(id, node, parent);
		DirectZob::GetInstance()->GetLightManager()->AddLight(l);
		zob = l;
	}
	else
	{
		std::string name = node->Attribute("name");
		DirectZob::LogError("Error creating ZObjects %s", name.c_str());
	}
	for (TiXmlElement* e = node->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
	{
		const char* file = e->Attribute("file");
		if (file)
		{
			std::string f = std::string(file);
			LoadZobObject(f, zob);
		}
		else
		{
			LoadZobObject(e, zob, factoryPath);
		}
	}
}

void SceneLoader::UnloadScene()
{
	DirectZob::AddIndent();
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	LightManager* lightManager = DirectZob::GetInstance()->GetLightManager();
	CameraManager* cameraManager = DirectZob::GetInstance()->GetCameraManager();
	Engine* engine = DirectZob::GetInstance()->GetEngine();
	engine->Stop();
	SLEEP_MS(1000);	//ugly but ...
	zobObjectManager->UnloadAll();
	meshManager->UnloadAll();
	materialManager->UnloadAll();
	lightManager->UnloadAll();
	cameraManager->UnloadAll();
	DirectZob::RemoveIndent();
}

void SceneLoader::NewScene()
{
	DirectZob::AddIndent();
	UnloadScene();
	m_path = "";
	m_file = "";
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	DirectZob::GetInstance()->GetEngine()->Start();
	DirectZob::RemoveIndent();
}

void SceneLoader::LoadScene(std::string& path, std::string& file)
{
	DirectZob::AddIndent();
	DirectZob::LogInfo("Load scene %s%s", path.c_str(), file.c_str());
	UnloadScene();
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
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
		DirectZob::LogError("Error loading %s : %s", fullPath.c_str(), doc.ErrorDesc());
		m_path = "";
		m_file = "";
	}
	else
	{
		DirectZob::LogInfo("loading objects");
		TiXmlElement* scene = doc.FirstChildElement("Scene");
		if (scene)
		{
			/*TiXmlElement* textures = root->FirstChildElement("Textures");
			for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
			{
				LoadTexture(e, MaterialManager);
			}*/
			DirectZob::GetInstance()->GetLightManager()->LoadFromNode(scene->FirstChildElement("Globals"));
			for (TiXmlElement* e = scene->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
			{
				const char* file = e->Attribute("file");
				if (file)
				{
					std::string f = std::string(file);
					LoadZobObject(f);
				}
				else
				{
					LoadZobObject(e, NULL, NULL);
				}
			}
			DirectZob::LogInfo("Scene loaded");
		}
		else
		{
			DirectZob::LogError("Scene file has error");
		}
	}
	DirectZob::GetInstance()->GetEngine()->Start();
	DirectZob::RemoveIndent();
}

void SceneLoader::SaveScene()
{
	if (CanFastSave())
	{
		SaveScene(m_path, m_file);
	}
}

void SceneLoader::SaveScene(std::string &path, std::string &file)
{
	DirectZob::LogInfo("Saving scene to %s/%s", path.c_str(), file.c_str());
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	std::string fullPath;
	m_file = file;
	m_path = path;
	fullPath = path + file;
	TiXmlDocument doc("Scene");
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	TiXmlElement * root = new TiXmlElement("Scene");
	doc.LinkEndChild(root);
	TiXmlElement globals = TiXmlElement("Globals");
	DirectZob::GetInstance()->GetLightManager()->SaveUnderNode(&globals);
	root->InsertEndChild(globals);	
	ZobObject* rootObj = zobObjectManager->GetRootObject();
	bool bok = true;
	for (int i = 0; i < rootObj->GetNbChildren(); i++)
	{
		bok &= SaveZobObjectRecusrive(root, rootObj->GetChild(i));
	}
	if (!bok)
	{
		DirectZob::LogError("Error saving objects in scene : ", doc.ErrorDesc());
	}
	else if (!doc.SaveFile(fullPath.c_str()))
	{
		DirectZob::LogError("Error saving scene : ", doc.ErrorDesc());
	}
	else
	{
		DirectZob::LogInfo("Scene saved");
	}
}

bool SceneLoader::SaveZobObjectRecusrive(TiXmlNode* node, ZobObject* z)
{
	if (z->GetType() == ZOBGUID::type_editor)
	{
		return true;
	}
	TiXmlNode* newNode = z->SaveUnderNode(node);
	if (!newNode)
	{
		DirectZob::LogError("Error saving Object : ", z->GetName().c_str());
		return false;
	}
	else
	{
		bool bok = true;
		for (int i = 0; i < z->GetNbChildren(); i++)
		{
			bok &= SaveZobObjectRecusrive(newNode, z->GetChild(i));
		}
		return bok && (node->InsertEndChild(*newNode) != NULL);
	}
}

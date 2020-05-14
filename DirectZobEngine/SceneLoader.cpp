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

void SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent)
{	
	std::string type;
	ZobObject* zob = NULL;
	if (parent == nullptr)
	{
		ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
		parent = zobObjectManager->GetRootObject();
	}
	type = node->Attribute("type")? node->Attribute("type"):"mesh";
	if (type == "mesh")
	{
		TiXmlElement* f = node->FirstChildElement("Mesh");
		std::string meshName = f ? f->GetText() : "";
		MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
		Mesh* m = meshManager->GetMesh(meshName);
		zob = new ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobOject, node, m, parent);
	}
	else if (type == "camera")
	{
		Camera* c = new Camera(node, parent);
		DirectZob::GetInstance()->GetCameraManager()->AddCamera(c);
		zob = c;
	}
	else if (type == "light")
	{
		Light* l = new Light(node, parent);
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
		LoadZobObject(e, zob);
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
	SLEEP(1000);	//ugly but ...
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
		TiXmlElement* root = doc.FirstChildElement("root");
		/*TiXmlElement* textures = root->FirstChildElement("Textures");
		for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
		{
			LoadTexture(e, MaterialManager);
		}*/
		TiXmlElement* meshes = root->FirstChildElement("Meshes");
		for (TiXmlElement* e = meshes->FirstChildElement("Mesh"); e != NULL; e = e->NextSiblingElement("Mesh"))
		{
			LoadMesh(e);
		}
		TiXmlElement* scene = root->FirstChildElement("Scene");
		DirectZob::GetInstance()->GetLightManager()->LoadFromNode(scene->FirstChildElement("Globals"));
		for (TiXmlElement* e = scene->FirstChildElement("ZobObject"); e != NULL; e = e->NextSiblingElement("ZobObject"))
		{
			LoadZobObject(e, NULL);
		}
	}
	DirectZob::LogInfo("Scene loaded");
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
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
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
	TiXmlElement globals = TiXmlElement("Globals");
	DirectZob::GetInstance()->GetLightManager()->SaveUnderNode(&globals);
	scene.InsertEndChild(globals);	
	ZobObject* rootObj = zobObjectManager->GetRootObject();
	for (int i = 0; i < rootObj->GetNbChildren(); i++)
	{
		SaveZobObjectRecusrive(&scene, rootObj->GetChild(i));
	}
	root->InsertEndChild(scene);
	if (!doc.SaveFile(fullPath.c_str()))
	{
		DirectZob::LogError("Error saving scene : ", doc.ErrorDesc());
	}
}

void SceneLoader::SaveZobObjectRecusrive(TiXmlNode* node, ZobObject* z)
{
	if (z->GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	TiXmlNode* newNode = z->SaveUnderNode(node);
	for (int i = 0; i < z->GetNbChildren(); i++)
	{
		SaveZobObjectRecusrive(newNode, z->GetChild(i));
	}
	node->InsertEndChild(*newNode);
}

#include "SceneLoader.h"
#include "DirectZob.h"
#include "ZobPhysic/ZobPhysicsEngine.h"
#include "Misc/ZobXmlHelper.h"

//TODO !
std::string SceneLoader::m_path = "D://_Git//directZob//resources//";
std::string SceneLoader::m_file = "";
std::string SceneLoader::m_nextScenePath = "";
std::string SceneLoader::m_nextSceneName = "";
int SceneLoader::m_nbZobObjectLoaded = 0;
char tmpBuffer[256];
bool SceneLoader::m_loadNextScene = false;


void SceneLoader::Update()
{
	if (m_loadNextScene)
	{
		m_loadNextScene = false;
		m_path = m_nextScenePath;
		CleanPath(m_path);
		m_file = m_nextSceneName;
		LoadSceneInternal();
		m_nextScenePath = "";
		m_nextSceneName = "";
	}
}

void SceneLoader::LoadScene(std::string& path, std::string& file)
{
	m_nbZobObjectLoaded = 0;
	m_nextScenePath = path;
	m_nextSceneName = file;
	m_loadNextScene = true;
}

void SceneLoader::LoadMesh(TiXmlElement* node)
{
	std::string name = node->Attribute(XML_ATTR_NAME);
	std::string file = node->Attribute(XML_ATTR_FILE);
	ZobFilePath zfp = ZobFilePath(name, m_path, file, false);
	DirectZob::GetInstance()->GetMeshManager()->LoadMesh(zfp);
}

void SceneLoader::LoadZobObject(std::string& path, std::string& file)
{
	if (m_path.size() == 0)
	{
		m_path = path;
	}
	std::string fullPath;
	TiXmlDocument doc(XML_ELEMENT_ZOBOBJECT);
	fullPath = path + file;
	LoadZobObject(fullPath);
}

void SceneLoader::LoadZobObject(std::string& fullPath, ZobObject* parent /* = NULL*/)
{
	TiXmlDocument doc(XML_ELEMENT_ZOBOBJECT);
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
		TiXmlElement* n = doc.FirstChildElement(XML_ELEMENT_ZOBOBJECT);
		LoadZobObject(n, parent, &fullPath);
	}
}

void SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent, const std::string* factoryPath)
{	
	m_nbZobObjectLoaded++;
	ZobObject* zob = NULL;
	if (parent == nullptr)
	{
		ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
		parent = zobObjectManager->GetRootObject();
	}
	const char* name = node->Attribute(XML_ATTR_NAME);
	const char* type= node->Attribute(XML_ATTR_TYPE);
	const char* cid =node->Attribute(XML_ATTR_GUID);
	if (!cid)
	{
		DirectZob::LogError("Cannot create ZobObject ", name);
		return;
	}
	std::string id = cid;
	zobId zid = ZobEntity::ZobIdFromString(id);
	if (ZobEntity::GetSubType(zid) == ZobEntity::subtype_zobOject)
	{
		zob = new ZobObject(zid, node, parent, factoryPath);
	}
	else if (ZobEntity::GetSubType(zid) == ZobEntity::subtype_zobCamera)
	{
		Camera* c = new Camera(zid, node, parent);
		DirectZob::GetInstance()->GetCameraManager()->AddCamera(c);
		zob = c;
	}
	else if (ZobEntity::GetSubType(zid) == ZobEntity::subtype_zobLight)
	{
		Light* l = new Light(zid, node, parent);
		DirectZob::GetInstance()->GetLightManager()->AddLight(l);
		zob = l;
	}
	else
	{
		assert(NULL);
	}
	for (TiXmlElement* e = node->FirstChildElement(XML_ELEMENT_ZOBOBJECT); e != NULL; e = e->NextSiblingElement("ZobObject"))
	{
		const char* file = e->Attribute(XML_ATTR_FILE);
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
	DirectZob::LogInfo("Unloading scene");
	DirectZob::AddIndent();
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	LightManager* lightManager = DirectZob::GetInstance()->GetLightManager();
	CameraManager* cameraManager = DirectZob::GetInstance()->GetCameraManager();
	ZobHUDManager* hudManager = DirectZob::GetInstance()->GetHudManager();
	Engine* engine = DirectZob::GetInstance()->GetEngine();
	ZobPhysicsEngine* phy = DirectZob::GetInstance()->GetPhysicsEngine();
	engine->Stop();
	hudManager->Stop();
	ZobEntity::Init();
	cameraManager->UnloadAll();
	zobObjectManager->UnloadAll();
	meshManager->UnloadAll();
	materialManager->UnloadAll();
	lightManager->UnloadAll();
//	phy->ReInit(); 
	DirectZob::RemoveIndent();
}

void SceneLoader::NewScene(std::string workspace)
{
	DirectZob::AddIndent();
	UnloadScene();
	m_path = workspace;
	m_file = "";
	DirectZob::GetInstance()->GetHudManager()->Start();
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	DirectZob::GetInstance()->GetHudManager()->Init();
	DirectZob::GetInstance()->GetEngine()->Start();
	DirectZob::GetInstance()->GetHudManager()->Start();
	DirectZob::RemoveIndent();
}

void SceneLoader::LoadSceneInternal()
{
	DirectZob::AddIndent();
	DirectZob::LogWarning("WORKSPACE PATH : %s", m_path.c_str());
	DirectZob::LogWarning("loading scene %s", m_file.c_str());
	UnloadScene();
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	DirectZob::GetInstance()->GetHudManager()->Init();
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
	float x, y, z, fov, znear, zfar;
	std::string name, texture, fullPath;
	TiXmlDocument doc("Scene");
	fullPath = m_path + m_file;
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
		TiXmlElement* scene = doc.FirstChildElement(XML_ELEMENT_SCENE);
		if (scene)
		{
			/*TiXmlElement* textures = root->FirstChildElement("Textures");
			for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
			{
				LoadTexture(e, MaterialManager);
			}*/
			DirectZob::GetInstance()->GetLightManager()->LoadFromNode(scene->FirstChildElement(XML_ELEMENT_GLOBALS));
			DirectZob::GetInstance()->GetEngine()->LoadFromNode(scene->FirstChildElement(XML_ELEMENT_GLOBALS));
			for (TiXmlElement* e = scene->FirstChildElement(XML_ELEMENT_ZOBOBJECT); e != NULL; e = e->NextSiblingElement(XML_ELEMENT_ZOBOBJECT))
			{
				const char* file = e->Attribute(XML_ATTR_FILE);
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
	DirectZob::GetInstance()->GetHudManager()->Start();
	DirectZob::RemoveIndent();
	DirectZob::GetInstance()->OnSceneLoaded();
	DirectZob::LogInfo("Scene loaded");
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
	TiXmlDocument doc(XML_ELEMENT_SCENE);
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);
	TiXmlElement * root = new TiXmlElement(XML_ELEMENT_SCENE);
	doc.LinkEndChild(root);
	TiXmlElement globals = TiXmlElement(XML_ELEMENT_GLOBALS);
	DirectZob::GetInstance()->GetLightManager()->SaveUnderNode(&globals);
	DirectZob::GetInstance()->GetEngine()->SaveUnderNode(&globals);
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
	//delete root;
}

bool SceneLoader::SaveZobObjectRecusrive(TiXmlNode* node, ZobObject* z)
{
	if (ZobEntity::GetType(z->GetIdValue()) == ZobEntity::type_editor)
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

void SceneLoader::CleanPath(std::string& p)
{
	std::replace(p.begin(), p.end(), '\\', '/');
	if (p[p.length() - 1] != '/')
	{
		p.append("/");
	}
}

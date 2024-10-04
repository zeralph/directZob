#include "SceneLoader.h"
#include "DirectZob.h"
#include "ZobPhysic/ZobPhysicsEngine.h"
#include "Managers/LightManager.h"
#include "Managers/ZobHUDManager.h"
#include "Managers/CameraManager.h"
#include "Misc/ZobXmlHelper.h"
#include "../../dependencies/physfs/include/physfs.h"

//TODO !
std::string							SceneLoader::m_path = "";
std::string							SceneLoader::m_file = "";
std::string							SceneLoader::m_nextScenePath = "";
std::string							SceneLoader::m_nextSceneName = "";
int									SceneLoader::m_nbZobObjectLoaded = 0;
char								tmpBuffer[256];
bool								SceneLoader::m_loadNextScene = false;
bool								SceneLoader::m_loadFromArchive = false;
DirectZobType::sceneLoadingCallback	SceneLoader::m_onSceneLoading = NULL;

void SceneLoader::Update()
{
	if (m_loadNextScene)
	{
		m_loadNextScene = false;
		m_path = m_nextScenePath;
		CleanPath(m_path);
		m_file = m_nextSceneName;
		if(m_loadFromArchive)
		{
			LoadSceneInternalFromArchive();
		}
		else
		{
			LoadSceneInternalFromFiles();
		}
		m_nextScenePath = "";
		m_nextSceneName = "";
	}
}

void SceneLoader::LoadPack(std::string &path, std::string &file)
{
	PHYSFS_init(NULL);
	std::string fullPath = path + file;
	int res = PHYSFS_mount(fullPath.c_str(), "", 1);
	m_loadFromArchive = true;
	if (!res)
	{
		DirectZob::LogError("cannot load archive %s", fullPath.c_str());
		exit(-1);
	}
	else
	{
		int res = PHYSFS_exists("0");
		if (res)
		{
			PHYSFS_file* myfile = PHYSFS_openRead("0");
			int fileLength = PHYSFS_fileLength(myfile);
			char* myBuf;
			myBuf = new char[fileLength + 1];
			int length_read = PHYSFS_readBytes(myfile, myBuf, PHYSFS_fileLength(myfile));
			myBuf[fileLength] = '\0';
			m_loadNextScene = true;
			m_nextScenePath = "";
			m_nextSceneName = std::string(myBuf);
		}
		else
		{
			DirectZob::LogError("Entrypoint '0' not found in %s", fullPath.c_str());
			exit(-1);
		}
	}
}

void SceneLoader::LoadScene(std::string& path, std::string& file, DirectZobType::sceneLoadingCallback cb)
{
	m_onSceneLoading = cb;
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
	DirectZob::GetInstance()->GetMeshManager()->LoadMesh(&zfp);
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
	throw("deprecated");
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
		LoadZobObject(n, parent, false);
	}
}

ZobObject* SceneLoader::LoadZobObject(TiXmlElement* node, ZobObject* parent, bool bRegenerateGuid)
{	
	m_nbZobObjectLoaded++;
	ZobObject* zob = NULL;
	if (parent == nullptr)
	{
		ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
		parent = zobObjectManager->GetRootObject();
	}
	const char* type= node->Attribute(XML_ATTR_TYPE);
	const char* cid =node->Attribute(XML_ATTR_GUID);
	if (!cid)
	{
		DirectZob::LogError("Cannot create ZobObject %s", cid);
		return NULL;
	}
	std::string id = cid;
	zobId zid = ZobEntity::ZobIdFromString(id);
	if (bRegenerateGuid)
	{
		zid = ZobEntity::Regenerate(zid);
	}
	if (ZobEntity::GetSubType(zid) == ZobEntity::subtype_zobOject)
	{
		zob = new ZobObject(zid, node, parent);
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
	if (m_onSceneLoading)
	{
		m_onSceneLoading(0, 0, zob->GetName());
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
			LoadZobObject(e, zob, bRegenerateGuid);
		}
	}
	return zob;
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
	//UnloadScene();
	if (workspace.length() > 0)
	{
		m_path = workspace;
	}
	m_file = "";
	ResetEngine();
}

void SceneLoader::ResetEngine()
{
	UnloadScene();
	DirectZob::GetInstance()->GetLightManager()->ReInitGlobalSettings();
	DirectZob::GetInstance()->GetHudManager()->Init();
	MeshManager* meshManager = DirectZob::GetInstance()->GetMeshManager();
	MaterialManager* materialManager = DirectZob::GetInstance()->GetMaterialManager();
	ZobObjectManager* zobObjectManager = DirectZob::GetInstance()->GetZobObjectManager();
}

void SceneLoader::LoadSceneInternalFromArchive()
{
	ResetEngine();
	int res = PHYSFS_exists(m_file.c_str());
	if (res)
	{
		PHYSFS_file* myfile = PHYSFS_openRead(m_file.c_str());
		int fileLngth = PHYSFS_fileLength(myfile);
		char* myBuf;
		myBuf = new char[fileLngth + 1];
		int length_read = PHYSFS_readBytes(myfile, myBuf, PHYSFS_fileLength(myfile));
		myBuf[fileLngth] = '\0';
		PHYSFS_close(myfile);
		TiXmlDocument doc("Scene");
		doc.ClearError();
		//doc.LoadFile()
		doc.Parse(myBuf);
		ParseXml(&doc);
	}
	else
	{
		DirectZob::LogError("cannot find entryPoint %s", m_file.c_str());
		exit(-1);
	}
}

void SceneLoader::LoadSceneInternalFromFiles()
{
	DirectZob::AddIndent();
	DirectZob::LogWarning("WORKSPACE PATH : %s", m_path.c_str());
	DirectZob::LogWarning("loading scene %s", m_file.c_str());
	ResetEngine();
	float x, y, z, fov, znear, zfar;
	std::string name, fullPath;
	TiXmlDocument doc("Scene");
	fullPath = m_path + m_file;
	doc.ClearError();
	doc.LoadFile(fullPath.c_str());
	ParseXml(&doc);
}

void SceneLoader::LoadAsset(ZobObject* parent, std::string& path, std::string& file)
{
	TiXmlDocument doc("Scene");
	std::string fullPath = path + file;
	doc.ClearError();
	doc.LoadFile(fullPath.c_str());
	LoadAsset(parent, (TiXmlElement*)doc.FirstChild());
}

ZobObject* SceneLoader::LoadAsset(ZobObject* parent, TiXmlElement* node)
{
	return LoadZobObject(node, parent, true);
}

const std::string& SceneLoader::GetResourcePath() 
{ 
	return m_path; 
}

void SceneLoader::ParseXml(TiXmlDocument* doc)
{
	int nbEntities = 0;
	int nbObjects = 0;
	if (doc->Error())
	{
		DirectZob::LogError("Error loading %s", doc->ErrorDesc());
		m_path = "";
		m_file = "";
	}
	else
	{
		DirectZob::LogInfo("loading objects");
		TiXmlElement* scene = doc->FirstChildElement(XML_ELEMENT_SCENE);
		if (scene)
		{
			/*TiXmlElement* textures = root->FirstChildElement("Textures");
			for (TiXmlElement* e = textures->FirstChildElement("Texture"); e != NULL; e = e->NextSiblingElement("Texture"))
			{
				LoadTexture(e, MaterialManager);
			}*/
			DirectZob::GetInstance()->GetLightManager()->LoadFromNode(scene->FirstChildElement(XML_ELEMENT_GLOBALS));
			DirectZob::GetInstance()->GetEngine()->LoadFromNode(scene->FirstChildElement(XML_ELEMENT_GLOBALS));
			nbEntities = scene->Attribute(XML_NUMBER_OF_ENTITIES) ? atoi(scene->Attribute(XML_NUMBER_OF_ENTITIES)) : 0;
			nbObjects = scene->Attribute(XML_NUMBER_OF_OBJECTS) ? atoi(scene->Attribute(XML_NUMBER_OF_OBJECTS)) : 0;
			if (m_onSceneLoading)
			{
				m_onSceneLoading(nbObjects, 0, "");
			}
			for (TiXmlElement* e = scene->FirstChildElement(XML_ELEMENT_ZOBOBJECT); e != NULL; e = e->NextSiblingElement(XML_ELEMENT_ZOBOBJECT))
			{
				//curObject++;
				const char* file = e->Attribute(XML_ATTR_FILE);
				if (file)
				{
					std::string f = std::string(file);
					LoadZobObject(f);
				}
				else
				{
					LoadZobObject(e, NULL, false);
				}
			}
			DirectZob::LogInfo("Scene loaded");
		}
		else
		{
			DirectZob::LogError("Scene file has error");
		}
	}
	if (m_onSceneLoading)
	{
		m_onSceneLoading(nbEntities + 1, 0, "");
	}
	DirectZob::GetInstance()->GetEngine()->Start(m_onSceneLoading);
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

	ZobPhysicsEngine* zp = DirectZob::GetInstance()->GetPhysicsEngine();
	std::string a;
	std::string b;
	if (zp->HasRigiBodyCollision(a,b))
	{
		DirectZob::LogError("Dynamic rigidbody of '%s' is in collision with '%s'.\nRigid bodies cannot collide with an object in a save.\n Saving abort", a.c_str(), b.c_str());
		return;
	}

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
	root->SetAttribute(XML_NUMBER_OF_ENTITIES, ZobEntity::GetAllEntities().size());
	root->SetAttribute(XML_NUMBER_OF_OBJECTS, ZobEntity::GetEntities<ZobObject>().size());
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

#pragma once
#include <string>
#include <vector>
#include "Managers/ZobObjectManager.h"
#include "ZobObjects/ZobObject.h"
#include "Managers/MaterialManager.h"
#include "Managers/MeshManager.h"
#include "ZobMaterial.h"
#include "Mesh.h"
#include "Rendering/Engine.h"
#include "tinyxml.h"
#include "Types.h"

class SceneLoader
{
public:
	
	static void							LoadPack(std::string& path, std::string& file);
	static void							SetWorkSpace(std::string path) { m_path = path; }
	static void							LoadScene(std::string &path, std::string &file, DirectZobType::sceneLoadingCallback cb);
	static void							SaveScene(std::string &path, std::string &file);
	static void							SaveScene();
	static bool							CanFastSave() { return m_path.length() > 0 && m_file.length() > 0; }
	static void							NewScene(std::string workspace);
	static void							UnloadScene();
	static void							Update();
	static const std::string&			GetResourcePath();
	static void							LoadZobObject(std::string& path, std::string& file);
	static void							LoadZobObject(std::string& fullPath, ZobObject* parent = NULL);
	static const int					GetNbZobObjectsInScene() { return m_nbZobObjectLoaded; }
	static const bool					LoadFromArchive() { return m_loadFromArchive; }
private:
	static void							ResetEngine();
	static void							LoadSceneInternalFromArchive();
	static void							LoadSceneInternalFromFiles();
	static void							ParseXml(TiXmlDocument* doc);
	static void							LoadMesh(TiXmlElement* node);
	static void							CleanPath(std::string &path);
	static void							LoadZobObject(TiXmlElement* node, ZobObject* parent, const std::string* factoryPath);
	static bool							SaveZobObjectRecusrive(TiXmlNode* node, ZobObject* z);
	static std::string					m_path;
	static std::string					m_file;
	static std::vector<Mesh*>			m_meshes;
	static std::string					m_nextScenePath;
	static std::string					m_nextSceneName;
	static bool							m_loadNextScene;
	static int							m_nbZobObjectLoaded;
	static bool							m_loadFromArchive;
	static DirectZobType::sceneLoadingCallback			m_onSceneLoading;
};

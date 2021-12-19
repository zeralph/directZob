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
	static void SetWorkSpace(std::string path) { m_path = path; }
	static void LoadScene(std::string &path, std::string &file);
	static void SaveScene(std::string &path, std::string &file);
	static void SaveScene();
	static bool CanFastSave() { return m_path.length() > 0 && m_file.length() > 0; }
	static void NewScene(std::string workspace);
	static void UnloadScene();
	static void Update();
	static const std::string& GetResourcePath() { return m_path; };
	static void LoadZobObject(std::string& path, std::string& file);
	static void LoadZobObject(std::string& fullPath, ZobObject* parent = NULL);
	static const int GetNbZobObjectsInScene() { return m_nbZobObjectLoaded; }
private:
	static void LoadSceneInternal();
	static void LoadMesh(TiXmlElement* node);
	static void CleanPath(std::string &path);
	static void LoadZobObject(TiXmlElement* node, ZobObject* parent, const std::string* factoryPath);
	static bool SaveZobObjectRecusrive(TiXmlNode* node, ZobObject* z);
	static std::string m_path;
	static std::string m_file;
	static std::vector<Mesh*> m_meshes;
	static std::string m_nextScenePath;
	static std::string m_nextSceneName;
	static bool m_loadNextScene;
	static int m_nbZobObjectLoaded;
};

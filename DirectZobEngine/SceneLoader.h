#pragma once
#include <string>
#include <vector>
#include "ZobObjectManager.h"
#include "ZobObject.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Engine.h"
#include "tinyxml.h"


class SceneLoader
{
public:
	static void LoadScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* materialManager);
	static void SaveScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* materialManager);
	static void SaveScene(ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* materialManager);
	static bool CanFastSave() { return m_path.length() > 0 && m_file.length() > 0; }
	static void NewScene(Core::Engine* engine, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* materialManager);
private:

	static void LoadMesh(TiXmlElement* node, MeshManager* meshMgr, MaterialManager* texMgr);
	static void LoadZobObject(TiXmlElement* node, ZobObject* parent, ZobObjectManager* zobMgr, MeshManager* meshMgr);

	static void SaveZobObjectRecusrive(TiXmlElement* node, ZobObject* z);
	static void UnloadScene(Core::Engine* engine, ZobObjectManager* zobObjectManager, MeshManager* meshManager, MaterialManager* materialManager);

	static std::string m_path;
	static std::string m_file;
	static std::vector<Mesh*> m_meshes;
};

#pragma once
#include <string>
#include <vector>
#include "ZobObjectManager.h"
#include "ZobObject.h"
#include "MaterialManager.h"
#include "MeshManager.h"
#include "ZobMaterial.h"
#include "Mesh.h"
#include "Engine.h"
#include "tinyxml.h"
#include "Types.h"

class SceneLoader
{
public:
	static void LoadScene(std::string &path, std::string &file);
	static void SaveScene(std::string &path, std::string &file);
	static void SaveScene();
	static bool CanFastSave() { return m_path.length() > 0 && m_file.length() > 0; }
	static void NewScene();
	static void UnloadScene();
	static const std::string& GetResourcePath() { return m_path; };

private:

	static void LoadMesh(TiXmlElement* node);
	static void LoadZobObject(TiXmlElement* node, ZobObject* parent);
	static void SaveZobObjectRecusrive(TiXmlNode* node, ZobObject* z);
	static std::string m_path;
	static std::string m_file;
	static std::vector<Mesh*> m_meshes;
};

#pragma once
#include <string>
#include <vector>
#include "ZobObjectManager.h"
#include "ZobObject.h"
#include "MaterialManager.h"
#include "LightManager.h"
#include "MeshManager.h"
#include "Material.h"
#include "Mesh.h"
#include "Engine.h"
#include "tinyxml.h"


static class SceneLoader
{
public:
	static void LoadScene(std::string &path, std::string &file);
	static void SaveScene(std::string &path, std::string &file);
	static void SaveScene();
	static bool CanFastSave() { return m_path.length() > 0 && m_file.length() > 0; }
	static void NewScene();
private:

	static void LoadMesh(TiXmlElement* node);
	static void LoadZobObject(TiXmlElement* node, ZobObject* parent);
	static void SaveZobObjectRecusrive(TiXmlElement* node, ZobObject* z);
	static void UnloadScene();
	static void LoadGlobals(TiXmlElement* node);
	static std::string m_path;
	static std::string m_file;
	static std::vector<Mesh*> m_meshes;
};
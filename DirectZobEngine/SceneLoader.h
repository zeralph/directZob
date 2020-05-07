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
	static void LoadScene(std::string &path, std::string &file);
	static void SaveScene(std::string &path, std::string &file);
	static void SaveScene();
	static bool CanFastSave() { return m_path.length() > 0 && m_file.length() > 0; }
	static void NewScene();
	static void UnloadScene();
private:
	static Vector3 Vector2Color(Vector3* v) { return Vector3((int)(v->x * 255.0f), (int)(v->y * 255.0f), (int)(v->z * 255.0f)); };
	static Vector3 Color2Vector(Vector3* v) { return Vector3(v->x / 255.0f, v->y / 255.0f, v->z / 255.0f); };
	static void LoadMesh(TiXmlElement* node);
	static void LoadZobObject(TiXmlElement* node, ZobObject* parent);
	static void SaveZobObjectRecusrive(TiXmlElement* node, ZobObject* z);
	static void SaveGlobals(TiXmlElement* node);
	static void LoadGlobals(TiXmlElement* node);
	static std::string m_path;
	static std::string m_file;
	static std::vector<Mesh*> m_meshes;
};

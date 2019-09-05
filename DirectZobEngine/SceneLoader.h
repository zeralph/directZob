#pragma once
#include <string>
#include <vector>
#include "ZobObjectManager.h"
#include "ZobObject.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "tinyxml.h"

static class SceneLoader
{
public:
	static void LoadScene(std::string &path, std::string &file, ZobObjectManager* zobObjectManager, TextureManager* textureManager);

private:

	static void LoadTexture(TiXmlElement* node, TextureManager* texMgr);
	static void LoadMesh(TiXmlElement* node, TextureManager* texMgr);
	static void LoadZobObject(TiXmlElement* node, ZobObject* parent, ZobObjectManager* zobMgr);
	static Mesh* GetMesh(std::string& mesh);

	static std::string m_path;
	static std::string m_file;
	static std::vector<Mesh*> m_meshes;
};
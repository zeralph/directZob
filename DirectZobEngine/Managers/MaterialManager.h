#pragma once
#include "../Types.h"
#include "../ZobMaterial.h"
#undef PropertyNotify
#include <fbxsdk.h>
#include <string>
#include <vector>
#include "../Rendering/Texture.h"

class MaterialManager
{
public:

	MaterialManager();
	~MaterialManager();

	const ZobMaterial* 		LoadMaterial(const std::string& name, const ZobVector3* ambientColor, const ZobVector3* diffuseColor, const std::string &textureFile);
	const ZobMaterial*		LoadMaterial(const std::string& name, const ZobVector3* ambientColor, const ZobVector3* diffuseColor, Texture* texture);
	const ZobMaterial* 		GetMaterial(const std::string& name) const;
	const int 				GetNbTextures() const { return (int)m_materials.size(); }
	const ZobMaterial* 		GetMaterial(const int i) const;
	void 					LoadOBJMaterials(std::string& path, std::string& file);
	const ZobMaterial* 		LoadFbxMaterial(const fbxsdk::FbxMesh* mesh, const std::string &path);
	void 					UnloadAll();
	ZobMaterial*			CreateMaterial();
	const Texture*			GetTexture(const std::string name);
private:
	const Texture*			LoadTexture(const std::string name);
	void 					SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	std::vector<ZobMaterial*> 	m_materials;
	std::vector<Texture*> 	m_textures;
};
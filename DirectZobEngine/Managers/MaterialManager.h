#pragma once
#undef PropertyNotify
#include "../Types.h"
#ifdef ENABLE_FBX_SUPPORT
#include <fbxsdk.h>
#endif
#include <string>
#include <vector>
#include "../Misc/ZobFilePath.h"

class Texture;
class ZobMaterial;
class ZobColor;
class MaterialManager
{
public:

	MaterialManager();
	~MaterialManager();

	const ZobMaterial* 			LoadMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor,
											float specularExponent, float transparency, ZobFilePath* zfp);
	const ZobMaterial*			LoadMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor,
											float specularExponent, float transparency, Texture* texture);
	const ZobMaterial* 			GetMaterial(const std::string& name) const;
	const int 					GetNbMaterials() const { return (int)m_materials.size(); }
	const ZobMaterial* 			GetMaterial(const int i) const;
	ZobMaterial*				GetMaterialNoConst(const int i) const;
	void 						LoadOBJMaterials(ZobFilePath* zfp);
#ifdef ENABLE_FBX_SUPPORT
	const ZobMaterial* 			LoadFbxMaterial(const fbxsdk::FbxMesh* mesh, ZobFilePath* zfp);
#endif
	void 						UnloadAll();
	ZobMaterial*				CreateMaterial();
	const Texture*				GetTexture(const std::string name);
private:
	const Texture*				LoadTexture(ZobFilePath* zfp);
	void 						SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	std::vector<ZobMaterial*> 	m_materials;
	std::vector<Texture*> 		m_textures;
};
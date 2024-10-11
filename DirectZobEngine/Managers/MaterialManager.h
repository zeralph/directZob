#pragma once
#undef PropertyNotify
#include "../Types.h"
#ifdef ENABLE_FBX_SUPPORT
#include <fbxsdk.h>
#endif
#include <string>
#include <vector>
#include "../Misc/ZobFilePath.h"
#include "../../dependencies/tinygltf/include/tiny_gltf.h"

class ZobTexture;
class ZobMaterial;
class ZobColor;
//class tinygltf::Material;
class MaterialManager
{
public:

	MaterialManager();
	~MaterialManager();

	const ZobMaterial* 			LoadMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor,
											float specularExponent, ZobFilePath* zfp);
	const ZobMaterial*			LoadMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor,
											float specularExponent, ZobTexture* texture);
	const ZobMaterial*			LoadGlTFMaterial(tinygltf::Model& model, int index);
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
	const ZobTexture*				GetTexture(const std::string name);
private:
	const ZobTexture*				LoadTexture(ZobFilePath* zfp);
	void 						SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	std::vector<ZobMaterial*> 	m_materials;
	std::vector<ZobTexture*> 		m_textures;
};
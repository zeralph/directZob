#pragma once
#include "Types.h"
#include "Material.h"
#include <string>
#include <vector>

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void LoadMaterial(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const std::string* textureFile = NULL);
	const Material* GetTexture(const std::string& name) const;
	const int GetNbTextures() const { return m_textures.size(); }
	const Material* GetTexture(const int i) const;
	void LoadMaterials(std::string& path, std::string& file);
	void UnloadAll();
private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	std::vector<Material*> m_textures;
};
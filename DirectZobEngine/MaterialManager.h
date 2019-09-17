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

	void LoadTexture(std::string& name, std::string& path, std::string& file);
	const Material* GetTexture(std::string& name) const;
	const int GetNbTextures() const { return m_textures.size(); }
	const Material* GetTexture(const int i) const;
	void LoadMaterials(std::string& path, std::string& file);
	void UnloadAll();
private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	std::vector<Material*> m_textures;
};
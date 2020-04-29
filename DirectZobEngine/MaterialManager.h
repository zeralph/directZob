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

	const Material* LoadMaterial(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const std::string &textureFile);
	const Material* GetMaterial(const std::string& name) const;
	const int GetNbTextures() const { return (int)m_materials.size(); }
	const Material* GetMaterial(const int i) const;
	void LoadOBJMaterials(std::string& path, std::string& file);
	void UnloadAll();
private:
	void SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim);
	std::vector<Material*> m_materials;
};
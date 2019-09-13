#pragma once
#include "Types.h"
#include "Texture.h"
#include <string>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void LoadTexture(std::string& name, std::string& path, std::string& file);
	const Texture* GetTexture(std::string& name) const;
	const int GetNbTextures() const { return m_textures.size(); }
	const Texture* GetTexture(const int i) const;
	void LoadMaterial(std::string& path, std::string& file);
	void UnloadAll();
private:
	std::vector<Texture*> m_textures;
};
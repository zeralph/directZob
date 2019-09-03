#pragma once
#include "Types.h"
#include "Texture.h"
#include <string>

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void LoadTexture(std::string& name, std::string& file);
	const Texture* GetTexture(std::string& name) const;

private:
	std::vector<Texture*> m_textures;
};
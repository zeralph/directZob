#include "TextureManager.h"

TextureManager::TextureManager()
{
	m_textures.clear();
}

TextureManager::~TextureManager()
{
	for (int i = 0; i < m_textures.size(); i++)
	{
		delete (m_textures[i]);
	}
	m_textures.clear();
}

void TextureManager::LoadTexture(std::string& name, std::string& path, std::string& file)
{
	if (GetTexture(name) == NULL)
	{
		Texture* t = new Texture(path, file, name);
		m_textures.push_back(t);
	}
	else
	{
		//output error
	}
}

const Texture* TextureManager::GetTexture(const int i) const
{
	if (i >= 0 && i < m_textures.size())
	{
		return m_textures[i];
	}
	return NULL;
}

const Texture* TextureManager::GetTexture(std::string& name) const
{
	for (std::vector<Texture*>::const_iterator iter = m_textures.begin(); iter != m_textures.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}


void TextureManager::UnloadAll()
{
	for (int i = 0; i < m_textures.size(); i++)
	{
		delete (m_textures[i]);
	}
	m_textures.clear();
}
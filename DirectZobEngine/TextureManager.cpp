#include "TextureManager.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "DirectZob.h"

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

void TextureManager::LoadMaterial(std::string& path, std::string& file)
{
	std::string::size_type sz;
	// Open the file.
	std::string fullPath = path;
	fullPath.append(file);
	std::ifstream sfile(fullPath, ios::in);
	std::string line;
	std::string mat;
	if (!sfile.is_open())
	{
		std::string s = "ERROR";
		DirectZob::Log(s);
		return;
	}
	while (getline(sfile, line))
	{
		if (line.rfind("newmtl", 0) == 0)
		{
			std::vector<std::string> v;
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				mat = v[1];
			}
		}
		else if (line.rfind("map_Kd", 0) == 0)
		{
			std::vector<std::string> v;
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				LoadTexture(mat, path, v[1]);
			}
		}
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

void TextureManager::SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim)
{
	std::size_t current, previous = 0;
	current = s->find(delim);
	while (current != std::string::npos) {
		v->push_back(s->substr(previous, current - previous));
		previous = current + 1;
		current = s->find(delim, previous);
	}
	v->push_back(s->substr(previous, current - previous));
}
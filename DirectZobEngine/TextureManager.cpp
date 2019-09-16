#include "TextureManager.h"
#include "DirectZob.h"
#include <fstream>
#include <iostream>

struct MaterialInfo
{
	std::string file;
	std::string name;
	std::string texture;
	//put other stuff here
};

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

void TextureManager::LoadMaterials(std::string& path, std::string& file)
{
	std::string::size_type sz;
	// Open the file.
	std::string fullPath = path;
	fullPath.append(file);
	std::ifstream sfile(fullPath, std::ios::in);
	std::string line;
	std::vector<MaterialInfo> materials;
	if (!sfile.is_open())
	{
		std::string s = "ERROR opening ";
		s.append(fullPath);
		DirectZob::LogError(s.c_str());
		return;
	}
	std::vector<std::string> v;
	while (getline(sfile, line))
	{
		if (line.rfind("newmtl", 0) == 0)
		{
			v.clear();
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				MaterialInfo matInfo;
				matInfo.name = v[1];
				matInfo.file = file.substr(0, file.size() - 4);;
				v.clear();
				while(getline(sfile, line))
				{
					if (line.rfind("map_Kd", 0) == 0)
					{
						SplitEntry(&line, &v, ' ');
						if (v.size() == 2)
						{
							matInfo.texture = v[1];
							materials.push_back(matInfo);
							break;
						}
					}
				}

			}
		}
	}
	for (int i = 0; i < materials.size(); i++)
	{
		std::string n = materials.at(i).file;
		n.append(".");
		n.append(materials.at(i).name);
		LoadTexture(n, path, materials.at(i).texture);
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
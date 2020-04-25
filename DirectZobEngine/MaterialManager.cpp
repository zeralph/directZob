#include "materialManager.h"
#include "DirectZob.h"
#include <fstream>
#include <iostream>
#include "Vector3.h"

struct MaterialInfo
{
	std::string file;
	std::string name;
	std::string texture;
	Vector3 diffuse;
	Vector3 ambient;
	//put other stuff here
};

MaterialManager::MaterialManager()
{
	m_materials.clear();
}

MaterialManager::~MaterialManager()
{
	for (int i = 0; i < m_materials.size(); i++)
	{
		delete (m_materials[i]);
	}
	m_materials.clear();
}

const Material* MaterialManager::LoadMaterial(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const char* textureFile/* = NULL*/)
{
	if (GetMaterial(name) == NULL)
	{
		Material* t = new Material(name, ambientColor, diffuseColor, textureFile);
		m_materials.push_back(t);
		return t;
	}
	else
	{
		//output error
		return GetMaterial(name);
	}
}

void MaterialManager::LoadMaterials(std::string& path, std::string& file)
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
		DirectZob::LogError("Error opening %s", fullPath.c_str());
		return;
	}
	std::string s = "Opening ";
	s.append(fullPath);
	DirectZob::LogInfo(s.c_str());
	std::vector<std::string> v;
	bool bJumpRead = false;
	while (bJumpRead || getline(sfile, line))
	{
		bJumpRead = false;
		if (line.rfind("newmtl", 0) == 0)
		{
			v.clear();
			SplitEntry(&line, &v, ' ');
			if (v.size() == 2)
			{
				MaterialInfo matInfo;
				matInfo.texture = "";
				matInfo.name = v[1];
				matInfo.file = file.substr(0, file.size() - 4);
				while(getline(sfile, line))
				{
					if (line.rfind("map_Kd", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 2)
						{
							matInfo.texture = v[1];
						}
					}
					if (line.rfind("Ka", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 4)
						{
							matInfo.ambient.x = (float)atof(v[1].c_str());
							matInfo.ambient.y = (float)atof(v[2].c_str());
							matInfo.ambient.z = (float)atof(v[3].c_str());
						}
					}
					if (line.rfind("Kd", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 4)
						{
							matInfo.diffuse.x = (float)atof(v[1].c_str());
							matInfo.diffuse.y = (float)atof(v[2].c_str());
							matInfo.diffuse.z = (float)atof(v[3].c_str());
						}
					}
					if (line.rfind("newmtl", 0) == 0)
					{
						bJumpRead = true;
						break;
					}
				}
				materials.push_back(matInfo);
			}
		}
	}
	for (int i = 0; i < materials.size(); i++)
	{
		std::string n = materials.at(i).file;
		n.append(".");
		n.append(materials.at(i).name);

		if (materials.at(i).texture.length() > 0)
		{
			std::string p = path;
			p.append(materials.at(i).texture);
			LoadMaterial(n, &materials.at(i).ambient, &materials.at(i).diffuse, p.c_str());
		}
		else
		{
			LoadMaterial(n, &materials.at(i).ambient, &materials.at(i).diffuse, NULL);
		}
	}
}

const Material* MaterialManager::GetMaterial(const int i) const
{
	if (i >= 0 && i < m_materials.size())
	{
		return m_materials[i];
	}
	return NULL;
}

const Material* MaterialManager::GetMaterial(const std::string& name) const
{
	for (std::vector<Material*>::const_iterator iter = m_materials.begin(); iter != m_materials.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}


void MaterialManager::UnloadAll()
{
	for (int i = 0; i < m_materials.size(); i++)
	{
		delete (m_materials[i]);
		m_materials[i] = NULL;
	}
	m_materials.clear();
}

void MaterialManager::SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim)
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
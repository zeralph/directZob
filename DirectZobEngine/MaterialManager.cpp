#include "MaterialManager.h"
#include "DirectZob.h"
#include <fstream>
#include <iostream>
#include "ZobVector3.h"

struct OBJMaterialInfo
{
	std::string file;
	std::string name;
	std::string texture;
	ZobVector3 diffuse;
	ZobVector3 ambient;
	//put other stuff here
};

MaterialManager::MaterialManager()
{
	m_materials.clear();
	m_textures.clear();
}

MaterialManager::~MaterialManager()
{
	UnloadAll();
}

ZobMaterial* MaterialManager::CreateMaterial()
{
	ZobMaterial* m = new ZobMaterial();
	m_materials.push_back(m);
	return m;
}

const ZobMaterial* MaterialManager::LoadMaterial(const std::string& name, const ZobVector3* ambientColor, const ZobVector3* diffuseColor, const std::string &textureFile)
{
	if (GetMaterial(name) == NULL)
	{
		const Texture* texture = NULL;
		if (textureFile.length() > 0)
		{
			for (std::vector<Texture*>::const_iterator iter = m_textures.begin(); iter != m_textures.end(); iter++)
			{
				if ((*iter)->GetPath() == textureFile)
				{
					texture = (*iter);
					break;
				}
			}
			if(texture == NULL)
			{
				texture = LoadTexture(textureFile);
			}
		}
		ZobMaterial* t = new ZobMaterial(name, ambientColor, diffuseColor, texture);
		m_materials.push_back(t);
		return t;
	}
	else
	{
		//output error
		return GetMaterial(name);
	}
}

const ZobMaterial* MaterialManager::LoadFbxMaterial(const fbxsdk::FbxMesh* mesh, const std::string& path)
{
	const ZobMaterial* finalMaterial = NULL;
	MaterialManager* materialMgr = DirectZob::GetInstance()->GetMaterialManager();
	int mcount = mesh->GetElementMaterialCount();
	for (int index = 0; index < mcount; index++)
	{
		//fbxsdk::FbxGeometryElementMaterial* material = mesh->GetElementMaterial(0);
		const fbxsdk::FbxGeometryElementMaterial* gMaterial = mesh->GetElementMaterial(index);
		if (gMaterial)
		{
			fbxsdk::FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(gMaterial->GetIndexArray().GetAt(0));
			int propCount = material->GetSrcPropertyCount();
			//if (propCount > 0)
			{
				const char* matName = material->GetName();
				finalMaterial = materialMgr->GetMaterial(matName);
				if (finalMaterial)
				{
					return finalMaterial;
				}
				else
				{
					// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
					ZobVector3 diffuse = ZobVector3(0, 0, 0);
					ZobVector3 ambient = ZobVector3(0, 0, 0);
					fbxsdk::FbxProperty prop;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
					FbxDouble3 d;
					if (prop.IsValid())
					{
						d = prop.Get<FbxDouble3>();
						diffuse = ZobVector3(d[0], d[1], d[2]);
					}
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sAmbient);
					if (prop.IsValid())
					{
						d = prop.Get<FbxDouble3>();
						ambient = ZobVector3(d[0], d[1], d[2]);
					}
					const char* texture_name = NULL;
					const char* texture_name2 = NULL;
					const char* texture_name3 = NULL;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
					int file_texture_count = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();
					if (file_texture_count > 0)
					{
						for (int j = 0; j < file_texture_count; j++)
						{
							FbxFileTexture* file_texture = FbxCast<fbxsdk::FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
							texture_name = file_texture->GetFileName();
							texture_name2 = file_texture->GetRelativeFileName();
							texture_name3 = file_texture->GetMediaName();
						}
					}
					float f;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);
					f = prop.Get<FbxDouble>();

					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sSpecularFactor);
					f = prop.Get<FbxDouble>();
					std::string texFullPath = "";
					if (texture_name2)
					{
						std::string t = std::string(texture_name2);
						size_t i = t.rfind('\\');
						if (i >= 0)
						{
							t = t.substr(i+1);
						}
						texFullPath = path + std::string(t);
					}
					finalMaterial = LoadMaterial(matName, &ambient, &diffuse, texFullPath);
				}
			}
		}
	}
	return finalMaterial;
}


void MaterialManager::LoadOBJMaterials(std::string& path, std::string& file)
{
	std::string::size_type sz;
	// Open the file.
	std::string fullPath = path;
	fullPath.append(file);
	std::ifstream sfile(fullPath, std::ios::in);
	std::string line;
	std::vector<OBJMaterialInfo> materials;
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
				OBJMaterialInfo matInfo;
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
			LoadMaterial(n, &materials.at(i).ambient, &materials.at(i).diffuse, p);
		}
		else
		{
			LoadMaterial(n, &materials.at(i).ambient, &materials.at(i).diffuse, "");
		}
	}
}

const ZobMaterial* MaterialManager::GetMaterial(const int i) const
{
	if (i >= 0 && i < m_materials.size())
	{
		return m_materials[i];
	}
	return NULL;
}

const Texture* MaterialManager::GetTexture(const std::string name)
{
	for(int i=0; i<m_textures.size(); i++)
	{
		if (m_textures[i]->GetPath() == name)
		{
			return m_textures[i];
		}
	}
	return LoadTexture(name);
}

const Texture* MaterialManager::LoadTexture(const std::string name)
{
	Texture* texture = new Texture();
	texture->LoadFromFile(name);
	m_textures.push_back(texture);
	return texture;
}

const ZobMaterial* MaterialManager::GetMaterial(const std::string& name) const
{
	for (std::vector<ZobMaterial*>::const_iterator iter = m_materials.begin(); iter != m_materials.end(); iter++)
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
		ZobMaterial* m = m_materials[i];
		m_materials[i] = NULL;
		delete m;
	}
	m_materials.clear();
	for (int i = 0; i < m_textures.size(); i++)
	{
		Texture* t = m_textures[i];
		m_textures[i] = NULL;
		delete t;
	}
	m_textures.clear();
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

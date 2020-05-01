#include "MaterialManager.h"
#include "DirectZob.h"
#include <fstream>
#include <iostream>
#include "Vector3.h"

struct OBJMaterialInfo
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
	m_textures.clear();
}

MaterialManager::~MaterialManager()
{
	UnloadAll();
}

const Material* MaterialManager::LoadMaterial(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const std::string &textureFile)
{
	if (GetMaterial(name) == NULL)
	{
		Texture* texture = NULL;
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
				texture = new Texture();
				texture->LoadFromFile(textureFile);
				m_textures.push_back(texture);
			}
		}
		Material* t = new Material(name, ambientColor, diffuseColor, texture);
		m_materials.push_back(t);
		return t;
	}
	else
	{
		//output error
		return GetMaterial(name);
	}
}

const Material* MaterialManager::LoadFbxMaterial(const fbxsdk::FbxMesh* mesh, const std::string& path)
{
	const Material* finalMaterial = NULL;
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
					fbxsdk::FbxProperty prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
					// Check if it's layeredtextures
					FbxDouble3 c = prop.Get<FbxDouble3>();
					Vector3 diffuse = Vector3(c[0], c[1], c[2]);
					const char* texture_name = NULL;
					const char* texture_name2 = NULL;
					const char* texture_name3 = NULL;
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
					/*int layered_texture_count = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();
					if (layered_texture_count > 0)
					{
						for (int j = 0; j < layered_texture_count; j++)
						{
							FbxLayeredTexture* layered_texture = FbxCast<fbxsdk::FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
							int lcount = layered_texture->GetSrcObjectCount<fbxsdk::FbxTexture>();
							for (int k = 0; k < lcount; k++)
							{
								FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(layered_texture->GetSrcObject<fbxsdk::FbxTexture>(k));
								// Then, you can get all the properties of the texture, include its name
								texture_name = texture->GetName();
								//texture_name2 = texture->GetRelativeFileName();
								//texture_name3 = texture->GetMediaName();
							}
						}
					}
					else
					{
						// Directly get textures
						int texture_count = prop.GetSrcObjectCount<fbxsdk::FbxTexture>();
						for (int j = 0; j < texture_count; j++)
						{
							const fbxsdk::FbxTexture* texture = FbxCast<fbxsdk::FbxTexture>(prop.GetSrcObject<fbxsdk::FbxTexture>(j));
							//texture->FindSrcObject
							// Then, you can get all the properties of the texture, include its name
							texture_name = texture->GetName();
							//texture_name2 = texture->GetRelativeFileName();
							//texture_name3 = texture->GetMediaName();
						}
					}*/
					float f;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuseFactor);
					f = prop.Get<FbxDouble>();

					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sSpecularFactor);
					f = prop.Get<FbxDouble>();
					Vector3 ambient;
					std::string texFullPath = "";
					if (texture_name2)
					{
						texFullPath = path + std::string(texture_name2);
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
	for (int i = 0; i < m_textures.size(); i++)
	{
		delete (m_textures[i]);
		m_textures[i] = NULL;
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

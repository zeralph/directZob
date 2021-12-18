#include "MaterialManager.h"
#include "DirectZob.h"
#include <fstream>
#include <iostream>
#include "Rendering/ZobVector3.h"
#include <SceneLoader.h>

struct OBJMaterialInfo
{
	std::string file;
	std::string name;
	std::string texture;
	ZobColor diffuse;
	ZobColor ambient;
	ZobColor specular;
	float transparency;
	float specularExponent;
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

const ZobMaterial* MaterialManager::LoadMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor,
												float specularExponent, float transparency, Texture* texture)
{
	m_textures.push_back(texture);
	if (GetMaterial(name) == NULL)
	{
		ZobMaterial* t = new ZobMaterial(name, ambientColor, diffuseColor, specularColor, specularExponent, transparency, texture);
		m_materials.push_back(t);
		return t;
	}
	else
	{
		//output error
		return GetMaterial(name);
	}
}

const ZobMaterial* MaterialManager::LoadMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor,
												float specularExponent, float transparency, ZobFilePath zfp)
{
	//assert(zfp.IsDefined());
	if (GetMaterial(name) == NULL)
	{
		const Texture* texture = NULL;
		if (zfp.IsDefined())
		{
			std::string textureFile = zfp.GetFullPath();
			for (std::vector<Texture*>::const_iterator iter = m_textures.begin(); iter != m_textures.end(); iter++)
			{
				if ((*iter)->GetPath() == textureFile)
				{
					texture = (*iter);
					break;
				}
			}
			if (texture == NULL)
			{
				texture = LoadTexture(textureFile);
			}
		}
		ZobMaterial* t = new ZobMaterial(name, ambientColor, diffuseColor, specularColor, specularExponent, transparency, texture);
		m_materials.push_back(t);
		return t;
	}
	else
	{
		//output error
		return GetMaterial(name);
	}
}

const ZobMaterial* MaterialManager::LoadFbxMaterial(const fbxsdk::FbxMesh* mesh, ZobFilePath zfp)
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
					ZobColor diffuse = &ZobColor::White;
					ZobColor ambient = &ZobColor::White;
					ZobColor specular = &ZobColor::White;
					float specularExponent = 0;
					float transparency = 1;
					fbxsdk::FbxProperty prop;
					FbxDouble3 d;
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sDiffuse);
					if (prop.IsValid())
					{
						d = prop.Get<FbxDouble3>();
						diffuse = ZobColor(255, (int)(d[0]*255.0f), (int)(d[1] * 255.0f), (int)(d[2] * 255.0f));
					}
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sAmbient);
					if (prop.IsValid())
					{
						d = prop.Get<FbxDouble3>();
						ambient = ZobColor(255, (int)(d[0] * 255.0f), (int)(d[1] * 255.0f), (int)(d[2] * 255.0f));
					}
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sSpecular);
					if (prop.IsValid())
					{
						d = prop.Get<FbxDouble3>();
						specular = ZobColor(255, (int)(d[0] * 255.0f), (int)(d[1] * 255.0f), (int)(d[2] * 255.0f));
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
					if (prop.IsValid())
					{
						f = prop.Get<FbxDouble>();
					}
					prop = material->FindProperty(fbxsdk::FbxSurfaceMaterial::sShininess);
					if (prop.IsValid())
					{
						specularExponent = (float)prop.Get<FbxDouble>();
					}
					std::string texFullPath = "";
					ZobFilePath zfpMaterial;
					if (texture_name2)
					{
						std::string t = std::string(texture_name2);
						size_t i = t.rfind('\\');
						if (i >= 0)
						{
							t = t.substr(i+1);
						}
						zfpMaterial = ZobFilePath(t, zfp.GetFullPathWithoutFile(), t, zfp.IsAbsolute());
					}
						
					finalMaterial = LoadMaterial(matName, &ambient, &diffuse, &specular, specularExponent, transparency, zfpMaterial);
				}
			}
		}
	}
	return finalMaterial;
}


void MaterialManager::LoadOBJMaterials(ZobFilePath zfp)
{
	std::string::size_type sz;
	std::string fullPath = zfp.GetFullPath();
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
				matInfo.transparency = 1.0f;
				matInfo.texture = "";
				matInfo.name = v[1];
				matInfo.file = zfp.GetName();
				matInfo.file = matInfo.file.substr(0, matInfo.file.size() - 4);
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
							float r = (float)atof(v[1].c_str());
							float g = (float)atof(v[2].c_str());
							float b = (float)atof(v[3].c_str());
							matInfo.ambient.Set(255, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f));
						}
					}
					if (line.rfind("Ks", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 4)
						{
							float r = (float)atof(v[1].c_str());
							float g = (float)atof(v[2].c_str());
							float b = (float)atof(v[3].c_str());
							matInfo.specular.Set(255, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f));
						}
					}
					if (line.rfind("Ns", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 2)
						{
							matInfo.specularExponent = (float)atof(v[1].c_str());
						}
					}
					if (line.rfind("d", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 2)
						{
							matInfo.transparency = (float)atof(v[1].c_str());
						}
					}
					if (line.rfind("Kd", 0) == 0)
					{
						v.clear();
						SplitEntry(&line, &v, ' ');
						if (v.size() == 4)
						{
							float r = (float)atof(v[1].c_str());
							float g = (float)atof(v[2].c_str());
							float b = (float)atof(v[3].c_str());
							matInfo.diffuse.Set(255, (int)(r * 255.0f), (int)(g * 255.0f), (int)(b * 255.0f));
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
			ZobFilePath zfpMaterial = ZobFilePath(materials.at(i).texture, zfp.GetPath(), materials.at(i).texture, zfp.IsAbsolute());
			LoadMaterial(n, &materials.at(i).ambient, &materials.at(i).diffuse, &materials.at(i).specular, materials.at(i).specularExponent, materials.at(i).transparency, zfpMaterial);
		}
		else
		{
			ZobFilePath zfpMaterial;
			LoadMaterial(n, &materials.at(i).ambient, &materials.at(i).diffuse, &materials.at(i).specular, materials.at(i).specularExponent, materials.at(i).transparency, zfpMaterial);
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

ZobMaterial* MaterialManager::GetMaterialNoConst(const int i) const
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
